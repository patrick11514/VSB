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

pub async fn geonear_objects(
    collection: &Collection<SkyObject>,
    center_lon: f64,
    center_lat: f64,
    radius_deg: f64,
    max_magnitude: Option<f64>,
    limit: usize,
    exclude_designation: Option<&str>,
    catalog_filter: Option<&str>,
) -> Result<Vec<(SkyObject, f64)>, Box<dyn Error + Send + Sync>> {
    use mongodb::bson::raw::RawBsonRef;

    let mut query_doc = doc! {};
    if let Some(exclude) = exclude_designation {
        query_doc.insert("designation", doc! { "$ne": exclude });
    }
    if let Some(catalog) = catalog_filter {
        query_doc.insert("source", doc! { "$regex": catalog, "$options": "i" });
    }

    let mut pipeline = vec![doc! {
        "$geoNear": {
            "near": {
                "type": "Point",
                "coordinates": [center_lon, center_lat]
            },
            "distanceField": "distance_degrees",
            "distanceMultiplier": 1.0 / 111_000.0,
            "spherical": true,
            "maxDistance": radius_deg * 111_000.0,
            "query": query_doc
        }
    }];

    if let Some(max_mag) = max_magnitude {
        pipeline.push(doc! { "$match": { "magnitude": { "$lte": max_mag } } });
    }

    pipeline.push(doc! { "$limit": limit as i32 });

    let mut cursor = collection.aggregate(pipeline).await?;
    let mut results = Vec::new();

    while cursor.advance().await? {
        let raw_doc = cursor.current();
        let obj: SkyObject = mongodb::bson::from_slice(raw_doc.as_bytes())?;

        let distance_degrees = match raw_doc.get("distance_degrees") {
            Ok(Some(RawBsonRef::Double(d))) => d,
            Ok(Some(RawBsonRef::Int32(i))) => i as f64,
            Ok(Some(RawBsonRef::Int64(i))) => i as f64,
            _ => 0.0,
        };

        results.push((obj, distance_degrees));
    }

    Ok(results)
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
