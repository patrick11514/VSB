//! Run this file with `cargo test --test 05_srl_validator`.

// TODO: Implement a SRL (Simple Resource Locator) validator.
// A SRL consists of two parts, an **optional** protocol (String) and an address (String).
// The format of the SRL looks like this: `[<protocol>://]<address>`
// The protocol and the address have to contain only lowercase English characters.
// Protocol must not be empty if :// is present in the SRL.
// Address must not be empty.
//
// As an example, these are valid SRLs:
// - `http://foo`
// - `bar://baz`
// - `foobar`
//
// And these are invalid SRLs:
// - `http://foo1` (invalid character in address)
// - `asd://bar://` (invalid character in address)
// - `://baz` (empty protocol)
// - `01://baz` (invalid character in protocol)
//
// Create a struct `SRL` in a module named `srl`. Expose functions for parsing a SRL and getting
// its individual parts, but do not allow modifying the fields of `SRL` outside its module.
// Do not use regular expressions, SRLs can be easily parsed with a big of parsing logic.
//
// Hint: Put `#[derive(Debug, Eq, PartialEq)]` on top of `SRL` and `SRLValidationError`,
// so that asserts in tests work.

mod srl {
    #[derive(Debug, Eq, PartialEq)]
    pub struct SRL {
        proto: Option<String>,
        address: String,
    }

    #[derive(Debug, Eq, PartialEq)]
    pub enum SRLValidationError {
        EmptyAddress,
        EmptyProtocol,
        InvalidCharacterInAddress(char),
        InvalidCharacterInProtocol(char),
    }

    enum StrCheck {
        Success,
        Empty,
        InvalidChar(char),
    }

    impl SRL {
        fn check_str(str: &str) -> StrCheck {
            if str.is_empty() {
                return StrCheck::Empty;
            }

            let invalid_char = str.chars().find(|c| match c {
                'a'..'z' => false,
                _ => true,
            });

            match invalid_char {
                Some(ch) => StrCheck::InvalidChar(ch),
                _ => StrCheck::Success,
            }
        }

        pub fn new(input: &str) -> Result<Self, SRLValidationError> {
            let parts: Vec<&str> = input.split("://").collect();
            if parts.len() > 2 {
                return Err(SRLValidationError::InvalidCharacterInAddress(':'));
            }

            let result = SRL::check_str(parts[0]);
            let ret = match result {
                StrCheck::Success => Ok(SRL {
                    proto: None,
                    //We treat this as address, even if it can ve proto, we move this string later
                    address: parts[0].into(),
                }),
                //Same with errors, if we discover, that the parts.len() == 2 we transfer the errors
                StrCheck::Empty => Err(SRLValidationError::EmptyAddress),
                StrCheck::InvalidChar(ch) => Err(SRLValidationError::InvalidCharacterInAddress(ch)),
            };

            match ret {
                Ok(srl) => {
                    if parts.len() == 1 {
                        Ok(srl) //This means, that the SRL only includes address
                    } else {
                        let result = SRL::check_str(parts[1]); //here we parse real address

                        match result {
                            StrCheck::Success => Ok(SRL {
                                proto: Some(srl.address), //here we do the move
                                address: parts[1].into(),
                            }),
                            StrCheck::Empty => Err(SRLValidationError::EmptyAddress),
                            StrCheck::InvalidChar(ch) => {
                                Err(SRLValidationError::InvalidCharacterInAddress(ch))
                            }
                        }
                    }
                }
                Err(err) => {
                    if parts.len() == 1 {
                        Err(err) // here the errors are correct
                    } else {
                        Err(match err {
                            SRLValidationError::EmptyAddress => SRLValidationError::EmptyProtocol,
                            SRLValidationError::InvalidCharacterInAddress(ch) => {
                                SRLValidationError::InvalidCharacterInProtocol(ch)
                            }
                            //These never happen, but we need to map them
                            SRLValidationError::EmptyProtocol => SRLValidationError::EmptyProtocol,
                            SRLValidationError::InvalidCharacterInProtocol(ch) => {
                                SRLValidationError::InvalidCharacterInProtocol(ch)
                            }
                        })
                    }
                }
            }
        }

        pub fn get_protocol(&self) -> Option<&str> {
            match &self.proto {
                Some(str) => Some(str),
                None => None,
            }
        }

        pub fn get_address(&self) -> String {
            self.address.clone()
        }
    }
}

/// Below you can find a set of unit tests.
#[cfg(test)]
mod tests {
    use super::srl::{SRL, SRLValidationError};

    #[test]
    fn empty_address() {
        assert_eq!(SRL::new(""), Err(SRLValidationError::EmptyAddress));
    }

    #[test]
    fn only_separator() {
        assert_eq!(SRL::new("://"), Err(SRLValidationError::EmptyProtocol));
    }

    #[test]
    fn empty_protocol() {
        assert_eq!(SRL::new("://foo"), Err(SRLValidationError::EmptyProtocol));
    }

    #[test]
    fn multiple_protocols() {
        assert_eq!(
            SRL::new("ab://bc://foo"),
            Err(SRLValidationError::InvalidCharacterInAddress(':'))
        );
    }

    #[test]
    fn invalid_protocol() {
        assert_eq!(
            SRL::new("bAc://foo"),
            Err(SRLValidationError::InvalidCharacterInProtocol('A'))
        );
        assert_eq!(
            SRL::new("a02://foo"),
            Err(SRLValidationError::InvalidCharacterInProtocol('0'))
        );
    }

    #[test]
    fn invalid_address_with_protocol() {
        assert_eq!(
            SRL::new("abc://fo1o"),
            Err(SRLValidationError::InvalidCharacterInAddress('1'))
        );
        assert_eq!(
            SRL::new("bar://fooBZcX"),
            Err(SRLValidationError::InvalidCharacterInAddress('B'))
        );
    }

    #[test]
    fn invalid_address_without_protocol() {
        assert_eq!(
            SRL::new("fo1o"),
            Err(SRLValidationError::InvalidCharacterInAddress('1'))
        );
        assert_eq!(
            SRL::new("fooBAc"),
            Err(SRLValidationError::InvalidCharacterInAddress('B'))
        );
    }

    #[test]
    fn invalid_protocol_and_address() {
        assert_eq!(
            SRL::new("bZcA://fo2o"),
            Err(SRLValidationError::InvalidCharacterInProtocol('Z'))
        );
        assert_eq!(
            SRL::new("a20://barBAZ"),
            Err(SRLValidationError::InvalidCharacterInProtocol('2'))
        );
    }

    #[test]
    fn invalid_char_emoji() {
        assert_eq!(
            SRL::new("asd://fo🙃o"),
            Err(SRLValidationError::InvalidCharacterInAddress('🙃'))
        );
    }

    #[test]
    fn no_protocol() {
        let srl = SRL::new("foobar").unwrap();
        assert_eq!(srl.get_protocol(), None);
        assert_eq!(srl.get_address(), "foobar");
    }

    #[test]
    fn protocol_and_scheme() {
        let srl = SRL::new("bar://foobar").unwrap();
        assert_eq!(srl.get_protocol(), Some("bar"));
        assert_eq!(srl.get_address(), "foobar");
    }
}
