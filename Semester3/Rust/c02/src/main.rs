/*mod foo {
    //product type
    struct User {
        age: u8, // 256 různých hodnot
        alive: bool, // 2 různé hodnoty
                 // 256 * 2 = 512 různých hodnot
    }

    //tuple struct
    struct Car(u32, u32, bool);

    // Newtype
    pub struct CarId(u32);

    pub fn create_car_id(a: u32) -> CarId {
        assert!(a > 1000);
        CarId(a)
    }
}

// Algebraické datové typy
// discriminated union
enum PrintType {
    MsgPack,
    Json { indent: u32 },
}

fn print_data(data: u32, print_type: PrintType) {}

fn main() {
    //print_data(1, PrintType::Json { indent: 4 });

    let print_type = PrintType::Json { indent: 4 };

    let indent = match print_type {
        PrintType::MsgPack => 0,
        PrintType::Json { indent } => indent,
    };

    //let mut car = foo::create_car_id(1256);

    /*let user = foo::User {
        age: 5,
        alive: true,
    };

    println!("My age: {} and i'm alive: {}", user.age, user.alive);*/
}*/

struct File {
    fd: u32,
}

fn close(file: File) {}

fn main() {
    let file = File { fd: 5 };
    close(file);
    //ownership se přesunul do funkce close, tedy nemůžeme použít file
    close(file);
}
