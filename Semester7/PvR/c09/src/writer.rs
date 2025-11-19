use std::io::Write;

pub struct MessageWriter<W> {
    sink: W,
}

impl<W: Write> MessageWriter<W> {
    pub fn new(write: W) -> Self {
        Self { sink: write }
    }

    pub fn write(&mut self, message: &str) -> anyhow::Result<()> {
        let message = message.as_bytes();

        // Write size
        let size = message.len() as u32;
        self.sink.write_all(&size.to_le_bytes())?;

        // Write data
        self.sink.write_all(&message)?;
        self.sink.flush()?;
        Ok(())
    }
}
