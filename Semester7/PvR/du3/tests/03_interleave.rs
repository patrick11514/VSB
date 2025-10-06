//! Run this file with `cargo test --test 03_interleave`.

// TODO: Implement a function called `interleave`, which will take two string slices and return
// a string that contains the characters from the two input strings interleaved.
// The first character of the output should start with the first character of the first argument.
// See tests for details.
//
// Can you write the function without any explicit indexing (`str[index]`)?
//
// Hint: you can use `string.chars()` to create an iterator over the Unicode characters of a string.

fn interleave(first: &str, second: &str) -> String {
    let mut result = String::new();
    let mut first = first.chars();
    let mut first_end = false;
    let mut second = second.chars();
    let mut second_end = false;

    fn push_char(it: &mut std::str::Chars, result: &mut String, end: &mut bool) {
        let ch = it.next();
        if let Some(ch) = ch {
            result.push(ch);
        } else {
            *end = true;
        }
    }

    loop {
        if !first_end {
            push_char(&mut first, &mut result, &mut first_end);
        }

        if !second_end {
            push_char(&mut second, &mut result, &mut second_end);
        }

        if first_end && second_end {
            break;
        }
    }

    result
}

/// Below you can find a set of unit tests.
#[cfg(test)]
mod tests {
    use crate::interleave;

    #[test]
    fn interleave_empty() {
        assert_eq!(interleave("", ""), "");
    }

    #[test]
    fn interleave_only_left() {
        assert_eq!(interleave("a", ""), "a");
        assert_eq!(interleave("zxjas", ""), "zxjas");
    }

    #[test]
    fn interleave_only_right() {
        assert_eq!(interleave("", "z"), "z");
        assert_eq!(interleave("", "foobar"), "foobar");
    }

    #[test]
    fn interleave_same_length() {
        assert_eq!(interleave("abcdef", "012345"), "a0b1c2d3e4f5");
    }

    #[test]
    fn interleave_first_longer() {
        assert_eq!(
            interleave("Programming Rust", "O'Reilly"),
            "POr'oRgerialmlmying Rust"
        );
    }

    #[test]
    fn interleave_second_longer() {
        assert_eq!(
            interleave("ahoj, jak se máš?", "díky za optání, mám se dobře"),
            "adhíokjy,  zjaa ko psteá nmíá,š ?mám se dobře"
        );
    }
}
