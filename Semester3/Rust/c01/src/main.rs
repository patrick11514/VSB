fn main() {
    let a = 1;
    let mut b = 5;
    println!("b je: {}", b);
    b = b * 2;

    let out = add(a, b);

    println!("out je: {}", out);
}

fn add(a: i8, b: i8) -> i8 {
    a + b
}

#[test]
fn test_add() {
    assert!(add(1, 2) == 3);
}
