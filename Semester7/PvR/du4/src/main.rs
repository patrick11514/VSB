//! You can use this file for experiments.

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
        let cx = self.point.col as f64 + 0.5;
        let cy = self.point.row as f64 + 0.5;
        let r = self.radius as f64;

        let x0 = point.col as f64;
        let y0 = point.row as f64;
        let x1 = x0 + 1.0;
        let y1 = y0 + 1.0;

        let dx = if cx < x0 {
            x0 - cx
        } else if cx > x1 {
            cx - x1
        } else {
            0.0
        };
        let dy = if cy < y0 {
            y0 - cy
        } else if cy > y1 {
            cy - y1
        } else {
            0.0
        };

        if dx * dx + dy * dy <= r * r {
            Some(self.char)
        } else {
            None
        }
    }
}

fn draw(height: u32, width: u32, objects: &[&dyn Drawable]) -> String {
    let mut canvas = String::with_capacity((width * height + width) /* for new line */ as usize);

    for row in 0..height {
        for col in 0..width {
            let pixel = objects
                .iter()
                .filter_map(|obj| obj.pixel_at(Point { row, col }))
                .last();
            canvas.push(match pixel {
                Some(ch) => ch,
                None => '.',
            });
        }
        canvas.push('\n');
    }

    canvas
}

fn main() {
    println!("Hello Rust!");

    let res = draw(12, 12, &[&Circle::new(Point::from_row_col(6, 6), 3, 'x')]);

    println!("{}", res);
}
