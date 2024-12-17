import JSONdb from 'simple-json-db';

export class JsonDBAbstract<$DataType> {
    private db: JSONdb<$DataType[]>;

    constructor(path: string) {
        this.db = new JSONdb<$DataType[]>(path, {
            syncOnWrite: true
        });
    }

    addItem(item: $DataType): bigint {
        const current = this.db.get('data') || [];
        if (typeof item === 'object' && item && 'id' in item) {
            item.id = current.length + 1;
        }
        current.push(item);
        this.db.set('data', current);
        return BigInt(current.length);
    }

    removeItem(item: $DataType) {
        const current = this.db.get('data') || [];
        this.db.set(
            'data',
            current.filter((i) => i != item)
        );
    }

    getAll() {
        return this.db.get('data') || [];
    }
}
