
use proc_macro::TokenStream;
use quote::quote;
use syn::{Data, DeriveInput, parse_macro_input};

/// TODO: implement the following procedural `#[derive(DisplayMe)]` macro
/// It should be usable only on structs. When used on enums (or unions), it should produce a compile
/// error.
///
/// The macro should generate code that will implement the `Display` trait for the struct. The
/// specific format of the display implementation is defined by tests in the `assignments` crate.
#[proc_macro_derive(DisplayMe)]
pub fn derive_display_me(stream: TokenStream) -> TokenStream {
    // Parse the input token stream as an ADT (struct/enum/union) using the `syn` crate
    let input = parse_macro_input!(stream as DeriveInput);

    match input.data {
        Data::Union(_) | Data::Enum(_) => {
            panic!("DisplayMe can only be derived for structs");
        }
        _ => {}
    }

    // Generate some tokens that will be appended after the struct
    let output = quote! {};
    output.into()
}
