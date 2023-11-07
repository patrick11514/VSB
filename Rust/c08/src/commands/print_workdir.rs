struct PrintWorkDir;

use crate::

impl Command for PrintWorkDir {
    fn execute(&self, state: &mut InterpreterState) {
        println!("{}", state.workdir.display());
    }
}
