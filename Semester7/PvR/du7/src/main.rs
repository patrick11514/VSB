//! You can use this file for experiments.
use std::{
    sync::{
        RwLock,
        mpsc::{self, Receiver, SyncSender},
    },
    thread::{self, JoinHandle},
};

struct WorkerQueue<T: Send> {
    size: usize,
    current: RwLock<usize>,
    queue: Receiver<T>,
    threads: Vec<(SyncSender<Box<dyn Fn() -> T + Send>>, JoinHandle<()>)>,
}

impl<T: Send + 'static> WorkerQueue<T> {
    fn new(thread_count: usize, queue_size: usize) -> Self
where {
        let (main_tx, main_rx) = mpsc::channel::<T>();

        let mut threads = Vec::with_capacity(thread_count);

        for _ in 0..thread_count {
            let (tx, rx) = mpsc::sync_channel::<Box<dyn Fn() -> T + Send>>(queue_size);

            let main_tx = main_tx.clone();
            threads.push((
                tx,
                thread::spawn(move || {
                    while let Ok(f) = rx.recv() {
                        main_tx.send(f()).unwrap();
                    }
                }),
            ));
        }

        WorkerQueue {
            size: thread_count,
            current: RwLock::new(0),
            queue: main_rx,
            threads,
        }
    }

    fn close(self) {
        for t in self.threads {
            drop(t.0);
            t.1.join().unwrap()
        }
    }

    fn enqueue<F>(&self, closure: F)
    where
        F: Fn() -> T + Send + 'static,
    {
        let current = *self.current.read().unwrap();
        let (tx, ..) = &self.threads[current];

        tx.send(Box::new(closure)).unwrap();

        let mut write = self.current.write().unwrap();
        *write = (current + 1) % self.size;
    }

    fn next_result(&self) -> T {
        self.queue.recv().unwrap()
    }
}

fn main() {
    let queue = WorkerQueue::<u32>::new(4, 4);

    for _ in 0..10 {
        thread::spawn(|| {
            queue.enqueue(|| 10);
        });
    }

    for _ in 0..10 {
        println!("Result: {}", queue.next_result());
    }
}
