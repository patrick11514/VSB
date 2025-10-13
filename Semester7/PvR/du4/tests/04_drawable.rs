//! Run this file with `cargo test --test 04_drawable`.

//! TODO: Implement a function called `draw` that will render a set of drawable shapes onto a string
//! canvas.
//!
//! The `draw` function will receive:
//! - The height of the canvas
//! - The width of the canvas
//! - A slice of things that can be drawn
//!
//! Think about the signature of the function. What approach should be used to work with a set of
//! drawable types? Generics or runtime polymorphism (v-tables)?
//!
//! Implement two geometrical shapes that can be drawn (`Rectangle` and `Circle`).
//! `Rectangle::new` will receive the top left corner of the rectangle, its width, height and the
//! pixels that it contains (in this order).
//! `Circle::new` will receive the center of the circle, its radius and the pixels that it contains
//! (in this order).
//!
//! The coordinate system has its top-left coordinate at (0, 0), rows grow down and columns
//! grow to the right.

#[derive(Copy, Clone)]
struct Point {
    row: u32,
    col: u32,
}

impl Point {
    fn from_row_col(row: u32, col: u32) -> Self {
        Self { row, col }
    }
}

trait Drawable {
    /// If `point` is contained within this drawable object, returns the pixel that should be drawn.
    /// Otherwise, returns `None`.
    ///
    /// Use ONLY integer coordinates for all calculations in this function!
    fn pixel_at(&self, point: Point) -> Option<char>;
}

struct Rectangle {
    point: Point,
    width: u32,
    height: u32,
    char: char,
}

impl Rectangle {
    fn new(point: Point, width: u32, height: u32, char: char) -> Self {
        Rectangle {
            point,
            width,
            height,
            char,
        }
    }
}

impl Drawable for Rectangle {
    fn pixel_at(&self, point: Point) -> Option<char> {
        if point.row >= self.point.row
            && point.row < (self.point.row + self.height)
            && point.col >= self.point.col
            && point.col < (self.point.col + self.width)
        {
            Some(self.char)
        } else {
            None
        }
    }
}

struct Circle {
    point: Point,
    radius: u32,
    char: char,
}

impl Circle {
    fn new(point: Point, radius: u32, char: char) -> Self {
        Circle {
            point,
            radius,
            char,
        }
    }
}

impl Drawable for Circle {
    fn pixel_at(&self, point: Point) -> Option<char> {
        let dx = self.point.col.abs_diff(point.col);
        let dy = self.point.row.abs_diff(point.row);

        //let diff = dx * dx + dy + dy;

        //my circle have spikes :(
        if dx * dx + dy * dy <= self.radius * self.radius {
            // also this didn't worked :(
            // if  diff.isqrt() <= self.radius {
            Some(self.char)
        } else {
            None
        }

        //and I tried for fun to prompt ChatGPT for it and surprisingly the code generated the
        //correct circles, but I don't want to present this code as mine :)
        /*
            let cx = self.point.col as f64 + 0.5;
            let cy = self.point.row as f64 + 0.5;
            let r  = self.radius as f64;

            let x0 = point.col as f64;
            let y0 = point.row as f64;
            let x1 = x0 + 1.0;
            let y1 = y0 + 1.0;

            let dx = if cx < x0 { x0 - cx } else if cx > x1 { cx - x1 } else { 0.0 };
            let dy = if cy < y0 { y0 - cy } else if cy > y1 { cy - y1 } else { 0.0 };

            if dx*dx + dy*dy <= r*r { Some(self.char) } else { None }
        */
    }
}

fn draw(height: u32, width: u32, objects: &[&dyn Drawable]) -> String {
    let mut canvas = String::with_capacity((width * height + width) /* for new line */ as usize);

    for row in 0..height {
        for col in 0..width {
            let pixel = objects
                .iter()
                .filter_map(|obj| obj.pixel_at(Point { row, col }))
                .next_back();
            canvas.push(pixel.unwrap_or('.'));
        }
        canvas.push('\n');
    }

    canvas
}

/// Below you can find a set of unit tests.
/// The tests use the [`insta`](https://insta.rs/) snapshot testing library to make it easier to
/// compare string diffs.
#[cfg(test)]
mod tests {
    use crate::{Circle, Point, Rectangle, draw};

    #[test]
    fn rectangle_simple() {
        let res = draw(
            8,
            8,
            &[&Rectangle::new(Point::from_row_col(3, 4), 4, 2, 'x')],
        );
        insta::assert_snapshot!(res, @r"
        ........
        ........
        ........
        ....xxxx
        ....xxxx
        ........
        ........
        ........
        ");
    }

    #[test]
    fn rectangle_non_square_canvas() {
        let res = draw(
            8,
            12,
            &[&Rectangle::new(Point::from_row_col(3, 4), 4, 2, 'x')],
        );
        insta::assert_snapshot!(res, @r"
        ............
        ............
        ............
        ....xxxx....
        ....xxxx....
        ............
        ............
        ............
        ");
    }

    #[test]
    fn rectangle_oob() {
        let res = draw(
            3,
            2,
            &[&Rectangle::new(Point::from_row_col(0, 1), 5, 2, 'x')],
        );
        insta::assert_snapshot!(res, @r"
        .x
        .x
        ..
        ");
    }

    #[test]
    fn rectangle_other_char() {
        let res = draw(
            4,
            4,
            &[&Rectangle::new(Point::from_row_col(0, 0), 2, 2, 'p')],
        );
        insta::assert_snapshot!(res, @r"
        pp..
        pp..
        ....
        ....
        ");
    }

    #[test]
    fn circle_simple() {
        let res = draw(12, 12, &[&Circle::new(Point::from_row_col(6, 6), 3, 'x')]);
        insta::assert_snapshot!(res, @r"
        ............
        ............
        ............
        ....xxxxx...
        ...xxxxxxx..
        ...xxxxxxx..
        ...xxxxxxx..
        ...xxxxxxx..
        ...xxxxxxx..
        ....xxxxx...
        ............
        ............
        ");
    }

    #[test]
    fn multiple() {
        let res = draw(
            15,
            15,
            &[
                &Circle::new(Point::from_row_col(7, 7), 3, 'x'),
                &Rectangle::new(Point::from_row_col(1, 1), 10, 2, 'o'),
            ],
        );
        insta::assert_snapshot!(res, @r"
        ...............
        .oooooooooo....
        .oooooooooo....
        ...............
        .....xxxxx.....
        ....xxxxxxx....
        ....xxxxxxx....
        ....xxxxxxx....
        ....xxxxxxx....
        ....xxxxxxx....
        .....xxxxx.....
        ...............
        ...............
        ...............
        ...............
        ");
    }

    #[test]
    fn overwrite() {
        let res = draw(
            15,
            15,
            &[
                &Circle::new(Point::from_row_col(7, 7), 3, 'x'),
                &Rectangle::new(Point::from_row_col(1, 1), 7, 14, 'o'),
                &Circle::new(Point::from_row_col(7, 5), 3, '.'),
                &Rectangle::new(Point::from_row_col(5, 3), 5, 5, 'J'),
            ],
        );
        insta::assert_snapshot!(res, @r"
        ...............
        .ooooooo.......
        .ooooooo.......
        .ooooooo.......
        .oo.....xx.....
        .o.JJJJJ.xx....
        .o.JJJJJ.xx....
        .o.JJJJJ.xx....
        .o.JJJJJ.xx....
        .o.JJJJJ.xx....
        .oo.....xx.....
        .ooooooo.......
        .ooooooo.......
        .ooooooo.......
        .ooooooo.......
        ");
    }
}
