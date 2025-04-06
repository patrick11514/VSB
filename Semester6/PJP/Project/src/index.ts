import fs from 'node:fs';
import ArrayInitLexer from './lib/Generated/ProjectLexer';
import ArrayInitParser from './lib/Generated/ProjectParser';
import { CharStream, CommonTokenStream } from 'antlr4';

const data = fs.readFileSync('in3.txt', 'utf8');
const charStream = new CharStream(data);
const lexer = new ArrayInitLexer(charStream);
const tokenStream = new CommonTokenStream(lexer);
const parser = new ArrayInitParser(tokenStream);
const tree = parser.start();
console.log(tree.toStringTree(parser.ruleNames, parser));

if (parser.syntaxErrorsCount == 0) {
} else {
    console.log('ERRORS: ', parser.syntaxErrorsCount);
}
