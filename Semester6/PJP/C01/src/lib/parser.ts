import { BinaryOperator } from './BinaryOperator';
import { Node } from './Node';

const symbols = ['+', '-', '*', '/'];

export const parse = (text: string) => {
    let num = '';
    let start = 0;

    for (let pos = 0; pos < text.length; ++pos) {
        const char = text[pos];

        if (symbols.includes(char)) {
            doParse(text.substring(start, pos), char);
            start = pos + 1;
        }
    }
    doParse(text.substring(start));
};

const doParse = (left: string, char?: string) => {
    //@TODO check braces
    const num = parseInt(left.trim());
    if (isNaN(num)) {
        return undefined;
    }

    return [left, char];
};
