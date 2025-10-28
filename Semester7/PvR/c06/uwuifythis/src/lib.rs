use proc_macro::TokenStream;
use quote::{quote_spanned, ToTokens};
use syn::spanned::Spanned;
use syn::visit_mut::VisitMut;
use syn::{parse_macro_input, parse_quote, Data, DeriveInput, LitStr};

/// TODO: implement the following procedural `#[derive(FieldCounter)]` macro
/// It should be usable on a structure. When used on enums (or unions), it should produce a compile
/// error.
/// It should add an associated function called `field_count`, which returns the number of fields
/// in the struct.
/// The visibility of the function should be the same as of the struct.
#[proc_macro_derive(FieldCounter)]
pub fn derive_field_counter(stream: TokenStream) -> TokenStream {
    // Parse the input token stream as an ADT (struct/enum/union) using the `syn` crate
    let input = parse_macro_input!(stream as DeriveInput);
    // Generate some tokens that will be appended after the struct
    let output = quote! {};
    output.into()
}

/// TODO: implement the following attribute procedural macro
/// It should go through all string literals in the given function, and uwuify them using
/// the [`uwuifier`](https://crates.io/crates/uwuify) crate.
///
/// Use the [`VisitMut`](https://docs.rs/syn/latest/syn/visit_mut/index.html) API from `syn`.
/// The [`parse_quote!`](https://docs.rs/syn/latest/syn/macro.parse_quote.html) macro might also be
/// useful.
#[proc_macro_attribute]
pub fn uwuifythis(_attr: TokenStream, item: TokenStream) -> TokenStream {
    let mut func: syn::ItemFn = syn::parse2(item.into()).unwrap();
    func.into_token_stream().into()
}
