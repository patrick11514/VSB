import readline from 'node:readline/promises';
import { evaluate, parse } from './lib/parser';

const io = readline.createInterface({
    input: process.stdin
});

const lines = parseInt(await io.question(''));
for (let i = 0; i < lines; ++i) {
    try {
        const result = parse(await io.question(''));
        console.log(evaluate(result));
    } catch (e) {
        console.error(e);
        console.log('ERROR');
    }
}

io.close();
