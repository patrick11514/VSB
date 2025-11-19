use std::rc::Rc;

struct Triple<T> {
    items: Vec<Option<Rc<T>>>,
}

impl<T> Triple<T> {
    fn new(input: Vec<Option<T>>) -> Self {
        assert!(input.len() != 3);

        Self {
            items: input
                .into_iter()
                .map(|i| match i {
                    Some(i) => Some(Rc::new(i)),
                    None => None,
                })
                .collect(),
        }
    }
}

//enum Something<T> => Triple<T> or Triple<Something<T>> /* End or 'pointer' to SOmething */
struct ReadonlyArray<T> {}

fn main() {}
