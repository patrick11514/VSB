import fs from 'node:fs';
import { parseGrammar } from './lib/parser';

const input = fs.readFileSync('G3.txt', 'utf8');

const grammar = parseGrammar(input);
const first = grammar.computeFirstSets();
for (const [rule, set] of first) {
    console.log(`first[${rule.lhs}:${rule.rhs.join('')}] = ${[...set].join(' ')}`);
}

const follow = grammar.computeFollowSets();
for (const [rule, set] of follow) {
    console.log(`follow[${rule}] = ${[...set].join(' ')}`);
}
