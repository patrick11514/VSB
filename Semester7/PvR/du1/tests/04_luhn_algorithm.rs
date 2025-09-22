//! Run this file with `cargo test --test 04_luhn_algorithm`.

// TODO: Implement the Luhn algorithm (https://en.wikipedia.org/wiki/Luhn_algorithm),
// which is used to check the validity of e.g. bank or credit card numbers.
// Note: If the number does not have at least two digits, the function should return `false`.
fn luhn_algorithm(num: u128) -> bool {
    if num < 10 {
        return false;
    }

    let check_num = num % 10;
    let mut num = num / 10;

    let mut sum = 0;
    let mut i = 0;

    while num > 0 {
        let digit = num % 10;
        num /= 10;

        sum += if i % 2 == 0 {
            let mul = digit * 2;
            if mul >= 10 {
                mul - 9
            } else {
                mul
            }
        } else {
            digit
        };

        i += 1;
    }

    ((10 - (sum % 10)) % 10) == check_num
}

/// Below you can find a set of unit tests.
#[cfg(test)]
mod tests {
    use super::luhn_algorithm;

    #[test]
    fn luhn_zero() {
        assert!(!luhn_algorithm(0));
    }

    #[test]
    fn luhn_smaller_than_ten() {
        assert!(!luhn_algorithm(5));
    }

    #[test]
    fn luhn_small_correct() {
        assert!(luhn_algorithm(18));
    }

    #[test]
    fn luhn_small_incorrect() {
        assert!(!luhn_algorithm(10));
    }

    #[test]
    fn luhn_correct() {
        assert!(luhn_algorithm(17893729974));
        assert!(luhn_algorithm(79927398713));
    }

    #[test]
    fn luhn_incorrect() {
        assert!(!luhn_algorithm(17893729975));
        assert!(!luhn_algorithm(17893729976));
        assert!(!luhn_algorithm(17893729977));
        assert!(!luhn_algorithm(123456));
    }
}
