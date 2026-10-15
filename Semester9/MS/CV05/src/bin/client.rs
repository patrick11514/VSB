use std::{println, time::Duration};

use CV05::{
    structs::{File, Message},
    utils::{ReadOutput, read_message, write_message},
};
use chrono::{DateTime, Utc};
use structopt::StructOpt;
use tokio::{
    io::{AsyncBufReadExt, BufReader},
    net::TcpStream,
};

#[derive(Debug, StructOpt)]
struct Args {
    #[structopt(short, long)]
    pub name: String,
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    let args = Args::from_args();

    client(args).await?;

    Ok(())
}

async fn client(args: Args) -> anyhow::Result<()> {
    println!("args: {:?}", args);

    let addr = "0.0.0.0:8080";

    let mut stream = TcpStream::connect(addr).await?;

    let (read, mut write) = stream.split();

    let mut reader = BufReader::new(read);

    write_message(&mut write, &Message::Connected(args.name)).await?;

    let mut last_change = DateTime::<Utc>::from(std::time::UNIX_EPOCH);

    loop {
        tokio::time::sleep(Duration::from_secs(10)).await;

        write_message(&mut write, &Message::CheckRevalidation(last_change)).await?;

        last_change = Utc::now();

        match read_message(&mut reader).await {
            ReadOutput::Message(message) => match message {
                Message::UpdatedFiles(files) => {
                    println!("Received updated files: {:?}", files);
                }
                _ => {}
            },
            ReadOutput::Exited => todo!(),
            ReadOutput::Error(error) => todo!(),
        }
    }
}
