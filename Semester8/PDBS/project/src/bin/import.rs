use std::collections::HashSet;
use std::error::Error;
use std::fmt::{Display, Formatter};
use std::fs::File;
use std::path::{Path, PathBuf};

use clap::Parser;
use csv::ReaderBuilder;
use mongodb::bson::{Document, doc};
use mongodb::options::IndexOptions;
use mongodb::{Client, Collection, IndexModel};
use serde::Deserialize;

use project::{SkyObject, normalize_longitude};

const DEFAULT_MONGO_URI: &str = "mongodb://10.10.10.232:27017";
const DEFAULT_DATABASE: &str = "celestial_data";
const DEFAULT_COLLECTION: &str = "objects";
const DEFAULT_DATA_DIR: &str = "data";

#[derive(Debug, Parser)]
#[command(name = "import", about = "Import celestial CSV files into MongoDB")]
struct Args {
    #[arg(long, default_value = DEFAULT_MONGO_URI)]
    mongo_uri: String,

    #[arg(long, default_value = DEFAULT_DATABASE)]
    database: String,

    #[arg(long, default_value = DEFAULT_COLLECTION)]
    collection: String,

    #[arg(long, default_value = DEFAULT_DATA_DIR)]
    data_dir: PathBuf,
}

#[derive(Debug, Default)]
struct ImportStats {
    imported: usize,
    skipped: usize,
}

#[derive(Debug)]
struct ImportRowError {
    source: &'static str,
    line: usize,
    message: String,
}

impl Display for ImportRowError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{} line {}: {}", self.source, self.line, self.message)
    }
}

impl Error for ImportRowError {}

#[derive(Debug, Deserialize)]
struct NgcRow {
    #[serde(rename = "Name")]
    name: String,
    #[serde(rename = "RA")]
    ra: String,
    #[serde(rename = "Dec")]
    dec: String,
    #[serde(rename = "Const")]
    constellation: Option<String>,
    #[serde(rename = "B-Mag")]
    b_mag: Option<f64>,
    #[serde(rename = "V-Mag")]
    v_mag: Option<f64>,
    #[serde(rename = "J-Mag")]
    j_mag: Option<f64>,
    #[serde(rename = "H-Mag")]
    h_mag: Option<f64>,
    #[serde(rename = "K-Mag")]
    k_mag: Option<f64>,
    #[serde(rename = "Hubble")]
    hubble: Option<String>,
    #[serde(rename = "Cstar Names")]
    cstar_names: Option<String>,
    #[serde(rename = "Identifiers")]
    identifiers: Option<String>,
    #[serde(rename = "Common names")]
    common_names: Option<String>,
    #[serde(rename = "NED notes")]
    ned_notes: Option<String>,
    #[serde(rename = "OpenNGC notes")]
    openngc_notes: Option<String>,
    #[serde(rename = "Sources")]
    sources: Option<String>,
    #[serde(rename = "NGC")]
    ngc: Option<String>,
    #[serde(rename = "IC")]
    ic: Option<String>,
    #[serde(rename = "M")]
    messier: Option<String>,
}

#[derive(Debug, Deserialize)]
struct MessierRow {
    messier_id: Option<u32>,
    ngc_id: Option<u32>,
    v_mag: Option<f64>,
    comments: Option<String>,
    ra: Option<f64>,
    dec: Option<f64>,
    #[serde(rename = "ref")]
    reference: Option<String>,
    is_up: Option<String>,
}

#[derive(Debug, Deserialize)]
struct HygRow {
    id: Option<u32>,
    hip: Option<u32>,
    hd: Option<u32>,
    hr: Option<u32>,
    gl: Option<String>,
    bf: Option<String>,
    proper: Option<String>,
    ra: Option<f64>,
    dec: Option<f64>,
    mag: Option<f64>,
    spect: Option<String>,
    con: Option<String>,
    bayer: Option<String>,
    flam: Option<String>,
    base: Option<String>,
    var: Option<String>,
    lum: Option<f64>,
    comp: Option<u32>,
    comp_primary: Option<u32>,
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let args = Args::parse();

    let client = Client::with_uri_str(&args.mongo_uri).await?;
    let database = client.database(&args.database);

    ensure_collection(&database, &args.collection).await?;
    let collection = database.collection::<Document>(&args.collection);

    ensure_indexes(&collection).await?;

    let mut total = ImportStats::default();
    total += import_ngc(&collection, &args.data_dir.join("NGC.csv")).await?;
    total += import_messier(&collection, &args.data_dir.join("messier.csv")).await?;
    total += import_hyg(&collection, &args.data_dir.join("hyg_v42.csv")).await?;

