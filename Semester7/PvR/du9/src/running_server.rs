use std::{
    net::TcpStream,
    sync::{Arc, Mutex},
    thread::{self, JoinHandle},
};

use crate::{ServerOpts, server::Server};

pub struct RunningServer {
    join_handle: Option<JoinHandle<()>>,
    pub port: u16,
    stopped: Arc<Mutex<bool>>,
}

impl RunningServer {
    pub fn new(opts: ServerOpts) -> Self {
        let stopped = Arc::new(Mutex::from(false));

        let mut server = Server::new(opts, stopped.clone());
        let port = server.get_port();

        let join_handle = Some(thread::spawn(move || {
            server.main_loop();
        }));

        Self {
            join_handle,
            port,
            stopped,
        }
    }

    pub fn port(&self) -> u16 {
        self.port
    }
}

impl Drop for RunningServer {
    fn drop(&mut self) {
        {
            *self.stopped.lock().unwrap() = true;
        }

        let _ = TcpStream::connect(("127.0.0.1", self.port));

        self.join_handle.take().unwrap().join().unwrap();
    }
}
