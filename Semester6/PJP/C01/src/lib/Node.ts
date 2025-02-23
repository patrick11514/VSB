export class Node<$Type> {
    constructor(
        public value: $Type,
        public left?: Node<any>,
        public right?: Node<any>
    ) { }
}
