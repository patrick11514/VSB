import axios from 'axios';

const start = performance.now();

// Task: Display average weight (mass) of people on each planet. https://swapi.profiq.com/api/planets

// Eeach planet has list of residents.
// Example of the API response per one planet:
/* 
{
    "id": 10,
    "name": "Kamino",
    "created": "2014-12-10T12:45:06Z",
    "url": "https://swapi.profiq.com/api/planets/10",
    "diameter": "19720",
    "edited": "2014-12-20T20:58:18Z",
    "films": [
        "https://swapi.profiq.com/api/films/5"
    ],
    "terrain": "ocean",
    "surface_water": "100",
    "rotation_period": "27",
    "residents": [
        "https://swapi.profiq.com/api/people/22",
        "https://swapi.profiq.com/api/people/72",
        "https://swapi.profiq.com/api/people/73"
    ],
    "population": "1000000000",
    "orbital_period": "463",
    "gravity": "1 standard",
    "climate": "temperate"
}
*/

// You need to query each person from each planet like this: https://swapi.profiq.com/api/people/22
// In case of kamiko, it would be 3 requests...
// You can utilize promise all to speed up your requests and run them at the same time.
// Be careful the mass can be "unknown" in that case, don't count it! https://swapi.profiq.com/api/people/73

// Example output to console:

// Planet: Kamiko
// Number of residents: 3
// Average mass of resident:  83.1
//
// Planet: ...
// ...

const response = await axios.get('https://swapi.profiq.com/api/planets');
const planets = await Promise.all(
    response.data.results.map(async (planet) => {
        const residents = await Promise.all(
            planet.residents.map(async (resident) => {
                const response = await axios.get(resident);
                return response.data;
            })
        );

        const masses = residents
            .map((resident) => parseFloat(resident.mass))
            .filter((mass) => !isNaN(mass));

        return {
            name: planet.name,
            residents: residents.length,
            avgMass: masses.reduce((a, b) => a + b, 0) / masses.length
        };
    })
);
console.log(
    planets
        .map(
            (planet) => `Planet: ${planet.name}
Number of residents: ${planet.residents}
Average mass of resident: ${planet.avgMass}`
        )
        .join('\n\n')
);

// End of your code
const end = performance.now();
console.log(`The execution took ${(end - start) | 0} ms`);
