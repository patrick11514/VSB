const fs = require("fs");

const file = fs.readFileSync("TestovaciData/PrumerKomponenty_ShlukovaciKoef_Centralita/Graf1.txt", "utf8");

const lines = file.split("\n");

let highest = 0;

let elements = []

for (let line of lines) {
    let [one, two] = line.split(" ");

    one = parseInt(one);
    two = parseInt(two);

    if (one > highest) {
        highest = one;
    }

    if (two > highest) {
        highest = two;
    }

    elements.push(one, two);
}

for (let i = 0; i < highest; i++) {
    if (!elements.includes(i)) {
        //console.log(i);
    }
}

