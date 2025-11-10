use std::{
    net::{TcpListener, TcpStream},
    sync::{Arc, Mutex},
    thread::{self, JoinHandle},
};

use anyhow::Context;

use crate::{ServerOpts, client::Client, messages::ServerToClientMsg};

pub struct RunningServer {
    exited: Arc<Mutex<bool>>,
    clients: Arc<Mutex<Vec<Client>>>,
    pub port: u16,
    main_thread: Option<JoinHandle<()>>,
}

impl RunningServer {
    pub fn new(opts: ServerOpts) -> anyhow::Result<Self> {
        let handle =
            TcpListener::bind(("127.0.0.1", 0)).context("Unable to bind server to port")?;

        let port = handle
            .local_addr()
            .context("Unable to get local address of listener")
            .unwrap()
            .port();

        let clients = Arc::new(Mutex::new(vec![]));
        let main_clients = clients.clone();
        let exited = Arc::new(Mutex::new(false));
        let main_exited = exited.clone();

        let main_thread = Some(thread::spawn(move || {
            while let Ok((stream, addr)) = handle.accept() {
                if *exited.lock().unwrap() {
                    break;
                }

                println!("New client connected from {}", addr);

                //clean clients
                let mut _clients = clients.lock().unwrap();

                _clients.retain(|t: &Client| !t.exited());

                let mut client = Client::new(clients.clone(), stream, addr).unwrap();

                if _clients.len() >= opts.max_clients {
                    println!("Server full, rejecting client {}", addr);
                    client.send_message(ServerToClientMsg::Error("Server is full".to_string()));
                    client.exit();
                    continue;
                }

                println!(
                    "Client {} connected, currently {} clients connected",
                    addr,
                    _clients.len() + 1
                );
                _clients.push(client);
            }
        }));

        Ok(Self {
            exited: main_exited,
            clients: main_clients,
            port,
            main_thread,
        })
    }

    pub fn port(&self) -> u16 {
        self.port
    }
}

impl Drop for RunningServer {
    //Wrote this function with help of Gemini (the .take() on Option + .drain(..) part)
    fn drop(&mut self) {
        *self.exited.lock().unwrap() = true;
        //connect to wake up server thread
        let _ = TcpStream::connect(("127.0.0.1", self.port));
        self.main_thread.take().unwrap().join().unwrap();

        let mut clients = self.clients.lock().unwrap();
        for client in clients.drain(..) {
            client.exit();
        }
    }
}
