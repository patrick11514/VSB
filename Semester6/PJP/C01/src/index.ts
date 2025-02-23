import readline from 'node:readline/promises';
import { parse } from './lib/parser';

const io = readline.createInterface({
    input: process.stdin
});

const lines = parseInt(await io.question(''));

for (let i = 0; i < lines; ++i) {
    console.log(parse(await io.question('')));
}

io.close();
