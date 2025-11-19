//! TODO: implement a TCP/IP client that will connect to a server and figure out a password to
//! open a VAULT.
//!
//! If you can guess the password to the vault by the end of the seminar, you'll be awarded 2 points
//! for Gryffindor.
//!
//! # Communication protocol
//! After you connect to the server on a given TCP/IP v4 address and port, the following should
//! happen:
//! 1) You send a string that identifies you. You have to choose a nickname.
//!
//! - You have to send the message within two seconds. If you don't, the server will disconnect you.
//! - If the nickname is not unique (someone else has the same nickname), you will be disconnected.
//! - The nickname cannot be longer than `15` (UTF-8) bytes.
//!
//! 2) The following communication happens in lockstep. You send a string that contains your guess
//! of the password. The server then responds either with:
//! - "correct" if you have guessed the password correctly
//! - "incorrect" if your password guess was wrong
//! - a string containing an error if some error has happened
//!
//! # Spam protection
//! - You must not send a message more often than once every 0.1 milliseconds. If
//!   you do, you will receive a strike. After accumulating three strikes, you will be disconnected.
//! - You must not make more than 10 thousand password guesses. After 10k attempts, you will be
//!   disconnected.
//!
//! # Inactivity protection
//! You have to send a message at least once every five seconds, otherwise you will be disconnected.
//!
//! # Message encoding
//! The encoding is similar to last week, although this time, each message is a simple UTF-8 string,
//! there is no JSON involved. You can use the provided `MessageReader` and `MessageWriter` structs
//! to communicate with the server.
//!
//! Note: you can use blocking I/O. Do not bother with non-blocking I/O, it would take too long.
//!
//! Bonus point if you can crash the server :)

use std::{net::TcpStream, time::Instant};

use crate::{reader::MessageReader, writer::MessageWriter};

mod reader;
mod writer;

fn try_password(
    writer: &mut MessageWriter<TcpStream>,
    reader: &mut MessageReader<TcpStream>,
    pass: String,
) -> Option<u128> {
    let mut sum = 0;

    let times = 2;

    for _ in 0..times {
        writer.write(&pass).unwrap();

        let begin = Instant::now();
        let msg = reader.read().unwrap().unwrap();

        let duration = begin.elapsed();
        sum += duration.as_millis();

        if msg != "incorrect" {
            return None;
        }
    }

    Some(sum)
}

fn main() {
    let mut password = "wtfs".to_string();

    let socket = TcpStream::connect(("158.196.174.201", 5555)).unwrap();
    let mut writer = MessageWriter::new(socket.try_clone().unwrap());
    let mut reader = MessageReader::new(socket);

    writer.write("min0150").unwrap();

    loop {
        let mut longest = (' ', 0);

        for ch in 'a'..='z' {
            let mut copy = password.clone();
            copy.push(ch);

            println!("Testing {copy}");

            match try_password(&mut writer, &mut reader, copy) {
                Some(took) => {
                    if took > longest.1 {
                        longest = (ch, took);
                    }
                    println!("took {took}");
                }
                None => {
                    println!("guessed");
                }
            }
        }

        password.push(longest.0);

        println!("Password progress: {password}");
    }
}
