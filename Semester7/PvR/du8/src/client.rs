#![allow(dead_code)]

use std::{
    net::{SocketAddr, TcpStream},
    sync::{Arc, Mutex},
    thread::{self, JoinHandle},
};

use anyhow::Context;

use crate::{
    messages::{ClientToServerMsg, ServerToClientMsg},
    reader::MessageReader,
    writer::MessageWriter,
};

type Reader = MessageReader<ClientToServerMsg, TcpStream>;
type Writer = MessageWriter<ServerToClientMsg, TcpStream>;

pub struct Client {
    username: Arc<Mutex<Option<String>>>,
    writer: Writer,
    handle: JoinHandle<()>,
    exited: Arc<Mutex<bool>>,
}

enum ClientError {
    UsernameError,
    UsernameTaken,
    WelcomeFailed,
    LoopError,
}

fn client_main(
    reader: &mut Reader,
    writer: &mut Writer,
    clients: Arc<Mutex<Vec<Client>>>,
    thread_username: Arc<Mutex<Option<String>>>,
) -> Result<(), ClientError> {
    println!("Acquiring username");
    let username = match acquire_username(reader, writer, clients.clone(), thread_username) {
        Some(res) => match res {
            Ok(username) => username,
            Err(err) => {
                return Err(match err {
                    UsernameError::UnableToRead => ClientError::UsernameError,
                    UsernameError::Taken => ClientError::UsernameTaken,
                    UsernameError::Other(_) => ClientError::UsernameError,
                });
            }
        },
        None => {
            let _ = writer.write(ServerToClientMsg::Error(
                "Unexpected message received".into(),
            ));
            return Err(ClientError::UsernameError);
        }
    };

    println!("Sending welcome message");

    if let Err(_) = writer.write(ServerToClientMsg::Welcome) { return Err(ClientError::WelcomeFailed) }

    println!("Entering client loop for user {}", username);
    if client_loop(reader, writer, username, clients).is_none() {
        return Err(ClientError::LoopError);
    }

    Ok(())
}

enum UsernameError {
    UnableToRead,
    Taken,
    Other(anyhow::Error),
}

fn acquire_username(
    reader: &mut Reader,
    writer: &mut Writer,
    clients: Arc<Mutex<Vec<Client>>>,
    thread_username: Arc<Mutex<Option<String>>>,
) -> Option<Result<String, UsernameError>> {
    let msg = match reader.read() {
        Some(result) => match result {
            Ok(msg) => msg,
            Err(err) => return Some(Err(UsernameError::Other(err))),
        },
        None => return Some(Err(UsernameError::UnableToRead)),
    };

    match msg {
        ClientToServerMsg::Join { name } => {
            let clients = clients.lock().unwrap();
            if clients.iter().any(|c| {
                if let Some(username) = c.username.lock().unwrap().clone()
                    && username == name {
                        let _ = writer.write(ServerToClientMsg::Error(
                            "Username already taken".to_string(),
                        ));
                        return true;
                    }

                false
            }) {
                return Some(Err(UsernameError::Taken));
            }

            *thread_username.lock().unwrap() = Some(name.clone());
            Some(Ok(name))
        }
        _ => None,
    }
}

