// 1. display (console.log) all names of vehicles from local file data/vehicles.json (folder data, file vehicles.json) (you can use FS readFileSync)
// How to use readFileSync can be found here https://www.geeksforgeeks.org/node-js-fs-readfilesync-method/
import fs from 'node:fs';

const data = fs.readFileSync('data/vehicles.json', 'utf8');
const vehicles = JSON.parse(data);
console.log(vehicles.map((vehicle) => vehicle.name));
