import { BinaryOperator, Operator } from './BinaryOperator';
import { Value } from './Value';

const symbols = ['+', '-', '*', '/'];
export const parse = (text: string): BinaryOperator | Value => {
    text = text.replace(/\s+/g, ''); // Remove spaces

    if (text.length === 0) {
        throw new Error('Empty expression');
    }

    // remove brackets
    if (
        text.startsWith('(') &&
        text.endsWith(')') &&
        isValidBrackets(text.substring(1, text.length - 1))
    ) {
        return parse(text.substring(1, text.length - 1));
    }

    let lastOperatorIndex = -1;
    let minPriority = 3; //something higher than 1 and 2
    let bracketLevel = 0;

    //find operator with lowest priority (+ or -)
    //so the + or - is on top of the tree and * or / is below
    for (let i = 0; i < text.length; i++) {
        const char = text[i];

        if (char === '(') {
            bracketLevel++;
        } else if (char === ')') {
            bracketLevel--;
        }

        if (bracketLevel === 0 && symbols.includes(char)) {
            const priority = getOperatorPriority(char as Operator);
            if (priority <= minPriority) {
                minPriority = priority;
                lastOperatorIndex = i;
            }
        }
    }

    if (lastOperatorIndex !== -1) {
        return new BinaryOperator(
            text[lastOperatorIndex] as Operator,
            parse(text.substring(0, lastOperatorIndex)),
            parse(text.substring(lastOperatorIndex + 1))
        );
    }

    if (!isNaN(Number(text))) {
        return new Value(Number(text));
    }

    throw new Error('Invalid expression');
};

export const evaluate = (node: BinaryOperator | Value): number => {
    if (node instanceof Value) {
        return node.value;
    }

    switch (node.value) {
        case '+':
            return evaluate(node.left!) + evaluate(node.right!);
        case '-':
            return evaluate(node.left!) - evaluate(node.right!);
        case '*':
            return evaluate(node.left!) * evaluate(node.right!);
        case '/':
            return evaluate(node.left!) / evaluate(node.right!);
    }
};

const isValidBrackets = (text: string): boolean => {
    let count = 0;
    for (const char of text) {
        if (char === '(') count++;
        if (char === ')') count--;
        if (count < 0) return false;
    }
    return count === 0;
};

const getOperatorPriority = (operator: Operator): number => {
    return operator === '+' || operator === '-' ? 1 : 2;
};
