use std::{println, rc::Rc, sync::Arc};

use CV05::{
    structs::{File, Message, ServerData},
    utils::{ReadOutput, read_message, write_message},
};
use rand::distr::{Alphanumeric, SampleString};
use tokio::{
    io::{AsyncBufReadExt, AsyncWriteExt, BufReader},
    net::TcpListener,
    sync::Mutex,
};

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    server().await?;

    Ok(())
}

fn random_file() -> File {
    let string = Alphanumeric.sample_string(&mut rand::rng(), 16);

    File {
        name: string,
        last_modified: chrono::Utc::now(),
    }
}

async fn random_cache_invalidation(server_data: Arc<Mutex<ServerData>>) {
    loop {
        tokio::time::sleep(tokio::time::Duration::from_secs(5)).await;

        //random cound
        let mut locked = server_data.lock().await;
        let file_count = locked.files.len();

        let random_count = rand::random::<u32>() % (((file_count + 1) / 2) as u32);

        println!("Randomly invalidating {} files", random_count);

        for _ in 0..random_count {
            let random_index = rand::random::<u32>() % (locked.files.len() as u32);
            //set modify time to now
            locked.files[random_index as usize].last_modified = chrono::Utc::now();
        }
    }
}

async fn server() -> anyhow::Result<()> {
    let addr = "0.0.0.0:8080";

    let server = TcpListener::bind(addr).await?;
    let server_data = Arc::new(Mutex::new(ServerData::new()));

    tokio::spawn(random_cache_invalidation(server_data.clone()));

    for _ in 0..10 {
        let mut locked = server_data.lock().await;

        locked.files.push(random_file());
    }

    loop {
        let (stream, _) = server.accept().await?;

        tokio::spawn(handle_clinet(stream, server_data.clone()));
    }
}

async fn handle_clinet(
    mut stream: tokio::net::TcpStream,
    server_data: Arc<Mutex<ServerData>>,
) -> anyhow::Result<()> {
    let (read, mut write) = stream.split();

    let mut reader = BufReader::new(read);

    loop {
        match read_message(&mut reader).await {
            ReadOutput::Message(message) => match message {
                Message::CheckRevalidation(timestamp) => {
                    let locked = server_data.lock().await;
                    let files_after = locked
                        .files
                        .iter()
                        .filter(|file| file.last_modified > timestamp)
                        .cloned()
                        .collect::<Vec<File>>();

                    write_message(&mut write, &Message::UpdatedFiles(files_after)).await?;
                }
                _ => {}
            },
            ReadOutput::Exited => break,
            ReadOutput::Error(error) => {
                println!("Error reading message: {:?}", error);
                break;
            }
        }
    }

    Ok(())
}
