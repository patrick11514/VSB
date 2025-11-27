use std::{fmt::Display, rc::Rc};

#[derive(Clone)]
struct Triple<T: Clone> {
    items: Vec<Option<Rc<Box<T>>>>,
}

impl<T: Clone> Triple<T> {
    fn new(input: Vec<Option<T>>) -> Self {
        assert_eq!(input.len(), 3, "Triple must have exactly 3 items");

        Self {
            items: input
                .into_iter()
                .map(|i| match i {
                    Some(i) => Some(Rc::new(Box::new(i))),
                    None => None,
                })
                .collect(),
        }
    }
}

//enum Something<T> => Triple<T> or Triple<Something<T>> /* End or 'pointer' to SOmething */
#[derive(Clone)]
enum Leaf<T: Clone> {
    Value(Triple<T>),
    Node(Triple<Leaf<T>>),
}

struct ReadonlyArray<T: Clone> {
    root: Leaf<T>,
}

impl<T: Clone> ReadonlyArray<T> {
    fn new(input: Vec<T>) -> Self {
        if input.len() <= 3 {
            return Self {
                root: Leaf::Value(Triple::new(input.into_iter().map(Some).collect())),
            };
        }

        let depth = (input.len() as f64).log(3.0).ceil() as u32;
        let mut tripples = Vec::new();

        input.chunks(3).for_each(|window| {
            let triple = Triple::new(window.iter().cloned().map(Some).collect());
            tripples.push(Leaf::Value(triple));
        });

        for _ in 1..depth {
            let mut new_tripples = Vec::new();
            tripples.chunks(3).for_each(|chunk| {
                let triple = Triple::new(chunk.iter().cloned().map(|item| Some(item)).collect());
                new_tripples.push(Leaf::Node(triple));
            });
            tripples = new_tripples;
        }

        Self {
            root: tripples.into_iter().next().unwrap(),
        }
    }
}

impl<T: std::fmt::Display + Clone> std::fmt::Display for ReadonlyArray<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        // We start with an empty prefix
        fmt_recursive(&self.root, "", true, f)
    }
}

// Helper function to handle the tree indentation logic
fn fmt_recursive<T: std::fmt::Display + Clone>(
    node: &Leaf<T>,
    prefix: &str,
    is_last: bool,
    f: &mut std::fmt::Formatter<'_>,
) -> std::fmt::Result {
    // 1. Determine the connector style for the current node
    let connector = if is_last { "└── " } else { "├── " };

    // 2. Calculate the prefix for the *next* generation of children
    let child_prefix = format!("{}{}", prefix, if is_last { "    " } else { "│   " });

    match node {
        // Recursive Step: It's a branch containing other Leaves
        Leaf::Node(triple) => {
            writeln!(f, "{}{}[Node]", prefix, connector)?;

            // Filter out None, enumerate to know position
            let children: Vec<_> = triple.items.iter().filter_map(|x| x.as_ref()).collect();

            for (i, child_ptr) in children.iter().enumerate() {
                let is_last_child = i == children.len() - 1;
                // Recurse deeper. Note: child_ptr is Rc<Box<Leaf<T>>>
                fmt_recursive(&***child_ptr, &child_prefix, is_last_child, f)?;
            }
        }

        // Base Case: It's a cluster of actual Values
        Leaf::Value(triple) => {
            writeln!(f, "{}{}[Leaf]", prefix, connector)?;

            let values: Vec<_> = triple.items.iter().filter_map(|x| x.as_ref()).collect();

            for (i, val_ptr) in values.iter().enumerate() {
                let is_last_val = i == values.len() - 1;
                let val_connector = if is_last_val {
                    "└── "
                } else {
                    "├── "
                };
                // Dereference Rc -> Box -> T
                writeln!(f, "{}{}{}{}", child_prefix, val_connector, ***val_ptr, "")?;
            }
        }
    }
    Ok(())
}

fn main() {
    let tripple_array = ReadonlyArray::new(vec![1, 2, 3, 4]);
    println!("{}", tripple_array);
}
