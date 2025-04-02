import fs from 'node:fs';
import GrammarLexer from './lib/Generated/GrammarLexer';
import GrammarParser from './lib/Generated/GrammarParser';
import { CharStream, CommonTokenStream, ParseTreeWalker } from 'antlr4';
import EvalVisitor from './lib/EvalVisitor';
import EvalListener from './lib/EvalListener';

const data = fs.readFileSync('input.txt', 'utf8');
const charStream = new CharStream(data);
const lexer = new GrammarLexer(charStream);
const tokenStream = new CommonTokenStream(lexer);
const parser = new GrammarParser(tokenStream);
const tree = parser.prog();
if (parser.syntaxErrorsCount == 0) {
    console.log(tree.toStringTree(parser.ruleNames, parser));

    const visitor = new EvalVisitor();
    visitor.visit(tree);

    ParseTreeWalker.DEFAULT.walk(new EvalListener(), tree);
}
