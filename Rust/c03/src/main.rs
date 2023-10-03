/*fn take_items(mut a: [i32; 4]) {
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

fn read_write(file: &OpenedFile, file2: &mut OpenedFile) {
    println!("{}", file.fd);
}*/

//pokud chci vracet referenci a jako parameter mám více referencí, tak musím udělat "label" pro ty reference a tím specifikovat, kterou z nich budu vracet, protože compiler checkuje pouze signaturu a returny a neví, kterou tu referenci budeme vracet
fn foo<'a>(a: &'a u32, b: &'a u32) -> &'a u32 {
    if true {
        a
    } else {
        b
    }
}

fn main() {
    /*let a = [1, 2, 3, 4];

    //předávání kopie, tedy se to nezmění
    take_items(a);
    //+ nepříjdu o ownership, protože i32 je copy typ, tím pádem pole to přebírá taky a tedy pole i32 je copy typ
    take_items(a);

    println!("Aa: {}", a[0]);*/

    /*let mut file = OpenedFile { fd: 0 };

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
    }*/

    //let mut file = OpenedFile { fd: 0 };
    /*//let file_ref: &OpenedFile /*& = Shared reference, lze jich udělat více, nelze měnit přes to věci */ = &file;
    let file_ref: &mut OpenedFile /* Mut reference, lze udělat pouze jednu, ale lze přes ni měnit data */ = &mut file;

    file_ref.fd = 5;

    foo(file_ref); // nemusíme to castit, z &mut OpenedFile na &OpenedFile
    foo(file_ref);*/

    //read_write(&file, &mut file); //Error

    /*let file2 = &file;
    let file3 = file2; //Shared lze kopírovat*/

    /*let file2 = &mut file;
    let file3 = file2;

    file2.fd = 3; //Movene se mi hodnota, takže nemůžu dělat kopie
    file3.fd = 5;*/
}
