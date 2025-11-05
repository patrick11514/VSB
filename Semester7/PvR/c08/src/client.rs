use std::{
    array::TryFromSliceError,
    io::{Read, Write},
    net::TcpStream,
    thread,
    time::Duration,
};

use crate::structs::{FromServer, ToServer};

pub struct Client {
    stream: TcpStream,
}

impl Client {
    pub fn new(ip: &str, port: u16) -> Self {
        let stream = TcpStream::connect((ip, port)).unwrap();

        Self { stream }
    }

    pub fn send_message(&mut self, message: ToServer) -> anyhow::Result<()> {
        let serialized = serde_json::to_vec(&message)?;
        let size = serialized.len() as u32;

        self.stream.write_all(&size.to_le_bytes()).unwrap();
        self.stream.write_all(&serialized).unwrap();
        self.stream.flush()?;

        Ok(())
    }

    pub fn read_message(&mut self) -> anyhow::Result<FromServer> {
        let mut message = [0; 4];
        self.stream.read_exact(&mut message)?;

        let size = u32::from_le_bytes(message);

        let mut message = vec![0 as u8; size as usize];

        self.stream.read_exact(&mut message)?;

        Ok(serde_json::from_slice(message.as_slice())?)
    }

    pub fn connect(mut self, name: String) -> anyhow::Result<ConnectedClient> {
        self.send_message(ToServer::Join { name })?;
        match self.read_message()? {
            FromServer::Welcome {
                token,
                width,
                height,
            } => Ok(ConnectedClient {
                client: self,
                token,
                width,
                height,
            }),
            msg => Err(anyhow::format_err!("{:?}", msg)),
        }
    }
}

pub struct ConnectedClient {
    client: Client,
    pub token: String,
    pub width: usize,
    pub height: usize,
}

impl ConnectedClient {
    pub fn send_message(&mut self, message: ToServer) -> anyhow::Result<()> {
        self.client.send_message(message)
    }

    pub fn read_message(&mut self) -> anyhow::Result<FromServer> {
        self.client.read_message()
    }

    pub fn draw_array(&mut self, x: usize, y: usize, data: String) -> anyhow::Result<()> {
        for (row_idx, row) in data.lines().enumerate() {
            for (col_idx, col) in row.chars().enumerate() {
                if col == '#' {
                    self.send_message(ToServer::Draw {
                        row: x + row_idx,
                        col: y + col_idx,
                        token: self.token.clone(),
                    })?;

                    thread::sleep(Duration::from_millis(100));
                }
            }
        }

        Ok(())
    }
}
