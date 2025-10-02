//! Run this file with `cargo test --test 02_witness`.

//! Imagine that you're implementing a website that has various endpoints.
//! When a user visits an endpoint, you have to check their permissions.
//! Below, there is an endpoint `endpoint_admin_dashboard` that should only be accessible to admins.
//! It checks if the user is admin, and if they are not, the access is denied.
//!
//! However, there can be hundreds of such endpoints.
//! What if the programmer forgets to use `is_admin` in one of them and calls
//! `show_admin_dashboard` by accident even for non-admin users?
//!
//! Try to modify the code in a way that it will be **impossible** to forget checking if the user
//! is an admin before calling `show_admin_dashboard`.
//! Try to encode invariants using the type system to achieve that.

use crate::admin::{AdminUser, is_admin};

struct User {
    id: u32,
}

mod admin {
    use crate::User;

    pub struct AdminUser {
        user: User,
    }

    impl Into<User> for AdminUser {
        fn into(self) -> User {
            self.user
        }
    }

    pub fn is_admin(user: User) -> Result<AdminUser, User> {
        if user.id == 0 {
            return Ok(AdminUser { user });
        }
        Err(user)
    }
}

// How to make sure that this function can only be called for admin users?
fn render_admin_dashboard(user: AdminUser) -> String {
    // Do not modify the body of this function below this line
    // Assume that this function e.g. does not access the DB anymore, and it thus can't check if the
    // user is admin.
    let user: User = user.into();
    "Secret admin content".to_string()
}

fn endpoint_admin_dashboard(user: User) -> String {
    // What if the user forgets this check?
    // Can we make the code more robust, so that they cannot forget?
    // Can ownership + encapsulation help us somehow?
    if let Ok(admin_user) = is_admin(user) {
        // Deny access
        return render_admin_dashboard(admin_user);
    }

    return String::from("Error 400");
}

fn main() {}
