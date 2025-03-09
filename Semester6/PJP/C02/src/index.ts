import fs from 'node:fs';
import { Tokenizer } from './lib/Tokenizer';
import { Token } from './lib/Token';

const input = fs.readFileSync('input.txt', 'utf8');

const tokenizer = new Tokenizer(input);

let token: Token;
while ((token = tokenizer.getNextToken()).name !== 'EOF') {
    console.log(`${token.name}${token.value ? `:${token.value}` : ''}`);
}
