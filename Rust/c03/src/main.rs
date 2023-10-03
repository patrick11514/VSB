//
fn take_items(mut a: [i32; 4]) {
    a[0] = 5;
}

struct OpenedFile {
    fd: u32,
}

enum ReadResult {
    Ok { file: OpenedFile, byte: i32 },
    Eof,
}

//je super si dělat na všechno enumy
fn read(file: OpenedFile) -> ReadResult {
    let byte = 5;

    if byte == 0 {
        ReadResult::Eof
    } else {
        ReadResult::Ok { file, byte }
    }
}

fn main() {
    /*let a = [1, 2, 3, 4];

    //předávání kopie, tedy se to nezmění
    take_items(a);
    //+ nepříjdu o ownership, protože i32 je copy typ, tím pádem pole to přebírá taky a tedy pole i32 je copy typ
    take_items(a);

    println!("Aa: {}", a[0]);*/

    let mut file = OpenedFile { fd: 0 };

    loop {
        //pak si to můžu matchnout a podle toho se rozhódnout
        match read(file) {
            //v Ok je file, ale file již máme udělaný, proto tedy přejmenuji na file2
            ReadResult::Ok { file: file2, byte } => {
                println!("{}", byte);
                file = file2;
            }
            ReadResult::Eof => break,
        }
    }
}