    println!(
        "Import finished: {} imported, {} skipped",
        total.imported, total.skipped
    );

    Ok(())
}

impl std::ops::AddAssign for ImportStats {
    fn add_assign(&mut self, rhs: Self) {
        self.imported += rhs.imported;
        self.skipped += rhs.skipped;
    }
}

async fn ensure_collection(
    database: &mongodb::Database,
    collection_name: &str,
) -> Result<(), Box<dyn Error>> {
    let collections = database.list_collection_names().await?;
    if !collections
        .iter()
        .any(|existing| existing == collection_name)
    {
        database.create_collection(collection_name).await?;
    }

    Ok(())
}

async fn ensure_indexes(collection: &Collection<Document>) -> Result<(), Box<dyn Error>> {
    let geo_index = IndexModel::builder()
        .keys(doc! { "location": "2dsphere" })
        .options(
            IndexOptions::builder()
                .name("location_2dsphere".to_string())
                .build(),
        )
        .build();
    collection.create_index(geo_index).await?;

    let text_index = IndexModel::builder()
        .keys(doc! { "designation": "text", "names": "text" })
        .options(
            IndexOptions::builder()
                .name("designation_names_text".to_string())
                .build(),
        )
        .build();
    collection.create_index(text_index).await?;

    let magnitude_index = IndexModel::builder()
        .keys(doc! { "magnitude": 1 })
        .options(
            IndexOptions::builder()
                .name("magnitude_1".to_string())
                .build(),
        )
        .build();
    collection.create_index(magnitude_index).await?;

    Ok(())
}

async fn import_ngc(
    collection: &Collection<Document>,
    path: &Path,
) -> Result<ImportStats, Box<dyn Error>> {
    let mut reader = ReaderBuilder::new()
        .delimiter(b';')
        .from_reader(open_csv(path)?);
    let mut stats = ImportStats::default();

    for (line, result) in reader.deserialize::<NgcRow>().enumerate() {
        match result {
            Ok(row) => match to_ngc_document(row) {
                Ok(document) => {
                    upsert_document(collection, document).await?;
                    stats.imported += 1;
                }
                Err(message) => {
                    stats.skipped += 1;
                    eprintln!(
                        "{}",
                        ImportRowError {
                            source: "NGC",
                            line: line + 2,
                            message
                        }
                    );
                }
            },
            Err(error) => {
                stats.skipped += 1;
                eprintln!(
                    "{}",
                    ImportRowError {
                        source: "NGC",
                        line: line + 2,
                        message: error.to_string()
                    }
                );
            }
        }
    }

    println!(
        "NGC.csv: {} imported, {} skipped",
        stats.imported, stats.skipped
    );
    Ok(stats)
}

async fn import_messier(
    collection: &Collection<Document>,
    path: &Path,
) -> Result<ImportStats, Box<dyn Error>> {
    let mut reader = ReaderBuilder::new().from_reader(open_csv(path)?);
    let mut stats = ImportStats::default();

    for (line, result) in reader.deserialize::<MessierRow>().enumerate() {
        match result {
            Ok(row) => match to_messier_document(row) {
                Ok(document) => {
                    upsert_document(collection, document).await?;
                    stats.imported += 1;
                }
                Err(message) => {
                    stats.skipped += 1;
                    eprintln!(
                        "{}",
                        ImportRowError {
                            source: "Messier",
                            line: line + 2,
                            message
                        }
                    );
                }
            },
            Err(error) => {
                stats.skipped += 1;
                eprintln!(
                    "{}",
                    ImportRowError {
                        source: "Messier",
                        line: line + 2,
                        message: error.to_string()
                    }
                );
            }
        }
    }

    println!(
        "messier.csv: {} imported, {} skipped",
        stats.imported, stats.skipped
    );
    Ok(stats)
}

async fn import_hyg(
    collection: &Collection<Document>,
    path: &Path,
) -> Result<ImportStats, Box<dyn Error>> {
    let mut reader = ReaderBuilder::new().from_reader(open_csv(path)?);
    let mut stats = ImportStats::default();

    for (line, result) in reader.deserialize::<HygRow>().enumerate() {
        match result {
            Ok(row) => match to_hyg_document(row) {
                Ok(document) => {
                    upsert_document(collection, document).await?;
                    stats.imported += 1;
                }
                Err(message) => {
                    stats.skipped += 1;
                    eprintln!(
                        "{}",
                        ImportRowError {
                            source: "HYG",
                            line: line + 2,
                            message
                        }
                    );
                }
            },
            Err(error) => {
                stats.skipped += 1;
                eprintln!(
                    "{}",
                    ImportRowError {
                        source: "HYG",
                        line: line + 2,
                        message: error.to_string()
                    }
                );
            }
        }
    }

    println!(
        "hyg_v42.csv: {} imported, {} skipped",
        stats.imported, stats.skipped
    );
    Ok(stats)
}

