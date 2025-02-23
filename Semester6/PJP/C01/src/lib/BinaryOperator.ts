import { Node } from './Node';
import { Value } from './Value';

export type Operator = '+' | '-' | '*' | '/';

export class BinaryOperator extends Node<Operator> {
    constructor(
        operator: Operator,
        left: BinaryOperator | Value,
        right: BinaryOperator | Value
    ) {
        super(operator, left, right);
    }
}
