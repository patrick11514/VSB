#[derive(serde::Serialize, serde::Deserialize, Debug)]
pub enum ClientToServerMsg {
    /// This is the first message in the communication, which should be sent by the client.
    /// When some other client with the same name already exists, the server should respond
    /// with an error "Username already taken" and disconnect the new client.
    Join { name: String },
    /// This message lets the server know that you're still alive.
    /// You have to send it at least once every five seconds, but you must not
    /// send it more often than once per second.
    Heartbeat,
    /// Perform an action on the server represented by the given number.
    /// The meaning of actions is determined by the server, it will periodically send you
    /// the [`ServerToClientMsg::ActionMappingUpdate`] message.
    PerformAction(u8),
}

#[derive(serde::Serialize, serde::Deserialize, Debug, Eq, PartialEq, Hash, Copy, Clone)]
pub enum Action {
    /// Turn left
    TurnLeft,
    /// Turn right
    TurnRight,
    /// Move forward
    MoveForward,
    /// Move backward
    MoveBackward,
    /// Enable shield.
    /// While the shield hold, you are invincible.
    /// However, you will also be unable to shoot.
    ///
    /// The shield lasts for a short duration of time, and some amount of time has to pass
    /// before you can use it again.
    Shield,
    /// Shoot a message in the direction of your blob.
    /// Shooting has a short cooldown.
    Fire,
    /// If you send this action to the server, it will freeze the blob and increase the damage it
    /// receives.
    Invalid,
}

#[derive(serde::Serialize, serde::Deserialize, Debug)]
pub enum ServerToClientMsg {
    /// Updates the mapping of actions to numbers.
    /// The index of the action in the array determines which number should be used to invoke
    /// that action.
    ActionMappingUpdate([Action; 10]),
    /// This message is returned by the server when an error occurs.
    Error(String),
}
