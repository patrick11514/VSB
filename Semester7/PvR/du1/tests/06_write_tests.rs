//! Run this file with `cargo test --test 06_write_tests`.

/// This function implements a string sanitization logic that should uphold the following
/// properties:
/// - After sanitization, the result must not end with the character `x`
/// - After sanitization, the result must not end with the character `o`
/// - After sanitization, the result must not end with the string `.exe`
///
/// The function assumes that the input to the function only consists of lower and uppercase
/// characters from the English alphabet and digits 0-9.
///
/// The implementation contains some bugs.
///
/// Your task is to write a set (at least 8) of unit tests, use them to find (at least 2) bugs in
/// this function and then fix the function.
fn sanitize(input: &str) -> &str {
    let mut input = input;

    loop {
        if input.ends_with("x") || input.ends_with("o") {
            input = input.trim_end_matches(['x', 'o']);
        } else if input.ends_with(".exe") {
            input = &input[0..input.len() - 4];
        } else {
            return input;
        }
    }
}

/// TODO: write tests for the `sanitize` function
///
/// Bonus: can you find any bugs using the [proptest](https://proptest-rs.github.io/proptest/intro.html)
/// crate?
/// Note that you will probably need to run `cargo test` with the `PROPTEST_DISABLE_FAILURE_PERSISTENCE=1`
/// environment variable to make proptest work for tests stored in the `tests` directory.
#[cfg(test)]
mod tests {
    use super::sanitize;

    #[test]
    fn empty() {
        assert_eq!(sanitize(""), "");
    }

    #[test]
    fn already_sanitized() {
        assert_eq!(sanitize("normal text"), "normal text");
        assert_eq!(sanitize("javascript"), "javascript");
        assert_eq!(sanitize("hello world"), "hello world");
    }

    #[test]
    fn strip_x() {
        assert_eq!(sanitize("carxx"), "car");
    }

    #[test]
    fn strip_o() {
        assert_eq!(sanitize("hellooo"), "hell");
    }

    #[test]
    fn ox_combination() {
        assert_eq!(sanitize("carxo"), "car");
        assert_eq!(sanitize("carox"), "car");
    }

    /// comment says: "lowercase & uppercase english alphabet + numbers"
    /// so idk if the input 'output.exe' is valid, but the function removes
    /// it, so I created test for it
    #[test]
    fn remove_exe() {
        assert_eq!(sanitize("output.exe"), "output");
        assert_eq!(sanitize("output.exe.exe"), "output");
    }

    #[test]
    fn chain_rules() {
        assert_eq!(sanitize("output.exeo"), "output");
        assert_eq!(sanitize("output.exex"), "output");
        assert_eq!(sanitize("outputo.exe"), "output");
        assert_eq!(sanitize("outputx.exe"), "output");
    }

    #[test]
    fn to_empty() {
        assert_eq!(sanitize("xx"), "");
        assert_eq!(sanitize("oo"), "");
        assert_eq!(sanitize(".exe"), "");
    }
}
