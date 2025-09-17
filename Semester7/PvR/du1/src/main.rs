//! You can use this file for experiments.
//! Exercises with unit tests can be found in the `tests` directory.

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
            if mul > 10 {
                mul - 9
            } else {
                mul
            }
        } else {
            digit
        };

        i += 1;
    }

    println!("check: {}", check_num);
    println!("sum: {}", sum);
    println!("calc: {}", (10 - (sum % 10) % 10));

    (10 - (sum % 10) % 10) == check_num
}

fn main() {
    println!("Hello Rust!");

    println!("{}", luhn_algorithm(123456));
}
