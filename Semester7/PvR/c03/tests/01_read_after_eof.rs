//! Run this file with `cargo test --test 01_read_after_eof`.

// Do not change this module
mod os {
    // Simulate a single byte of data being read from the OS for the passed file descriptor.
    // When this function returns `0`, it marks end-of-file.
    pub fn read(fd: u32) -> u8 {
        // The implementation doesn't matter, only the function signatures are
        // important in this exercise.
        0
    }
}

// Library code
struct OpenedFile {
    // File descriptor
    fd: u32,
}

// TODO: modify the `read` function in a way that `OpenedFile` cannot be used anymore after it reaches
// end-of-file while reading.

// Implement this function in a way that when the file reaches end-of-file (there is nothing
// else to read), it will not be possible to use it anymore (such usage should result in a
// compile-time error).
fn read(file: OpenedFile) -> Option<(OpenedFile, u8)> {
    let byte = os::read(file.fd);
    if byte == 0 {
        return None;
    }
    Some((file, byte))
}
// End of library code

// User code
fn main() {
    let mut file = OpenedFile { fd: 1 };
    let mut byte = 0;

    loop {
        let ret = read(file);
        match ret {
            None => break,
            Some(tuple) => {
                (file, byte) = tuple;
                println!("{}", byte);
            }
        }
    }
}
