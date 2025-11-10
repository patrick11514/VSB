use std::{
    fmt::write,
    io::Write,
    net::{SocketAddr, TcpListener, TcpStream},
    sync::{Arc, Mutex},
    thread::{self, JoinHandle},
};

use anyhow::Context;

use crate::{
    messages::{ClientToServerMsg, ServerToClientMsg},
    reader::MessageReader,
    server::RunningServer,
    writer::MessageWriter,
};

type Reader = MessageReader<ClientToServerMsg, TcpStream>;
type Writer = MessageWriter<ServerToClientMsg, TcpStream>;

pub struct Client {
    username: Arc<Mutex<Option<String>>>,
    stream: TcpStream,
    writer: Writer,
    handle: JoinHandle<()>,
}

fn client_main(
    mut reader: Reader,
    mut writer: Writer,
    addr: SocketAddr,
    clients: Arc<Mutex<Vec<Client>>>,
    thread_username: Arc<Mutex<Option<String>>>,
) ->  {
    let username = match acquire_username(&mut reader, &mut writer, clients.clone()) {
        Some(res) => match res {
            Ok(username) => Some(username),
            _ => None,
        },
        None => {
            let _ = writer.write(ServerToClientMsg::Error(
                "Unexpected message received".into(),
            ));
            return;
        }
    };

    let username = if let Some(username) = username {
        *thread_username.lock().unwrap() = Some(username.clone());
        username
    } else {
        println!("Client {} disconnected", addr);
        return;
    };

    match writer.write(ServerToClientMsg::Welcome) {
        Err(_) => return,
        _ => {}
    }

    if let None = client_loop(reader, &mut writer, addr, username, clients) {
        let _ = writer.write(ServerToClientMsg::Error(
            "Unexpected message received".into(),
        ));
    }
}

fn acquire_username(
    reader: &mut Reader,
    writer: &mut Writer,
    clients: Arc<Mutex<Vec<Client>>>,
) -> Option<anyhow::Result<String>> {
    let msg = match reader.read() {
        Some(result) => match result {
            Ok(msg) => msg,
            Err(err) => return Some(Err(err)),
        },
        None => return Some(Err(anyhow::anyhow!("Unable to read message"))),
    };

    match msg {
        ClientToServerMsg::Join { name } => {
            let clients = clients.lock().unwrap();
            if clients.iter().any(|c| {
                if let Some(username) = c.username.lock().unwrap().clone() {
                    if username == name {
                        let _ = writer.write(ServerToClientMsg::Error(
                            "Username already taken".to_string(),
                        ));
                        return true;
                    }
                }

                return false;
            }) {
                return Some(Err(anyhow::anyhow!("Username already taken")));
            }

            Some(Ok(name))
        }
        _ => None,
    }
}

fn client_loop(
    reader: Reader,
    writer: &mut Writer,
    addr: SocketAddr,
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
                let clients = clients.lock().unwrap();
                let usernames = clients
                    .iter()
                    .filter_map(|c| {
                        let username = c.username.lock().unwrap().clone();
                        match username {
                            Some(username) => Some(username),
                            None => None,
                        }
                    })
                    .collect::<Vec<_>>();
                ServerToClientMsg::UserList { users: usernames }
            }
            ClientToServerMsg::SendDM { to, message } => {
                if &to == &name {
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
                            writer
                                .write(ServerToClientMsg::Message {
                                    from: name.clone(),
                                    message,
                                })
                                .unwrap();
                            continue;
                        }
                    }
                }
            }
            ClientToServerMsg::Broadcast { message } => {
                let mut clients = clients.lock().unwrap();
                for client in clients.iter_mut() {
                    let username = client.username.lock().unwrap();
                    if let Some(_name) = &*username {
                        if _name != &name {
                            let _ = client.writer.write(ServerToClientMsg::Message {
                                from: "Broadcast".to_string(),
                                message: message.clone(),
                            });
                        }
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

    println!("Client {} disconnected", addr);

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

        let handle = thread::spawn(move || {});

        Ok(Self {
            stream,
            username,
            writer: main_writer,
            handle,
        })
    }

    pub fn exited(&self) -> bool {
        self.handle.is_finished()
    }

    pub fn send_message(&mut self, msg: ServerToClientMsg) {
        let _ = self.writer.write(msg);
    }

    pub fn exit(self) {
        let _ = self.stream.shutdown(std::net::Shutdown::Both);
        let _ = self.handle.join();
    }
}
