import { Terminal } from './Terminal';

export class Epsilon extends Terminal {
    constructor() {
        super('');
    }

    toString(): string {
        return 'ε';
    }
}
