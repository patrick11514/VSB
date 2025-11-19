use std::io::{ErrorKind, Read};

const MAX_MESSAGE_SIZE: u32 = 256;

pub struct MessageReader<R> {
    stream: R,
}

impl<R: Read> MessageReader<R> {
    pub fn new(read: R) -> Self {
        Self { stream: read }
    }

    pub fn read(&mut self) -> Option<anyhow::Result<String>> {
        // Read message size
        let mut message = [0; 4];
        match self.stream.read_exact(&mut message) {
            Ok(_) => {}
            Err(error) if error.kind() == ErrorKind::UnexpectedEof => {
                return None;
            }
            Err(error) => return Some(Err(error.into())),
        }

        let size = u32::from_le_bytes(message);
        if size > MAX_MESSAGE_SIZE {
            return Some(Err(anyhow::anyhow!("Message too large ({size} bytes)")));
        }

        // Read message
        let mut buffer = vec![0; size as usize];

        if let Err(error) = self.stream.read_exact(&mut buffer) {
            return Some(Err(anyhow::anyhow!("Cannot read message: {error:?}")));
        }

        // Deserialize string
        match String::from_utf8(buffer) {
            Ok(string) => Some(Ok(string)),
            Err(error) => Some(Err(anyhow::anyhow!("Cannot deserialize string: {error:?}"))),
        }
    }
}
