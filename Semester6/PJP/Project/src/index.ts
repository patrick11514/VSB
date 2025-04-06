import fs from 'node:fs';
import ArrayInitLexer from './lib/Generated/ProjectLexer';
import ArrayInitParser from './lib/Generated/ProjectParser';
import { CharStream, CommonTokenStream, ParseTreeWalker } from 'antlr4';
import { TypeChecker } from './lib/TypeChecker';
import { TypeError } from './lib/TypeError';

const FILENAME = 'inErr.txt';

const data = fs.readFileSync(FILENAME, 'utf8');
TypeError.fileName = FILENAME;
TypeError.sourceCode = data.split('\n');

const charStream = new CharStream(data);
const lexer = new ArrayInitLexer(charStream);
const tokenStream = new CommonTokenStream(lexer);
const parser = new ArrayInitParser(tokenStream);
const tree = parser.start();
//console.log(tree.toStringTree(parser.ruleNames, parser));

if (parser.syntaxErrorsCount == 0) {
    const walker = new ParseTreeWalker();
    const checker = new TypeChecker();
    walker.walk(checker, tree);

    if (checker.errors.length > 0) {
        for (const error of checker.errors) {
            console.log(error.toString());
        }
    }
} else {
    console.log(`Found ${parser.syntaxErrorsCount} errors`);
}
