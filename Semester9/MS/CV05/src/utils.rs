use tokio::io::{AsyncBufReadExt, AsyncWrite, AsyncWriteExt};

use crate::structs::Message;

pub enum ReadOutput {
    Message(Message),
    Exited,
    Error(anyhow::Error),
}

pub async fn write_message<W: AsyncWrite + Unpin, M: serde::Serialize>(
    writer: &mut W,
    message: &M,
) -> anyhow::Result<()> {
    let json = serde_json::to_string(&message)?;

    writer.write_all(json.as_bytes()).await?;
    writer.write_all(b"\n").await?;

    Ok(())
}

pub async fn read_message<R: tokio::io::AsyncBufRead + Unpin>(reader: &mut R) -> ReadOutput {
    let mut line = String::new();

    match reader.read_line(&mut line).await {
        Ok(0) => ReadOutput::Exited,
        Ok(_) => match serde_json::from_str::<Message>(&line) {
            Ok(message) => ReadOutput::Message(message),
            Err(e) => ReadOutput::Error(anyhow::Error::new(e)),
        },
        Err(e) => ReadOutput::Error(anyhow::Error::new(e)),
    }
}
