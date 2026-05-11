use std::error::Error;
use std::time::{SystemTime, UNIX_EPOCH};

use mongodb::Collection;
use mongodb::bson::{Bson, Document, doc};
use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct SkyObject {
    pub designation: String,
    #[serde(default, skip_serializing_if = "Vec::is_empty")]
    pub aliases: Vec<String>,
    pub ra_deg: f64,
    pub dec_deg: f64,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub magnitude: Option<f64>,
    #[serde(default, skip_serializing_if = "Option::is_none")]
    pub source: Option<String>,
    #[serde(default, skip_serializing_if = "Document::is_empty")]
    pub metadata: Document,
}

impl SkyObject {
    pub fn new(
        designation: impl Into<String>,
        aliases: Vec<String>,
        ra_deg: f64,
        dec_deg: f64,
        magnitude: Option<f64>,
        source: Option<String>,
        metadata: Document,
    ) -> Self {
        Self {
            designation: designation.into(),
            aliases,
            ra_deg,
            dec_deg,
            magnitude,
            source,
            metadata,
        }
    }

    pub fn normalized_longitude(&self) -> f64 {
        normalize_longitude(self.ra_deg)
    }

    pub fn to_document(&self) -> Document {
        let mut document = doc! {
            "designation": &self.designation,
            "aliases": self.aliases.clone(),
            "ra_deg": self.ra_deg,
            "dec_deg": self.dec_deg,
            "location": doc! {
                "type": "Point",
                "coordinates": [self.normalized_longitude(), self.dec_deg],
            },
            "metadata": self.metadata.clone(),
        };

        if let Some(magnitude) = self.magnitude {
            document.insert("magnitude", magnitude);
        } else {
            document.insert("magnitude", Bson::Null);
        }

        if let Some(source) = &self.source {
            document.insert("source", source.clone());
        }

        document.insert("_id", object_id(&self.source, &self.designation));
        document
    }
}

pub fn object_id(source: &Option<String>, designation: &str) -> String {
    match source {
        Some(source) => format!(
            "{}:{}",
            source.to_lowercase(),
            designation.trim().to_lowercase()
        ),
        None => designation.trim().to_lowercase(),
    }
}

pub fn normalize_longitude(mut longitude: f64) -> f64 {
    while longitude >= 180.0 {
        longitude -= 360.0;
    }

    while longitude < -180.0 {
        longitude += 360.0;
    }

    longitude
}

pub fn current_lst_deg(observer_longitude_deg: f64) -> f64 {
    let utc_seconds = SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap_or_default()
        .as_secs_f64();
    lst_deg_at_unix_seconds(observer_longitude_deg, utc_seconds)
}

pub fn lst_deg_at_unix_seconds(observer_longitude_deg: f64, unix_seconds: f64) -> f64 {
    normalize_longitude(unix_seconds / 86_164.090_5 * 360.0 + observer_longitude_deg)
}

pub fn altitude_deg(ra_deg: f64, dec_deg: f64, lst_deg: f64, latitude_deg: f64) -> f64 {
    let hour_angle = (normalize_longitude(lst_deg - ra_deg)).to_radians();
    let dec = dec_deg.to_radians();
    let latitude = latitude_deg.to_radians();

    let sin_altitude = dec.sin() * latitude.sin() + dec.cos() * latitude.cos() * hour_angle.cos();
    sin_altitude.clamp(-1.0, 1.0).asin().to_degrees()
}

pub async fn collection_count(
    collection: &Collection<SkyObject>,
) -> Result<u64, mongodb::error::Error> {
    collection.estimated_document_count().await
}

pub async fn search_objects(
    collection: &Collection<SkyObject>,
    query: &str,
    max_magnitude: Option<f64>,
    limit: usize,
) -> Result<Vec<SkyObject>, Box<dyn Error + Send + Sync>> {
    let filter = combine_filters(vec![
        build_name_filter(query),
        build_magnitude_filter(max_magnitude),
    ]);
    collect_find_results(collection, filter, limit).await
}

