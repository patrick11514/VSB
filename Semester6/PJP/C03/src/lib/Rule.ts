import { NonTerminal } from './NonTerminal';
import { Symbol } from './Symbol';

export class Rule {
    public rhs: Symbol[] = [];

    constructor(public readonly lhs: NonTerminal) {}
}
