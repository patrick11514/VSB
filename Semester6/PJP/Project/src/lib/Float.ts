export class Float {
    private value: number;
    constructor(value: string) {
        this.value = parseFloat(value);
    }

    getValue() {
        return this.value;
    }
}
