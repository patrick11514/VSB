use std::rc::Rc;

#[derive(Clone)]
struct Triple<T: Clone> {
    items: Vec<Option<Rc<Box<T>>>>,
}

impl<T: Clone> Triple<T> {
    fn new(input: Vec<Option<T>>) -> Self {
        let mut items = vec![None; 3];

        for (i, item) in input.into_iter().enumerate().take(3) {
            items[i] = item;
        }

        Self {
            items: items
                .into_iter()
                .map(|item| item.map(|v| Rc::new(Box::new(v))))
                .collect(),
        }
    }
}

#[derive(Clone)]
enum Leaf<T: Clone> {
    Value(Triple<T>),
    Node(Triple<Leaf<T>>),
}

#[derive(Clone)]
struct ReadonlyArray<T: Clone> {
    root: Leaf<T>,
    depth: u32,
}

//constructor
impl<T: Clone> ReadonlyArray<T> {
    fn new(input: Vec<T>) -> Self {
        if input.len() <= 3 {
            return Self {
                root: Leaf::Value(Triple::new(input.into_iter().map(Some).collect())),
                depth: 0,
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
            depth: depth - 1,
        }
    }

    pub fn get(&self, mut index: usize) -> Option<&T> {
        let mut current_node = &self.root;
        let mut children_item_count = 3_usize.pow(self.depth);

        loop {
            match current_node {
                Leaf::Node(triple) => {
                    let child_index = index / children_item_count;
                    index %= children_item_count;

                    children_item_count /= 3;

                    let child_ptr = triple.items.get(child_index)?;
                    current_node = &***child_ptr.as_ref()?;
                }
                Leaf::Value(triple) => {
                    let val_ptr = triple.items.get(index)?;
                    return val_ptr.as_ref().map(|rc| &***rc);
                }
            }
        }
    }

    pub fn set(&self, index: usize, value: T) -> Self {
        let child_item_count = 3_usize.pow(self.depth);

        let new_root = self.set_recursive(&self.root, index, value, child_item_count);

        Self {
            root: new_root,
            depth: self.depth,
        }
    }

    fn set_recursive(
        &self,
        node: &Leaf<T>,
        index: usize,
        value: T,
        child_item_count: usize,
    ) -> Leaf<T> {
        match node {
            Leaf::Node(triple) => {
                let child_idx = index / child_item_count;
                let remainder_idx = index % child_item_count;
                let next_stride = child_item_count / 3;

                let mut new_items = triple.items.clone();

                if let Some(child_rc) = &triple.items[child_idx] {
                    let old_child = &***child_rc;

                    let new_child =
                        self.set_recursive(old_child, remainder_idx, value, next_stride);

                    new_items[child_idx] = Some(Rc::new(Box::new(new_child)));
                } else {
                    panic!("Index out of bounds (node gap)");
                }

                Leaf::Node(Triple { items: new_items })
            }

            Leaf::Value(triple) => {
                let mut new_items = triple.items.clone();

                if index >= 3 || new_items[index].is_none() {
                    panic!("Index out of bounds");
                }

                new_items[index] = Some(Rc::new(Box::new(value)));

                Leaf::Value(Triple { items: new_items })
            }
        }
    }
}

//helper iterator structure
struct ArrayIter<T: Clone> {
    stack: Vec<Leaf<T>>,
    current_values: std::vec::IntoIter<Option<Rc<Box<T>>>>,
}

impl<T: Clone> Iterator for ArrayIter<T> {
    type Item = T;

    fn next(&mut self) -> Option<Self::Item> {
        loop {
            while let Some(item_opt) = self.current_values.next() {
                if let Some(val_rc) = item_opt {
                    return Some((**val_rc).clone());
                }
            }

            let next_node = self.stack.pop()?;

            match next_node {
                Leaf::Node(triple) => {
                    for child in triple.items.into_iter().rev() {
                        if let Some(child_rc) = child {
                            self.stack.push((**child_rc).clone());
                        }
                    }
                }
                Leaf::Value(triple) => {
                    self.current_values = triple.items.into_iter();
                }
            }
        }
    }
}

impl<T: Clone> IntoIterator for ReadonlyArray<T> {
    type Item = T;
    type IntoIter = ArrayIter<T>;

    fn into_iter(self) -> Self::IntoIter {
        ArrayIter {
            stack: vec![self.root],
            current_values: Vec::new().into_iter(),
        }
    }
}

fn main() {
    let tripple_array = ReadonlyArray::new((0..=30).collect::<Vec<_>>());

    println!("=========================== Tripple Array ===========================");
    println!("{}", tripple_array);

    let ten_value = tripple_array.get(10);
    println!("Get index 10: {:?}", ten_value);

    for value in tripple_array.clone() {
        println!("{}", value);
    }

    let updated_array = tripple_array.set(12, 999);

    println!("======================== Updated Tripple Array ========================");
    println!("{}", updated_array);

    let ten_value_updated = updated_array.get(10);
    println!("Get index 10 from updated array: {:?}", ten_value_updated);

    //check ptr equality
    let original_twelve = ten_value.unwrap();
    let updated_twelve = ten_value_updated.unwrap();
    println!(
        "Pointer equality check for index 10: {}",
        std::ptr::eq(original_twelve, updated_twelve)
    );
}

//PRINTING UTILS

impl<T: std::fmt::Display + Clone> std::fmt::Display for ReadonlyArray<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        fmt_recursive(&self.root, "", true, f)
    }
}

fn fmt_recursive<T: std::fmt::Display + Clone>(
    node: &Leaf<T>,
    prefix: &str,
    is_last: bool,
    f: &mut std::fmt::Formatter<'_>,
) -> std::fmt::Result {
    let connector = if is_last { "└── " } else { "├── " };

    let child_prefix = format!("{}{}", prefix, if is_last { "    " } else { "│   " });

    match node {
        Leaf::Node(triple) => {
            writeln!(f, "{}{}[Node]", prefix, connector)?;

            let children = &triple.items;

            for (i, child_ptr) in children.iter().enumerate() {
                let is_last_child = i == children.len() - 1;
                if let Some(child_ptr) = child_ptr {
                    fmt_recursive(&***child_ptr, &child_prefix, is_last_child, f)?;
                } else {
                    let child_connector = if is_last_child {
                        "└── "
                    } else {
                        "├── "
                    };
                    writeln!(f, "{}{}<empty>", child_prefix, child_connector)?;
                }
            }
        }

        Leaf::Value(triple) => {
            writeln!(f, "{}{}[Leaf]", prefix, connector)?;

            let values = &triple.items;

            for (i, val_ptr) in values.iter().enumerate() {
                let is_last_val = i == values.len() - 1;
                let val_connector = if is_last_val {
                    "└── "
                } else {
                    "├── "
                };

                if let Some(val_ptr) = val_ptr {
                    writeln!(f, "{}{}{}{}", child_prefix, val_connector, ***val_ptr, "")?;
                } else {
                    writeln!(f, "{}{}<empty>", child_prefix, val_connector)?;
                }
            }
        }
    }
    Ok(())
}
