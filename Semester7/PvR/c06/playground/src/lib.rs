#[cfg(test)]
mod tests {
    use uwuifythis::FieldCounter;

    #[test]
    fn field_count() {
        #[derive(FieldCounter)]
        struct NoFields;
        assert_eq!(NoFields::field_count(), 0);

        #[derive(FieldCounter)]
        struct OneField(bool);
        assert_eq!(OneField::field_count(), 1);

        #[derive(FieldCounter)]
        struct TwoFields {
            a: u32,
            b: u32,
        }
        assert_eq!(TwoFields::field_count(), 2);
    }
}
