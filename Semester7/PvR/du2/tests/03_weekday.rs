//! Run this file with `cargo test --test 03_weekday`.

// TODO: Implement methods for an enum that represents the day of the week.
// Implement the following methods for it:
// - `next` returns the next day of the week.
// - `prev` returns the previous day of the week.
// - `n_days_in_future(n)` returns the day of the week `n` days in the future.
//   - Note: the signature of the function should be `fn n_days_in_future(self, n: u64) -> WeekDay`
//   - Note: try to avoid unnecessary work in this function :)

#[derive(PartialEq, Debug)]
enum WeekDay {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
}

impl WeekDay {
    fn next(self) -> Self {
        match self {
            WeekDay::Monday => WeekDay::Tuesday,
            WeekDay::Tuesday => WeekDay::Wednesday,
            WeekDay::Wednesday => WeekDay::Thursday,
            WeekDay::Thursday => WeekDay::Friday,
            WeekDay::Friday => WeekDay::Saturday,
            WeekDay::Saturday => WeekDay::Sunday,
            WeekDay::Sunday => WeekDay::Monday,
        }
    }

    fn prev(self) -> Self {
        match self {
            WeekDay::Monday => WeekDay::Sunday,
            WeekDay::Tuesday => WeekDay::Monday,
            WeekDay::Wednesday => WeekDay::Tuesday,
            WeekDay::Thursday => WeekDay::Wednesday,
            WeekDay::Friday => WeekDay::Thursday,
            WeekDay::Saturday => WeekDay::Friday,
            WeekDay::Sunday => WeekDay::Saturday,
        }
    }

    fn n_days_in_future(self, n: u64) -> Self {
        let mut day = self;

        for _ in 0..(n % 7) {
            day = day.next();
        }

        day
    }
}

/// Below you can find a set of unit tests.
#[cfg(test)]
mod tests {
    use crate::WeekDay;

    #[test]
    pub fn test_next() {
        assert_eq!(WeekDay::Monday.next(), WeekDay::Tuesday);
        assert_eq!(WeekDay::Tuesday.next(), WeekDay::Wednesday);
        assert_eq!(WeekDay::Wednesday.next(), WeekDay::Thursday);
        assert_eq!(WeekDay::Thursday.next(), WeekDay::Friday);
        assert_eq!(WeekDay::Friday.next(), WeekDay::Saturday);
        assert_eq!(WeekDay::Saturday.next(), WeekDay::Sunday);
        assert_eq!(WeekDay::Sunday.next(), WeekDay::Monday);
    }

    #[test]
    pub fn test_prev() {
        assert_eq!(WeekDay::Monday.prev(), WeekDay::Sunday);
        assert_eq!(WeekDay::Tuesday.prev(), WeekDay::Monday);
        assert_eq!(WeekDay::Wednesday.prev(), WeekDay::Tuesday);
        assert_eq!(WeekDay::Thursday.prev(), WeekDay::Wednesday);
        assert_eq!(WeekDay::Friday.prev(), WeekDay::Thursday);
        assert_eq!(WeekDay::Saturday.prev(), WeekDay::Friday);
        assert_eq!(WeekDay::Sunday.prev(), WeekDay::Saturday);
    }

    #[test]
    pub fn test_n_days() {
        assert_eq!(WeekDay::Monday.n_days_in_future(1), WeekDay::Tuesday);
        assert_eq!(WeekDay::Monday.n_days_in_future(2), WeekDay::Wednesday);
        assert_eq!(WeekDay::Monday.n_days_in_future(7), WeekDay::Monday);
        assert_eq!(WeekDay::Wednesday.n_days_in_future(10), WeekDay::Saturday);
    }

    #[test]
    pub fn test_n_days_large() {
        // I wonder what year it is...
        assert_eq!(
            WeekDay::Monday.n_days_in_future(10000000000000000000),
            WeekDay::Thursday
        );
    }
}
