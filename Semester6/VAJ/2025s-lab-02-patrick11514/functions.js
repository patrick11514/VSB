// MOVE ALL FUNCTIONS and array into separate files in folders `utils` and `data` - split it by comments above functions
// You should have only one implementation of every function. If you need specific function in different file, use export/import

// Line 91+ should work same after your work

import { hexToRgb, rgbToHex } from "./utils/colors.js";
import { validateEmail, isValidHttpUrl } from "./utils/network.js";
import {
    isVaderInShip,
    hasCorrectSeatColor,
    addImperialSign,
} from "./utils/imperium.js";

// passengers data - data about passengers in the ship
const passengersOnShip = [
    {
        name: "Vader",
        id: 1,
        seat: "3E",
        colorOfSeat: rgbToHex(0, 0, 0),
    },
    {
        name: "TK-202",
        id: 2,
        seat: "17C",
        colorOfSeat: rgbToHex(255, 255, 0),
    },
];

// ======================
// THIS SHOULD WORK AFTER YOU MOVE FUNCTIONS INTO SEPARATE FILES

console.log(hexToRgb("#CECE20"));
console.log(rgbToHex(0, 51, 255));

console.log("[INVALID EMAIL]", validateEmail("vader.skywalker"));
console.log("[VALID EMAIL]", validateEmail("vader@imperium.com"));

console.log("[VALID URL]", isValidHttpUrl("https://imperium.com"));
console.log("[VALID URL]", isValidHttpUrl("imperiumIsAwesome.com"));
console.log("[INVALID URL]", isValidHttpUrl("rebelsAreNotCoolAtAll"));

console.log("[VADER ON THE BOARD]", isVaderInShip(passengersOnShip));
console.log(
    "[VADER IS ON CORRECT SEAT]",
    hasCorrectSeatColor(passengersOnShip, "Vader", "#000000"),
);
console.log(addImperialSign("IMPERIUM IS THE BEST"));
