export class Float {
    private value: number;
    constructor(value: string | number) {
        if (typeof value === 'number') {
            this.value = value;
        } else {
            this.value = parseFloat(value);
        }
    }

    getValue() {
        return this.value;
    }
}
