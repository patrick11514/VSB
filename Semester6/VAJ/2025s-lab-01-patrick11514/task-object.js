// Checkout useful functions at: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object
// Checkout useful functions at: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array

// 1 ----
// Create a function that takes an object argument sizes (contains width, length, height keys) and returns the volume of the box.
// Examples
// volumeOfBox({ width: 2, length: 5, height: 1 }) ➞ 10
// volumeOfBox({ width: 4, length: 2, height: 2 }) ➞ 16
// volumeOfBox({ width: 2, length: 3, height: 5 }) ➞ 30

// Your code:
const volumeOfBox = (obj) => {
    return obj.width * obj.length * obj.height;
};
console.log(volumeOfBox({ width: 2, length: 5, height: 1 }));

// 2 ----
// Create a function that takes strings - firstname, lastname, age, and return object with firstname, lastname, age, yearOfBirth
// Examples
// personObject("Obi-wan", "Kenobi", "40") ➞ { firstname: "Obi-wan", lastname: "Kenobi", age: 40, yearOfBirth: 1981 }

// Your code:
const personObject = (firstname, lastname, age) => {
    const ageNum = parseInt(age);
    return {
        firstname,
        lastname,
        age: ageNum,
        yearOfBirth: new Date().getFullYear() - ageNum
    };
};
console.log(personObject('Obi-wan', 'Kenobi', '40'));

// 3 ----
// Create the function that takes an array with objects and returns the sum of people's budgets.
// Example
// getBudgets([
//   { name: "John", age: 21, budget: 23000 },
//   { name: "Steve",  age: 32, budget: 40000 },
//   { name: "Martin",  age: 16, budget: 2700 }
// ]) ➞ 65700

//Your code:
const getBudgets = (persons) => {
    return persons.reduce((a, b) => a + b.budget, 0);
};
console.log(
    getBudgets([
        { name: 'John', age: 21, budget: 23000 },
        { name: 'Steve', age: 32, budget: 40000 },
        { name: 'Martin', age: 16, budget: 2700 }
    ])
);

// 4 ----
// Create function that takes array of cars and sort them by price
// Example
// const vehicles = [{name: "Executor Star Dreadnought", price: 999}, {name: "T-47 Airspeeder", price: 5}, {name: "AT-AT", price : 20}]
// sortVehiclesByPrice(vehicles) ➞ [{name: "T-47 Airspeeder", price :5}, {name: "AT-AT", price :20}, {name: "Executor Star Dreadnought", price: 999}]

// Your code:
const sortVehiclesByPrice = (vehicles) => {
    return [...vehicles].sort((a, b) => a.price - b.price);
};
const vehicles = [
    { name: 'Executor Star Dreadnought', price: 999 },
    { name: 'T-47 Airspeeder', price: 5 },
    { name: 'AT-AT', price: 20 }
];
console.log(sortVehiclesByPrice(vehicles));
