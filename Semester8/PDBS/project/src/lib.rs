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
