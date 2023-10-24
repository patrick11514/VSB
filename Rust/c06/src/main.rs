#[derive(PartialEq)]
struct Vec2D {
    x: f32,
    y: f32,
}

/*impl PartialEq for Vec2D {
    fn eq(&self, other: &Self) -> bool {
        self.x == other.x && self.y == other.y
    }
}*/

fn find_index<T: PartialEq>(items: &[T], needle: T) -> Option<usize> {
    for (index, item) in items.iter().enumerate() {
        if *item == needle {
            return Some(index);
        }
    }

    None
}

fn main() {
    let ret = find_index(&[1, 2, 3], 4);
    println!("{ret:?}");

    let ret = find_index(&[1.0, 2.0, 3.0], 2.0);
    println!("{ret:?}");

    let ret = find_index(
        &[Vec2D { x: 0.0, y: 2.2 }, Vec2D { x: 2.4, y: 4.5 }],
        Vec2D { x: 2.4, y: 4.6 },
    );
    println!("{ret:?}");
}