fn client_loop(
    reader: &mut Reader,
    writer: &mut Writer,
    name: String,
    clients: Arc<Mutex<Vec<Client>>>,
) -> Option<()> {
    for msg in reader {
        let msg = match msg {
            Ok(msg) => msg,
            Err(_) => break,
        };

        let msg = match msg {
            ClientToServerMsg::Ping => ServerToClientMsg::Pong,
            ClientToServerMsg::ListUsers => {
                println!("Listing users for {}", name);

                let clients = clients.lock().unwrap();
                let usernames = clients
                    .iter()
                    .filter_map(|c| {
                        let username = c.username.lock().unwrap().clone();
                        username
                    })
                    .collect::<Vec<_>>();
                ServerToClientMsg::UserList { users: usernames }
            }
            ClientToServerMsg::SendDM { to, message } => {
                println!("{} sending DM to {}", name, to);

                if to == name {
                    ServerToClientMsg::Error("Cannot send a DM to yourself".to_string())
                } else {
                    let mut clients = clients.lock().unwrap();
                    let writer = clients.iter_mut().find_map(|c| {
                        let username = c.username.lock().unwrap().clone();
                        match username {
                            Some(username) => {
                                if username == to {
                                    Some(&mut c.writer)
                                } else {
                                    None
                                }
                            }
                            None => None,
                        }
                    });

                    match writer {
                        None => ServerToClientMsg::Error(format!("User {} does not exist", to)),
                        Some(writer) => {
                            let _ = writer.write(ServerToClientMsg::Message {
                                from: name.clone(),
                                message,
                            });

                            continue;
                        }
                    }
                }
            }
            ClientToServerMsg::Broadcast { message } => {
                println!("{} broadcasting message", name);

                let mut clients = clients.lock().unwrap();
                for client in clients.iter_mut() {
                    let username = client.username.lock().unwrap();
                    if let Some(_name) = &*username
                        && _name != &name {
                            let _ = client.writer.write(ServerToClientMsg::Message {
                                from: name.clone(),
                                message: message.clone(),
                            });
                        }
                }
                continue;
            }
            ClientToServerMsg::Join { .. } => return None,
        };

        if let Err(_) = writer.write(msg) {
            break;
        }
    }

    Some(())
}

impl Client {
    pub fn new(
        clients: Arc<Mutex<Vec<Client>>>,
        stream: TcpStream,
        addr: SocketAddr,
    ) -> anyhow::Result<Self> {
        let mut reader =
            MessageReader::new(stream.try_clone().context("Unable to clone tcp stream")?);
        let mut writer =
            MessageWriter::new(stream.try_clone().context("Unable to clone tcp stream")?);
        let main_writer =
            MessageWriter::new(stream.try_clone().context("Unable to clone tcp stream")?);
        let username = Arc::new(Mutex::new(None));
        let thread_username = username.clone();
        let cleanup_username = username.clone();

        let exited = Arc::new(Mutex::new(false));
        let thread_exited = exited.clone();

        let handle = thread::spawn(move || {
            if let Err(err) =
                client_main(&mut reader, &mut writer, clients.clone(), thread_username)
            {
                match err {
                    ClientError::UsernameError | ClientError::LoopError => {
                        let _ = writer.write(ServerToClientMsg::Error(
                            "Unexpected message received".into(),
                        ));
                    }
                    _ => {}
                }
            };

            //Here if we exited, we don't want to remove ourselves from the client list
            //because Server will do that for us :)
            if *thread_exited.lock().unwrap() {
                return;
            }

            println!("Cleaning up client {}", addr);

            {
                let my_username = { cleanup_username.lock().unwrap().clone() };

                let mut clients = clients.lock().unwrap();
                //Remove client from list
                clients.retain(|c: &Client| {
                    let username = c.username.lock().unwrap();
                    match &*username {
                        Some(name) => {
                            if let Some(username) = &my_username
                                && name == username {
                                    return false;
                                }
                            true
                        }
                        None => true,
                    }
                });
                println!("CLEANUP DONE");
            }

            println!("User {} disconnected", addr);
        });

        Ok(Self {
            username,
            writer: main_writer,
            handle,
            exited,
        })
    }

    pub fn exited(&self) -> bool {
        self.handle.is_finished()
    }

    pub fn send_message(&mut self, msg: ServerToClientMsg) {
        let _ = self.writer.write(msg);
    }

    pub fn exit(self) {
        println!("Exiting client");
        {
            let mut exited = self.exited.lock().unwrap();
            *exited = true;
        }

        println!("Shutting down client writer");
        self.writer
            .into_inner()
            .shutdown(std::net::Shutdown::Both)
            .ok();

        println!("Joining client thread");
        let _ = self.handle.join();
    }
}
