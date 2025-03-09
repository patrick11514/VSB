import { Token } from './Token';

class TokenizerError extends Error {
    constructor(message: string) {
        super(message);
        this.name = 'Tokenizer Error';
    }
}

export class Tokenizer {
    private position = 0;

    constructor(public readonly input: string) { }

    getNextToken(): Token {
        if (this.position >= this.input.length) {
            return new Token('EOF');
        }

        const char = this.input[this.position];

        if (char.match(/\s/)) {
            this.position++;
            return this.getNextToken();
        }

        // check for comment which starts with //
        if (char === '/' && this.input[this.position + 1] === '/') {
            while (this.input[this.position] !== '\n') {
                this.position++;
            }
            return this.getNextToken();
        }

        //resolve operators
        const ops = ['+', '-', '*', '/'];
        if (ops.includes(char)) {
            this.position++;
            return new Token('OP', char);
        }

        switch (char) {
            case '(':
                this.position++;
                return new Token('LPAR');
            case ')':
                this.position++;
                return new Token('RPAR');
            case ';':
                this.position++;
                return new Token('SEMICOLON');
        }

        const rest = this.input.slice(this.position);
        if (rest.startsWith('div')) {
            this.position += 3;
            return new Token('DIV');
        }

        if (rest.startsWith('mod')) {
            this.position += 3;
            return new Token('MOD');
        }

        if (char.match(/[0-9]/)) {
            //read number
            let number = '';
            while (this.input[this.position].match(/[0-9]/)) {
                number += this.input[this.position];
                this.position++;
            }

            return new Token('NUM', number);
        }

        //match identifier
        if (char.match(/[a-zA-Z]/)) {
            let identifier = '';
            while (this.input[this.position].match(/[a-zA-Z0-9]/)) {
                identifier += this.input[this.position];
                this.position++;
            }

            return new Token('ID', identifier);
        }

        throw new TokenizerError(`Unknown token ${char} at position ${this.position}`);
    }
}
