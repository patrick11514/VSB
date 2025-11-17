use std::{
    io::{self, ErrorKind},
    net::{SocketAddr, TcpStream},
    os::fd::AsRawFd,
    time::Instant,
};

use anyhow::Context;
use epoll::{Event, Events};

use crate::{
    messages::{ClientToServerMsg, ServerToClientMsg},
    reader::MessageReader,
    server::ForServer,
    writer::MessageWriter,
};

pub enum ClientState {
    NotLoggedIn(Instant),
    Logged,
}

pub struct Client {
    reader: MessageReader<ClientToServerMsg, TcpStream>,
    writer: MessageWriter<ServerToClientMsg, TcpStream>,
    addr: SocketAddr,
    name: Option<String>,
    pub state: ClientState,
}

impl Client {
    pub fn new(stream: TcpStream, addr: SocketAddr) -> Self {
        println!("Client {} connected!", addr);

        let reader = MessageReader::new(
            stream
                .try_clone()
                .context("Unable to clone TCPStream for MessageReader")
                .unwrap(),
        );
        let writer = MessageWriter::new(stream);

        Self {
            reader,
            writer,
            addr,
            name: None,
            state: ClientState::NotLoggedIn(Instant::now()),
        }
    }

    pub fn register(&self, epoll: i32) -> io::Result<()> {
        epoll::ctl(
            epoll,
            epoll::ControlOptions::EPOLL_CTL_ADD,
            self.reader.inner().as_raw_fd(),
            Event::new(Events::EPOLLIN, self.reader.inner().as_raw_fd() as u64),
        )
    }

    pub fn unregister(&self, epoll: i32) -> io::Result<()> {
        epoll::ctl(
            epoll,
            epoll::ControlOptions::EPOLL_CTL_DEL,
            self.reader.inner().as_raw_fd(),
            Event::new(Events::EPOLLIN, self.reader.inner().as_raw_fd() as u64),
        )
    }

    pub fn fd(&self) -> i32 {
        self.reader.inner().as_raw_fd()
    }

    pub fn get_name(&self) -> Option<String> {
        self.name.clone()
    }

    fn handle_login(&mut self, msg: ClientToServerMsg) -> Option<ForServer> {
        match msg {
            ClientToServerMsg::Join { name } => {
                if self.name.is_some() {
                    self.send(ServerToClientMsg::Error(
                        "Unexpected message received".into(),
                    ));
                    return Some(ForServer::Disconnected);
                }

                self.name = Some(name.clone());
                self.state = ClientState::Logged;

                Some(ForServer::SetName { name })
            }
            _ => {
                self.send(ServerToClientMsg::Error(
                    "Unexpected message received".into(),
                ));
                Some(ForServer::Disconnected)
            }
        }
    }

    fn handle_msg(&mut self, msg: ClientToServerMsg) -> Option<ForServer> {
        match msg {
            ClientToServerMsg::Join { .. } => {
                self.send(ServerToClientMsg::Error(
                    "Unexpected message received".into(),
                ));
                Some(ForServer::Disconnected)
            }
            ClientToServerMsg::Ping => {
                self.send(ServerToClientMsg::Pong);
                None
            }
            ClientToServerMsg::ListUsers => Some(ForServer::ListUsers),
            ClientToServerMsg::SendDM { to, message } => Some(ForServer::DM { to, message }),
            ClientToServerMsg::Broadcast { message } => Some(ForServer::Broadcast { message }),
        }
    }

    pub fn handle(&mut self) -> Option<ForServer> {
        while let Some(msg) = self.reader.recv() {
            match msg {
                Ok(msg) => {
                    let res = match self.state {
                        ClientState::NotLoggedIn(_) => self.handle_login(msg),
                        ClientState::Logged => self.handle_msg(msg),
                    };
                    if res.is_none() {
                        continue;
                    }
                    return res;
                }
                Err(err) if err.kind() == ErrorKind::WouldBlock => return None,
                _ => return Some(ForServer::Disconnected),
            }
        }

        Some(ForServer::Disconnected)
    }

    pub fn send(&mut self, msg: ServerToClientMsg) {
        let _ = self.writer.send(msg);
    }

    pub fn get_address(&self) -> SocketAddr {
        self.addr
    }
}
