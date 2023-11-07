mod command;
mod state;

use command::Command;
use state::InterpreterState;

pub struct Shell {
    command: Vec<Box<dyn Command>>,
    state: InterpreterState,
}
