use crate::state::InterpreterState;

pub trait Command {
    fn execute(&self, state: &mut InterpreterState) {}
}