fn open_csv(path: &Path) -> Result<File, Box<dyn Error>> {
    Ok(File::open(path)?)
}

async fn upsert_document(
    collection: &Collection<Document>,
    object: SkyObject,
) -> Result<(), Box<dyn Error>> {
    let document = object.to_document();
    let id = document.get_str("_id")?.to_string();
    collection
        .replace_one(doc! { "_id": id }, document)
        .upsert(true)
        .await?;
    Ok(())
}

fn to_ngc_document(row: NgcRow) -> Result<SkyObject, String> {
    let designation = row.name.trim();
    if designation.is_empty() {
        return Err("missing designation".to_string());
    }

    let ra = normalize_longitude(
        parse_ra_to_degrees(&row.ra).ok_or_else(|| format!("invalid RA {:?}", row.ra))?,
    );
    let dec = parse_dec_to_degrees(&row.dec).ok_or_else(|| format!("invalid Dec {:?}", row.dec))?;

    let mut metadata = doc! {
        "source": "NGC",
        "source_file": "NGC.csv",
    };

    if let Some(value) = row.hubble {
        metadata.insert("hubble", value);
    }
    if let Some(value) = row.sources {
        metadata.insert("sources", split_names(&value));
    }
    if let Some(value) = row.ned_notes {
        metadata.insert("ned_notes", value);
    }
    if let Some(value) = row.openngc_notes {
        metadata.insert("openngc_notes", value);
    }

    let mut names = collect_names([
        row.common_names.as_deref(),
        row.identifiers.as_deref(),
        row.cstar_names.as_deref(),
        row.ngc.as_deref(),
        row.ic.as_deref(),
        row.messier.as_deref(),
    ]);
    names.sort();
    names.dedup();

    metadata.insert("constellation", row.constellation.unwrap_or_default());

    Ok(SkyObject::new(
        designation,
        names,
        ra,
        dec,
        row.v_mag
            .or(row.b_mag)
            .or(row.j_mag)
            .or(row.h_mag)
            .or(row.k_mag),
        Some("NGC".to_string()),
        metadata,
    ))
}

fn to_messier_document(row: MessierRow) -> Result<SkyObject, String> {
    let designation = row
        .messier_id
        .map(|value| format!("M{}", value))
        .or_else(|| row.ngc_id.map(|value| format!("NGC {}", value)))
        .ok_or_else(|| "missing messier_id and ngc_id".to_string())?;

    let ra = normalize_longitude(row.ra.ok_or_else(|| "missing RA".to_string())? * 15.0);
    let dec = row.dec.ok_or_else(|| "missing Dec".to_string())?;
    let mut names = collect_names([row.comments.as_deref()]);
    names.push(designation.clone());
    names.sort();
    names.dedup();

    let mut metadata = doc! {
        "source": "Messier",
        "source_file": "messier.csv",
    };
    if let Some(value) = row.ngc_id {
        metadata.insert("ngc_id", value as i64);
    }
    if let Some(value) = row.reference {
        metadata.insert("reference", value);
    }
    if let Some(value) = row.is_up {
        metadata.insert("is_up", value);
    }

    Ok(SkyObject::new(
        designation,
        names,
        ra,
        dec,
        row.v_mag,
        Some("Messier".to_string()),
        metadata,
    ))
}

