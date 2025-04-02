import fs from 'node:fs';
import ArrayInitLexer from './lib/Generated/ArrayInitLexer';
import ArrayInitParser from './lib/Generated/ArrayInitParser';
import { CharStream, CommonTokenStream, ParseTreeWalker } from 'antlr4';
import ValueListener from './lib/ValueListener';
import TransformVisitor from './lib/TransformVisitor';

const data = fs.readFileSync('input.txt', 'utf8');
const charStream = new CharStream(data);
const lexer = new ArrayInitLexer(charStream);
const tokenStream = new CommonTokenStream(lexer);
const parser = new ArrayInitParser(tokenStream);
const tree = parser.init();
console.log(tree.toStringTree(parser.ruleNames, parser));

if (parser.syntaxErrorsCount == 0) {
    const walker = new ParseTreeWalker();
    walker.walk(new ValueListener(), tree);

    const transform = new TransformVisitor();
    console.log(transform.visit(tree));
} else {
    console.log('ERRORS: ', parser.syntaxErrorsCount);
}