pub async fn nearby_objects(
    collection: &Collection<SkyObject>,
    center: &SkyObject,
    radius_deg: f64,
    max_magnitude: Option<f64>,
    limit: usize,
) -> Result<Vec<SkyObject>, Box<dyn Error + Send + Sync>> {
    let filter = combine_filters(vec![
        build_radius_filter(center, radius_deg),
        build_magnitude_filter(max_magnitude),
    ]);
    collect_find_results(collection, filter, limit).await
}

pub async fn visible_objects(
    collection: &Collection<SkyObject>,
    lst_deg: f64,
    horizon_limit_deg: f64,
    max_magnitude: Option<f64>,
    limit: usize,
) -> Result<Vec<SkyObject>, Box<dyn Error + Send + Sync>> {
    let visible_filter = build_visible_filter(lst_deg, horizon_limit_deg);
    let filter = combine_filters(vec![visible_filter, build_magnitude_filter(max_magnitude)]);
    collect_find_results(collection, filter, limit).await
}

fn build_name_filter(query: &str) -> Document {
    let query = query.trim();
    if query.is_empty() {
        return Document::new();
    }

    doc! {
        "$or": [
            { "designation": { "$regex": query, "$options": "i" } },
            { "aliases": { "$regex": query, "$options": "i" } },
        ]
    }
}

fn build_magnitude_filter(max_magnitude: Option<f64>) -> Document {
    match max_magnitude {
        Some(max_magnitude) => doc! { "magnitude": { "$lte": max_magnitude } },
        None => Document::new(),
    }
}

fn build_radius_filter(center: &SkyObject, radius_deg: f64) -> Document {
    if radius_deg <= 0.0 {
        return Document::new();
    }

    doc! {
        "location": {
            "$geoWithin": {
                "$centerSphere": [
                    [center.normalized_longitude(), center.dec_deg],
                    radius_deg.to_radians()
                ]
            }
        }
    }
}

fn build_visible_filter(lst_deg: f64, horizon_limit_deg: f64) -> Document {
    let left = normalize_longitude(lst_deg - 90.0);
    let right = normalize_longitude(lst_deg + 90.0);

    let make_ring = |west: f64, east: f64| {
        vec![
            vec![west, horizon_limit_deg],
            vec![east, horizon_limit_deg],
            vec![east, 90.0],
            vec![west, 90.0],
            vec![west, horizon_limit_deg],
        ]
    };

    if left <= right {
        doc! {
            "location": {
                "$geoWithin": {
                    "$geometry": {
                        "type": "Polygon",
                        "coordinates": [make_ring(left, right)],
                    }
                }
            }
        }
    } else {
        let western_ring = make_ring(left, 180.0);
        let eastern_ring = make_ring(-180.0, right);

        doc! {
            "$or": [
                {
                    "location": {
                        "$geoWithin": {
                            "$geometry": {
                                "type": "Polygon",
                                "coordinates": [western_ring],
                            }
                        }
                    }
                },
                {
                    "location": {
                        "$geoWithin": {
                            "$geometry": {
                                "type": "Polygon",
                                "coordinates": [eastern_ring],
                            }
                        }
                    }
                }
            ]
        }
    }
}

fn combine_filters(filters: Vec<Document>) -> Document {
    let filters: Vec<Document> = filters
        .into_iter()
        .filter(|filter| !filter.is_empty())
        .collect();

    match filters.len() {
        0 => Document::new(),
        1 => filters.into_iter().next().unwrap_or_default(),
        _ => doc! { "$and": filters },
    }
}

async fn collect_find_results(
    collection: &Collection<SkyObject>,
    filter: Document,
    limit: usize,
) -> Result<Vec<SkyObject>, Box<dyn Error + Send + Sync>> {
    let mut cursor = collection.find(filter).await?;
    let mut results = Vec::new();

    while cursor.advance().await? {
        let object: SkyObject = cursor.deserialize_current()?;
        results.push(object);

        if results.len() >= limit {
            break;
        }
    }

    results.sort_by(|left, right| left.designation.cmp(&right.designation));
    Ok(results)
}
