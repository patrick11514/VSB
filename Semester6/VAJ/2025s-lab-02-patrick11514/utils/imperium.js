// IMPERIUM - check if Vader is on the ship :D
export const isVaderInShip = (passengers) => {
    if (passengers.some((e) => e.name === "Vader")) {
        return true;
    }
    return false;
};

// IMPERIUM - check if passenger has correct color of seat.
export const hasCorrectSeatColor = (passengers, name, color) => {
    return passengers.some(
        (passenger) => passenger.name === name && passenger.colorOfSeat === color,
    );
};

// IMPERIUM - add standard imperium anotation around message
export const addImperialSign = (message) => {
    return `[MESSAGE]⚠️ ⚠️ ⚠️  ==== To: All imperial soldiers! | Message:🚀 🔥 ${message} 🔥 🚀 ====  ⚠️ ⚠️ ⚠️`;
};
