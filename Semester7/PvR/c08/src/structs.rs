use std::collections::HashMap;

use serde::{Deserialize, Serialize};

#[derive(Serialize, Debug)]
pub enum ToServer {
    Join {
        name: String,
    },
    Draw {
        row: usize,
        col: usize,
        token: String,
    },
    GetState,
}

#[derive(Deserialize, Debug)]
pub enum FromServer {
    Welcome {
        token: String,
        width: usize,
        height: usize,
    },
    CanvasState(HashMap<String, Vec<(usize, usize)>>),
    Error(String),
}
