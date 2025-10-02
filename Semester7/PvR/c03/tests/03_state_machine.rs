//! Run this file with `cargo test --test 03_state_machine`.

// TODO: Implement an HTTP request builder using a state machine.
// It should allow configuring HTTP method (default is GET) and URL (URL is required, there is no
// default).
// User of the API has to provide exactly one authentication mechanism, either
// HTTP AUTH (username + password) or a token.
// It must not be possible to provide both!
//
// When a token is provided, it can be then optionally encrypted.
//
// Once authentication is performed, the final request can be built.
// Once that is done, the builder must not be usable anymore.

enum HttpMethod {
    Get,
    Post,
}

struct RequestBuilder {
    url: String,
    method: HttpMethod,
}

struct RequestBuilderToken {
    rb: RequestBuilder,
    token: String,
}

struct RequestBuilderBasic {
    rb: RequestBuilder,
    username: String,
    password: String,
}

enum RequestWithAuth {
    Basic(RequestBuilderBasic),
    Token(RequestBuilderToken),
}

impl RequestBuilder {
    fn new(url: &str) -> RequestBuilder {
        RequestBuilder {
            url: url.to_string(),
            method: HttpMethod::Get,
        }
    }

    fn with_method(self, method: HttpMethod) -> Self {
        RequestBuilder {
            url: self.url,
            method,
        }
    }

    fn with_token(self, token: &str) -> RequestWithAuth {
        RequestWithAuth::Token(RequestBuilderToken {
            rb: self,
            token: token.to_string(),
        })
    }

    fn with_http_auth(self, username: &str, password: &str) -> RequestWithAuth {
        RequestWithAuth::Basic(RequestBuilderBasic {
            rb: self,
            username: username.to_string(),
            password: password.to_string(),
        })
    }
}

impl RequestWithAuth {
    fn build(self, body: &str) -> String {
        match self {
            RequestWithAuth::Basic(request_builder_basic) => {
                format!(
                    "{} {}\nauth=http-auth;{}:{}",
                    request_builder_basic.rb.method
                )
            }
            RequestWithAuth::Token(request_builder_token) => {}
        }
    }
}

/// Below you can find a set of unit tests.
#[cfg(test)]
mod tests {
    use crate::{HttpMethod, RequestBuilder};

    #[test]
    fn build_token() {
        assert_eq!(
            RequestBuilder::new("foo")
                .with_token("secret-token")
                .build("body1"),
            r#"GET foo
auth=token;secret-token
body1"#
        );
    }

    #[test]
    fn build_http_auth() {
        assert_eq!(
            RequestBuilder::new("foo")
                .with_http_auth("user", "password")
                .build("body1"),
            r#"GET foo
auth=http-auth;user:password
body1"#
        );
    }

    #[test]
    fn build_method() {
        assert_eq!(
            RequestBuilder::new("foo")
                .with_method(HttpMethod::Post)
                .with_method(HttpMethod::Get)
                .with_method(HttpMethod::Post)
                .with_token("secret-token")
                .build("body1"),
            r#"POST foo
auth=token;secret-token
body1"#
        );
    }

    // This must not compile
    // #[test]
    // fn fail_compilation_multiple_authentication_methods() {
    //     RequestBuilder::new("foo")
    //         .with_http_auth("user", "password")
    //         .with_token("token")
    //         .build("body1");
    // }

    // This must not compile
    // #[test]
    // fn fail_compilation_missing_auth() {
    //     RequestBuilder::new("foo").build("body1");
    // }
}
