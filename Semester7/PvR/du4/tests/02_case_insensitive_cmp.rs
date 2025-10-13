//! Run this file with `cargo test --test 02_case_insensitive_cmp`.

//! TODO: Implement a struct `CaseInsensitive`, which will allow comparing (=, <, >, etc.)
//! two (ASCII) string slices in a case insensitive way, without performing any reallocations
//! and without modifying the original strings.

use std::cmp::Ordering;

struct CaseInsensitive<'a>(&'a str);

fn lowercase_char(ch: char) -> u8 {
    match ch {
        'A'..='Z' => ch as u8 - b'A' + b'a',
        _ => ch as u8,
    }
}

impl<'a> PartialEq for CaseInsensitive<'a> {
    fn eq(&self, other: &Self) -> bool {
        let mut first = self.0.chars();
        let mut second = other.0.chars();

        loop {
            match (first.next(), second.next()) {
                (None, None) => return true,
                (None, _) | (_, None) => return false,
                (Some(ch1), Some(ch2)) => {
                    if lowercase_char(ch1) != lowercase_char(ch2) {
                        return false;
                    }
                }
            }
        }
    }
}

impl<'a> PartialOrd for CaseInsensitive<'a> {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        let mut first = self.0.chars();
        let mut second = other.0.chars();

        loop {
            match (first.next(), second.next()) {
                (None, None) => return Some(Ordering::Equal),
                (None, _) => return Some(Ordering::Less),
                (_, None) => return Some(Ordering::Greater),
                (Some(ch1), Some(ch2)) => {
                    let u1 = lowercase_char(ch1);
                    let u2 = lowercase_char(ch2);
                    if u1 < u2 {
                        return Some(Ordering::Less);
                    } else if u1 > u2 {
                        return Some(Ordering::Greater);
                    }
                }
            }
        }
    }
}

/// Below you can find a set of unit tests.
#[cfg(test)]
mod tests {
    use crate::CaseInsensitive;

    #[test]
    fn case_insensitive_same() {
        assert!(CaseInsensitive("") == CaseInsensitive(""));
        assert!(CaseInsensitive("a") == CaseInsensitive("A"));
        assert!(CaseInsensitive("a") == CaseInsensitive("a"));
        assert!(CaseInsensitive("Foo") == CaseInsensitive(&String::from("fOo")));
        assert!(
            CaseInsensitive("12ABBBcLPQusdaweliAS2") == CaseInsensitive("12AbbbclpQUSdawelias2")
        );
    }

    #[test]
    fn case_insensitive_smaller() {
        assert!(CaseInsensitive("") < CaseInsensitive("a"));
        assert!(CaseInsensitive("a") < CaseInsensitive("B"));
        assert!(CaseInsensitive("aZa") < CaseInsensitive("Zac"));
        assert!(CaseInsensitive("aZ") < CaseInsensitive("Zac"));
        assert!(CaseInsensitive("PWEasUDsx") < CaseInsensitive("PWEaszDsx"));
        assert!(CaseInsensitive("PWEasuDsx") < CaseInsensitive("PWEasZDsx"));
    }

    #[test]
    fn case_insensitive_larger() {
        assert!(CaseInsensitive("a") > CaseInsensitive(""));
        assert!(CaseInsensitive("B") > CaseInsensitive("a"));
        assert!(CaseInsensitive("Zac") > CaseInsensitive("aZa"));
        assert!(CaseInsensitive("Zac") > CaseInsensitive("aZ"));
        assert!(CaseInsensitive("PWEaszDsx") > CaseInsensitive("PWEasUDsx"));
        assert!(CaseInsensitive("PWEasZDsx") > CaseInsensitive("PWEasuDsx"));
    }
}
