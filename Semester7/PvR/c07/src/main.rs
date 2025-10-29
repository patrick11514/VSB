use std::{
    fs, io,
    ops::Add,
    path::{Path, PathBuf},
    sync::{Arc, mpsc},
    thread,
};

/// TODO: your task is to implement a parallel version of grep
///
/// The program will search for a given string pattern in a directory and all subdirectories.
/// It will crawl all files, read them and find all lines that contain the given pattern.
/// You do not have to use a regular expression, a simple substring search is enough.
///
/// 1) Download a copy of Linux to have a directory with a lot of files to search in
/// ```bash
/// $ git clone https://github.com/kobzol/linux --branch pvr --depth 1
/// ```
///
/// 2) Implement a function that will recursively crawl through a given directory and find all files
/// in it. Use the [read_dir](https://doc.rust-lang.org/std/fs/fn.read_dir.html) function to list
/// files and directories contained in a directory.
///
/// To test that the function works correctly, you can simply increment an integer for each found
/// file and then print its final value. In the Linux directory you should find `98624` files.
///
/// 3) Create N threads that will process the files found by the crawling function in parallel.
/// The threads should receive information about the found files via a channel, and then process
/// each file. The threads and the crawling function should proceed in parallel, as even the
/// crawling itself can take ~0.5s on the Linux directory.
///
/// Once a thread receives a file path to scan, it should open a file at that path, read its lines
/// and find all lines that contain the input search pattern.
/// Note: You can ignore I/O failures and failures caused by reading non-UTF8 files.
///
/// Each thread should periodically report the number of files read so far, along with all found
/// matches, to the main thread. Think about what approach can be used to exchange that information.
///
/// 4) Continuously read the number of scanned files and the number of found matches in the main
/// thread, and periodically print it with a simple progress bar.
///
/// 5) Once all files are scanned, wait for all threads to finish and join them.
///
/// 6) Print the duration it took to scan everything, the number of scanned files and the number of
/// found matches.
///
/// When searching for the pattern "rust" in the Linux directory, it should find `98624` files and
/// `6156` matched lines.
///
/// 7) Sort the matches by file path and line number, and output them into a file on disk (one match
/// per line) with the format `<file path>:<line number>:<line>`.
///
/// 8) Try different numbers of threads to see if you get a speedup. Don't forget to run your code
/// in release mode :)
/// Try to optimize the program: reduce allocations when searching through the file, amortize
/// channel/atomic operations, make sure that you do not hold a lock for too long (if you use a
/// lock).
///
/// 9) Add a CLI interface using `clap` and also output the files as JSON with `serde` and
/// `serde_json`.

fn scan_files(path: &PathBuf, thread_count: usize) -> anyhow::Result<()> {
    let mut to_scan: Vec<PathBuf> = vec![path.into()];
    let mut channels = Vec::with_capacity(thread_count);
    let mut threads: Vec<thread::JoinHandle<anyhow::Result<()>>> = Vec::with_capacity(thread_count);

    let mut current = 0;

    //Init channels
    for _ in 0..thread_count {
        let (tx, rx) = mpsc::sync_channel::<PathBuf>(10);
        channels.push(tx);
        threads.push(thread::spawn(move || {
            while let Ok(value) = rx.recv() {
                let file = fs::read_to_string(value)?;
            }

            Ok(())
        }))
    }

    while let Some(path) = to_scan.pop() {
        if !path.is_dir() {
            channels[current].send(path)?;
            current += 1;
            if current >= thread_count {
                current = 0;
            }
        } else {
            for file in path.read_dir()? {
                match file {
                    Ok(file) => {
                        to_scan.push(file.path());
                    }
                    _ => {}
                }
            }
        }
    }

    for t in threads {
        t.join().unwrap().unwrap();
    }

    Ok(())
}

fn main() {
    scan_files(&"/tmp/linux".into(), 1).unwrap();
}
