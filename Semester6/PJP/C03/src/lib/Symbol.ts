export class Symbol {
    constructor(public readonly name: string) {}

    toString(): string {
        return this.name;
    }
}
