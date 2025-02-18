type Cat = {
    name: string;
    age: number;
    color: 'white' | 'orange' | 'black' | 'gray';
};

const myCuteAnimal = {
    name: 'Mourek',
    age: 2,
    color: 'orange'
} satisfies Cat;

console.log(myCuteAnimal);
