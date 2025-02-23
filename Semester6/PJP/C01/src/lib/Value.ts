import { Node } from './Node';

export class Value extends Node<number> {
    constructor(value: number) {
        super(value);
    }
}
