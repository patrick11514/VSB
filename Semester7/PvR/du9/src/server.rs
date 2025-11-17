use std::{
    collections::HashMap,
    io::ErrorKind,
    net::TcpListener,
    os::fd::AsRawFd,
    sync::{Arc, Mutex},
    time::Duration,
};

use anyhow::Context;
use epoll::{Event, Events};

use crate::{
    ServerOpts,
    client::{Client, ClientState},
    messages::ServerToClientMsg,
};

pub struct Server {
    listener: TcpListener,
    port: u16,
    epoll: i32,
    opt: ServerOpts,
    clients: HashMap<i32, Client>,
    name_map: HashMap<String, i32>,
    stopped: Arc<Mutex<bool>>,
}

#[derive(Debug)]
pub enum ForServer {
    Broadcast { message: String },
    SetName { name: String },
    DM { to: String, message: String },
    Disconnected,
    ListUsers,
}

const IDLE_JOIN: Duration = Duration::from_secs(2);

impl Server {
    pub fn new(opt: ServerOpts, stopped: Arc<Mutex<bool>>) -> Self {
        let listener = TcpListener::bind(("127.0.0.1", 0))
            .context("Unable to bind server")
            .unwrap();
        listener
            .set_nonblocking(true)
            .context("Unable to set server as non-blocking")
            .unwrap();

        let epoll = epoll::create(false)
            .context("Unable to initialize epoll")
            .unwrap();

        epoll::ctl(
            epoll,
            epoll::ControlOptions::EPOLL_CTL_ADD,
            listener.as_raw_fd(),
            Event::new(Events::EPOLLIN, listener.as_raw_fd() as u64),
        )
        .context("Unable to register listener fd to epoll")
        .unwrap();

        let addr = listener
            .local_addr()
            .context("Unable to get local address of server")
            .unwrap();

        Self {
            stopped,
            port: addr.port(),
            listener,
            epoll,
            opt,
            clients: HashMap::new(),
            name_map: HashMap::new(),
        }
    }

    pub fn main_loop(&mut self) {
        let mut events = vec![Event::new(Events::empty(), 0); 1024];

        loop {
            let wait = self
                .clients
                .iter()
                .map(|c| &c.1.state)
                .filter_map(|s| match s {
                    ClientState::NotLoggedIn(instant) => Some(instant),
                    _ => None,
                })
                .min()
                .map(|i| IDLE_JOIN.saturating_sub(i.elapsed()).as_millis() as i32)
                .unwrap_or(-1);

            let ev_count = epoll::wait(self.epoll, wait, &mut events)
                .context("Unable to wait for events")
                .unwrap();

            {
                if *self.stopped.lock().unwrap() {
                    return;
                }
            }

            self.clients.retain(|_, client| {
                if let ClientState::NotLoggedIn(instant) = client.state {
                    if instant.elapsed() > IDLE_JOIN {
                        let _ = client.unregister(self.epoll);
                        false
                    } else {
                        true
                    }
                } else {
                    true
                }
            });

            for event in &events[..ev_count] {
                let fd = event.data as i32;

                if fd == self.listener.as_raw_fd() {
                    let (stream, addr) = match self.listener.accept() {
                        Ok(client) => client,
                        Err(err) if err.kind() == ErrorKind::WouldBlock => {
                            continue;
                        }
                        Err(err) => panic!("{:?}", err),
                    };

                    stream
                        .set_nonblocking(true)
                        .context("Unable to set client as non-blocking")
                        .unwrap();

                    let mut client = Client::new(stream, addr);

                    if self.clients.len() >= self.opt.max_clients {
                        client.send(ServerToClientMsg::Error("Server is full".into()));
                        continue;
                    }

                    if client.register(self.epoll).is_err() {
                        continue;
                    }

                    self.clients.insert(client.fd(), client);
                }

                //If we have more messages and some of message returned, we need to loop again
                loop {
                    let mut client_msg = None;
                    let mut client_name = None;

                    {
                        if let Some(client) = self.clients.get_mut(&fd) {
                            client_name = client.get_name();

                            client_msg = match client.handle() {
                                Some(msg) => match msg {
                                    ForServer::SetName { name } => {
                                        if let std::collections::hash_map::Entry::Vacant(e) = self.name_map.entry(name) {
                                            client.send(ServerToClientMsg::Welcome);
                                            e.insert(fd);
                                        } else {
                                            client.send(ServerToClientMsg::Error(
                                                "Username already taken".into(),
                                            ));
                                        }
                                        continue;
                                    }
                                    ForServer::DM { to, message } => {
                                        match self.name_map.get(&to) {
                                            Some(_fd) => {
                                                if &fd == _fd {
                                                    client.send(ServerToClientMsg::Error(
                                                        "Cannot send a DM to yourself".into(),
                                                    ));
                                                } else {
                                                    self.clients.get_mut(_fd).unwrap().send(
                                                        ServerToClientMsg::Message {
                                                            from: client_name.clone().unwrap(),
                                                            message,
                                                        },
                                                    );
                                                }
                                            }
                                            None => {
                                                self.clients.get_mut(&fd).unwrap().send(
                                                    ServerToClientMsg::Error(format!(
                                                        "User {} does not exist",
                                                        to
                                                    )),
                                                );
                                            }
                                        }
                                        continue;
                                    }
                                    ForServer::ListUsers => {
                                        client.send(ServerToClientMsg::UserList {
                                            users: self.name_map.keys().map(|k| k.into()).collect(),
                                        });
                                        continue;
                                    }
                                    msg => Some(msg),
                                },
                                None => None,
                            }
                        }
                    }

                    if let Some(msg) = client_msg {
                        match msg {
                            ForServer::Disconnected => {
                                if let Some(client) = self.clients.remove(&fd)
                                    && let Some(name) = client.get_name() {
                                        self.name_map.remove(&name);
                                    }
                            }
                            ForServer::Broadcast { message } => {
                                for (_fd, client) in self.clients.iter_mut() {
                                    if &fd == _fd {
                                        continue;
                                    }
                                    client.send(ServerToClientMsg::Message {
                                        from: client_name.clone().unwrap(),
                                        message: message.clone(),
                                    });
                                }
                            }
                            _ => break,
                        }
                    } else {
                        break; //We don't have any message
                    }
                }
            }
        }
    }

    pub fn get_port(&self) -> u16 {
        self.port
    }
}

impl Drop for Server {
    fn drop(&mut self) {
        for client in self.clients.values() {
            let _ = client.unregister(self.epoll);
        }
    }
}
