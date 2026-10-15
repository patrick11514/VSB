use serde::{Deserialize, Serialize};

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct File {
    pub name: String,
    pub last_modified: chrono::DateTime<chrono::Utc>,
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct ServerData {
    pub files: Vec<File>,
}

impl ServerData {
    pub fn new() -> Self {
        ServerData { files: Vec::new() }
    }
}

#[derive(Debug, Clone, Serialize, Deserialize)]
pub enum Message {
    Connected(String), //name
    CheckRevalidation(chrono::DateTime<chrono::Utc>),
    UpdatedFiles(Vec<File>),
}