fn to_hyg_document(row: HygRow) -> Result<SkyObject, String> {
    let designation = row
        .proper
        .as_deref()
        .and_then(normalize_optional_string)
        .or_else(|| row.bf.as_deref().and_then(normalize_optional_string))
        .or_else(|| row.gl.as_deref().and_then(normalize_optional_string))
        .or_else(|| row.hd.map(|value| format!("HD {}", value)))
        .or_else(|| row.hip.map(|value| format!("HIP {}", value)))
        .or_else(|| row.hr.map(|value| format!("HR {}", value)))
        .or_else(|| row.id.map(|value| format!("HYG {}", value)))
        .ok_or_else(|| "missing designation".to_string())?;

    let ra = normalize_longitude(row.ra.ok_or_else(|| "missing RA".to_string())? * 15.0);
    let dec = row.dec.ok_or_else(|| "missing Dec".to_string())?;

    let mut names = collect_names([
        row.proper.as_deref(),
        row.bf.as_deref(),
        row.gl.as_deref(),
        row.bayer.as_deref(),
        row.flam.as_deref(),
        row.base.as_deref(),
        row.var.as_deref(),
        row.con.as_deref(),
    ]);
    if let Some(value) = row.hd {
        names.push(format!("HD {}", value));
    }
    if let Some(value) = row.hip {
        names.push(format!("HIP {}", value));
    }
    if let Some(value) = row.hr {
        names.push(format!("HR {}", value));
    }
    names.sort();
    names.dedup();

    let mut metadata = doc! {
        "source": "HYG",
        "source_file": "hyg_v42.csv",
    };
    if let Some(value) = row.id {
        metadata.insert("id", value as i64);
    }
    if let Some(value) = row.hip {
        metadata.insert("hip", value as i64);
    }
    if let Some(value) = row.hd {
        metadata.insert("hd", value as i64);
    }
    if let Some(value) = row.hr {
        metadata.insert("hr", value as i64);
    }
    if let Some(value) = row.spect.as_ref() {
        metadata.insert("spectral_type", value.clone());
    }
    if let Some(value) = row.lum {
        metadata.insert("luminosity", value);
    }
    if let Some(value) = row.comp {
        metadata.insert("comp", value as i64);
    }
    if let Some(value) = row.comp_primary {
        metadata.insert("comp_primary", value as i64);
    }

    metadata.insert("constellation", row.con.unwrap_or_default());

    Ok(SkyObject::new(
        designation,
        names,
        ra,
        dec,
        row.mag,
        Some("HYG".to_string()),
        metadata,
    ))
}

fn collect_names<'a, I>(values: I) -> Vec<String>
where
    I: IntoIterator<Item = Option<&'a str>>,
{
    let mut seen = HashSet::new();
    let mut names = Vec::new();

    for value in values {
        if let Some(value) = value {
            for name in split_names(value) {
                if seen.insert(name.clone()) {
                    names.push(name);
                }
            }
        }
    }

    names
}

fn split_names(value: &str) -> Vec<String> {
    value
        .split([',', ';', '|'])
        .map(str::trim)
        .filter(|value| !value.is_empty())
        .map(|value| value.to_string())
        .collect()
}

fn normalize_optional_string(value: &str) -> Option<String> {
    let value = value.trim();
    if value.is_empty() {
        None
    } else {
        Some(value.to_string())
    }
}

fn parse_ra_to_degrees(value: &str) -> Option<f64> {
    let value = value.trim();
    if value.is_empty() {
        return None;
    }

    if value.contains(':') {
        parse_hms(value).map(|hours| hours * 15.0)
    } else {
        value.parse::<f64>().ok().map(|hours| hours * 15.0)
    }
}

fn parse_dec_to_degrees(value: &str) -> Option<f64> {
    let value = value.trim();
    if value.is_empty() {
        return None;
    }

    if value.contains(':') {
        parse_dms(value)
    } else {
        value.parse::<f64>().ok()
    }
}

fn parse_hms(value: &str) -> Option<f64> {
    let parts = value
        .split([':', ' '])
        .filter(|part| !part.is_empty())
        .collect::<Vec<_>>();

    if parts.is_empty() {
        return None;
    }

    let hours = parts.first()?.parse::<f64>().ok()?;
    let minutes = parts
        .get(1)
        .and_then(|part| part.parse::<f64>().ok())
        .unwrap_or(0.0);
    let seconds = parts
        .get(2)
        .and_then(|part| part.parse::<f64>().ok())
        .unwrap_or(0.0);

    Some(hours + minutes / 60.0 + seconds / 3600.0)
}

fn parse_dms(value: &str) -> Option<f64> {
    let sign = if value.trim_start().starts_with('-') {
        -1.0
    } else {
        1.0
    };
    let normalized = value.trim_start_matches(['+', '-']);
    let parts = normalized
        .split([':', ' '])
        .filter(|part| !part.is_empty())
        .collect::<Vec<_>>();

    if parts.is_empty() {
        return None;
    }

    let degrees = parts.first()?.parse::<f64>().ok()?;
    let minutes = parts
        .get(1)
        .and_then(|part| part.parse::<f64>().ok())
        .unwrap_or(0.0);
    let seconds = parts
        .get(2)
        .and_then(|part| part.parse::<f64>().ok())
        .unwrap_or(0.0);

    Some(sign * (degrees + minutes / 60.0 + seconds / 3600.0))
}
