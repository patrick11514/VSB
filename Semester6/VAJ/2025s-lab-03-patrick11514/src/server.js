import axios from 'axios';
import express from 'express';
import fs from 'node:fs/promises';

const app = express();

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// this is the simplest example - if you go to the localhost:3000, if show you "Hello world"
app.get('/', (req, res) => {
    res.send(req.headers);
});

// TASKS FOR SIMPLE ROUTING

//  TASK 0. GET request /testing-json and return JSON - simple object with your name, lastname, home
// return something like { firstname: "Obiwan", lastname: "Kenobi", home: "Tattoine"}
// Hint: For JSON response see Response methods https://expressjs.com/en/guide/routing.html - .json()

app.get('/testing-json', (req, res) => {
    res.json({ firstname: 'Obiwan', lastname: 'Kenobi', home: 'Tattoine' });
});

// TASK 1. GET request /vehicles and return array of simple objects of vehicles in JSON format.
// USE data/vehicles.json file. You have to do transformation of the loaded json file. Remove the extra fields!
// return [{id: 1, name: "Sand Crawler"}, {id: 2, name: "T-16 skyhopper"}, ...]
// Hint: See presentation 3, slide ~15? how to use async await and read file ...

app.get('/vehicles', async (req, res) => {
    const data = await fs.readFile('data/vehicles.json', 'utf8');
    const json = JSON.parse(data);

    res.json(json.map((item) => ({ id: item.id, name: item.name })));
});

// TASK 2. GET request /vehicles/id-of-vehicle and return whole object for specific vehicle
// request /vehicles/1 return object for Sand Crawler
// if you call for /vehicles/100 (this ID is not in vehicles.json), return { error: "We don't have vehicle with that ID"}
// only one function here! Not for each vehicle!
// Hint: See route parameters in https://expressjs.com/en/guide/routing.html

app.get('/vehicles/:id', async (req, res) => {
    const data = await fs.readFile('data/vehicles.json', 'utf8');
    const json = JSON.parse(data);

    const item = json.find((item) => item.id === parseInt(req.params.id));
    if (!item) {
        res.json({ error: "We don't have vehicle with that ID" });
        return;
    }

    res.json(item);
});

// TASK 3. POST request /secret-code
// If you send POST params password: {password: "2021-9-29-emperor-is-awesome"}   //(today in format YYYY-MM-DD-emperor-is-awesome)
// page should return text: "Your access is granted!"
// if password is wrong, it should return "Please send valid password or we are forced to destroy your ship."
// Password is changed every day - for tomorrow it is 2021-9-30-emperor-is-awesome
// Hint: For sending POST request, use Postman
// Hint: Add code console.log(req.body) send request, see what it returns
// Hint: Current date - many ways https://stackoverflow.com/questions/1531093/how-do-i-get-the-current-date-in-javascript

app.post('/secret-code', (req, res) => {
    const password = req.body.password;
    console.log(req.body);
    const now = new Date();
    const correctPassword = `${now.getFullYear()}-${now.getMonth() + 1}-${now.getDate()}-emperor-is-awesome`;

    if (!password || password != correctPassword) {
        res.json({
            error: 'Please send valid password or we are forced to destroy your ship.'
        });
        return;
    }

    res.json({ message: 'Your access is granted!' });
});

// TASK 4. GET request /search-vehicles with query parameters
// If you send GET request to /search-vehicles?cargoCapacityFrom=1000&cargoCapacityTo=2000
// it should return all vehicles from vehicles.json with cargo_capacity between 1000 and 2000
// Hint: be careful of items that have cargo_capacity as "none"

app.get('/search-vehicle', async (req, res) => {
    const data = await fs.readFile('data/vehicles.json', 'utf8');
    const json = JSON.parse(data);
    const query = req.query;
    let min = null;
    let max = null;

    if ('cargoCapacityFrom' in query) {
        min = parseInt(query.cargoCapacityFrom);
    }

    if ('cargoCapacityTo' in query) {
        max = parseInt(query.cargoCapacityTo);
    }

    const filtered = json.filter((item) => {
        if (min !== null && item.cargo_capacity === 'none') {
            return false;
        }

        if (min !== null && parseInt(item.cargo_capacity) < min) {
            return false;
        }

        if (max !== null && parseInt(item.cargo_capacity) > max) {
            return false;
        }

        return true;
    });

    res.json(filtered);
});

// BONUS TASK 1 - 0.5 points
// Implement express middlware https://expressjs.com/en/guide/using-middleware.html
// https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Authorization
// This middleware will check "Authorization" header of each request.
// The authheader should be in this format `Basic <base64-vsb-login>` for example `Basic VkVDMDAyMg==`.
// The middleware should print the decoded base64 into console (how to decode https://stackoverflow.com/a/6182519)

app.use((req, res, next) => {
    const header = req.headers.authorization;
    if (header && header.startsWith('Basic ')) {
        const splited = header.split(' ')[1];
        const vsbLogin = Buffer.from(splited, 'base64').toString('utf-8');
        console.log(vsbLogin);
    }

    next();
});

// BONUS TASK 2 - 0.5 points
// Implement a new end point /films which will return all films from films.json
// Add optional query parameter expand, which can accept string: characters , vehicles, starships, species, planets
// When the expand is present, instead of a link in the array of characters, there should be the object of the character instead
// The same goes for the rest of the types

const expands = ['characters', 'vehicles', 'starships', 'species', 'planets'];
app.get('/films', async (req, res) => {
    const data = await fs.readFile('data/films.json', 'utf8');
    let json = JSON.parse(data).results;

    if (req.query.expand && expands.includes(req.query.expand)) {
        const expand = req.query.expand;

        json = await Promise.all(
            json.map(async (item) => {
                item[expand] = await Promise.all(
                    item[expand].map(async (url) => {
                        const response = await axios.get(url);
                        return response.data;
                    })
                );

                return item;
            })
        );
    }
    res.json(json);
});

export { app };
