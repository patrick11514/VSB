//! TODO: implement a TCP/IP client that will connect to a server with a canvas
//! and draw pixels to it.
//!
//! If you can draw your login to the canvas (in a way that I will be able to read it) by the end of
//! the seminar, you'll be awarded 2 points for Gryffindor.
//!
//! The first person to do it without AI wins a free ticket to the TechMeetup conference
//! (https://www.techmeetup.cz) happening in Ostrava on Friday.
//!
//! # Communication protocol
//! After you connect to the server on a given TCP/IP v4 address and port, the following should
//! happen:
//! 1) You send a "Join" message that identifies you. You have to choose a nickname.
//! The format of the message is as follows:
//! `{"Join": {"name": "<nickname>"}}`
//!
//! - You have to send the message within two seconds. If you don't, you will be disconnected.
//! - If the nickname is not unique (someone else has the same nickname), you will be disconnected.
//! - The nickname cannot be longer than `15` (UTF-8) bytes.
//! - You will be assigned a random color by the server.
//!
//! 2) The server will respond with a welcome message that contains the dimensions of the canvas
//! and a secret token that you have to use to draw to the canvas. The format of the welcome message
//! is as follows:
//! `{"Welcome": {"token": "<token>", "width": <width>, "height": <height>}}`
//!
//! 3) After you read the welcome message, you can start drawing to the canvas by sending the
//! following message:
//! `{"Draw": {"row": <row>, "col": <col>, "token": "<token>"}}`
//!
//! - If you try to draw outside of the bounds of the canvas, you will be disconnected.
//!
//! In addition to drawing, you can also send the following message to the server to get the canvas
//! state: `"GetState"`
//! The server will respond with a hashmap that contains the pixel positions sent by connected
//! users. Positions are represented with an array `[row, col]`:
//! ```
//! {"CanvasState": {"user-a": [[9,47],[10,2]], "user-b": [[0, 1], [5, 8]]}}
//! ```
//!
//! # Spam protection
//! You must not send a Draw or GetState message more often than once every 500 milliseconds. If
//! you do, you will receive a strike. After accumulating three strikes, you will be disconnected.
//!
//! # Inactivity protection
//! You have to send a Draw or GetState command at least once every five seconds, otherwise you
//! will be disconnected.
//!
//! # Message encoding
//! Messages between the server and the client are exchanged in JSON.
//! Each message is prefixed with a 4 byte little-endian number that specifies the amount of bytes
//! of the serialized JSON payload.
//! The maximum length of the payload is 256, larger messages will not be accepted by the server.
//!
//! # Notes
//! - If you do anything wrong, the server will send you an error that you can read to figure out
//! what's wrong. The format of the error message is `{"Error": "<error>"}`.
//! - Use `#[derive(serde::Serialize, serde::Deserialize)]` to build the protocol messages, don't
//! build the JSON messages by hand from strings.
//! - Any time you disconnect from the server, your pixels will be removed from the canvas.
//!
//! Bonus point if you can crash the server :)
//! 158.196.173.103

use week08_exercises::client::Client;
fn main() -> anyhow::Result<()> {
    let client = Client::new("5.tcp.eu.ngrok.io", 16778);
    let mut client = client.connect("min0150".to_string())?;

    loop {
        client.draw_array(
            50,
            5,
            r#"
#   #       #   # 
#   #       #   # 
#   # #   # #   #
#   # # # # #   # 
 ###   # #   ###

###  #   #  ##  #####   #  ##    ###  ###   ###  #
#  # #   # #      #     # #     #    #   # #   # #
###  #   #  ##    #     #  ##   #    #   # #   # #
# #  #   #    #   #     #    #  #    #   # #   # #
#  #  ###   ##    #     #  ##    ###  ###   ###  ####
    "#
            .into(),
        )?;
    }
}
