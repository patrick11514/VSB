import { Float } from './Float';
import { stringToType, TypeLiteral, VariableType, VarType } from './utilities';
import readline from 'node:readline/promises';
import { stdin as input, stdout as output } from 'node:process';

type InstructionSet = Record<
    string,
    {
        argCount: number;
        callback: (args: any[], rest: string) => void | Promise<void>;
    }
>;

export class VirtualMachine {
    private stack: (string | number | Float | boolean)[] = [];
    private maxSize: number;
    private labels = new Map<string, number>();
    private variables = new Map<string, VarType | null>();
    private ip = 0;
    private io = readline.createInterface({ input, output });

    constructor(private sourceCode: string[]) {
        this.maxSize = sourceCode.length;
    }

    private initializeLabels() {
        for (let i = 0; i < this.maxSize; ++i) {
            const line = this.sourceCode[i];
            if (line.startsWith('label')) {
                this.labels.set(line.split(' ')[1], i);
            }
        }
    }

    private parseArguments(args: string[], count: number) {
        const _args = args.slice(0, count);
        const rest = args.slice(count).join(' ');
        return [_args, rest] as [string[], string];
    }

    private instructions: InstructionSet = {
        add: {
            argCount: 1,
            callback: this.add
        },
        sub: {
            argCount: 1,
            callback: this.sub
        },
        mul: {
            argCount: 1,
            callback: this.mul
        },
        div: {
            argCount: 1,
            callback: this.div
        },
        mod: {
            argCount: 0,
            callback: this.mod
        },
        uminus: {
            argCount: 1,
            callback: this.uminus
        },
        concat: {
            argCount: 0,
            callback: this.concat
        },
        gt: {
            argCount: 1,
            callback: this.gt
        },
        lt: {
            argCount: 1,
            callback: this.lt
        },
        eq: {
            argCount: 1,
            callback: this.eq
        },
        and: {
            argCount: 0,
            callback: this.and
        },
        or: {
            argCount: 0,
            callback: this.or
        },
        not: {
            argCount: 0,
            callback: this.not
        },
        itof: {
            argCount: 0,
            callback: this.itof
        },
        push: {
            argCount: 1,
            callback: this.push
        },
        pop: {
            argCount: 0,
            callback: () => {
                this.pop();
            }
        },
        load: {
            argCount: 1,
            callback: this.load
        },
        save: {
            argCount: 1,
            callback: this.save
        },
        label: {
            argCount: 1,
            callback: this.label
        },
        jmp: {
            argCount: 1,
            callback: this.jmp
        },
        fjmp: {
            argCount: 1,
            callback: this.fjmp
        },
        print: {
            argCount: 1,
            callback: this.print
        },
        read: {
            argCount: 1,
            callback: this.read
        }
    };

    async run() {
        this.initializeLabels();

        for (; this.ip < this.maxSize; ++this.ip) {
            const line = this.sourceCode[this.ip];
            const [instruction, ...args] = line.split(' ');

            if (instruction in this.instructions) {
                const [_args, rest] = this.parseArguments(
                    args,
                    this.instructions[instruction].argCount
                );
                await this.instructions[instruction].callback.bind(this)(_args, rest);
            }
        }
        this.io.close();
    }

    private pop() {
        const value = this.stack.pop();
        if (value === undefined) {
            throw new Error('Stack underflow');
        }
        return value;
    }

    private getType(varNameOfLiteral: VarType): VariableType {
        if (typeof varNameOfLiteral === 'string') {
            return VariableType.STRING;
        }
        if (typeof varNameOfLiteral === 'number') {
            return VariableType.INT;
        }
        if (varNameOfLiteral instanceof Float) {
            return VariableType.FLOAT;
        }
        if (typeof varNameOfLiteral === 'boolean') {
            return VariableType.BOOL;
        }
        return VariableType.NULL;
    }

    private toNumber(value: VarType): number {
        if (typeof value === 'number') {
            return value;
        }
        if (value instanceof Float) {
            return value.getValue();
        }
        throw new Error(`Cannot convert ${value} to number`);
    }

    private getOpNums(type: VariableType) {
        const first = this.pop();
        const firstType = this.getType(first);
        const second = this.pop();
        const secondType = this.getType(second);

        if (firstType !== type) {
            throw new Error(
                `Type mismatch: expected ${VariableType[type]}, got ${VariableType[firstType]}`
            );
        }

        if (secondType !== type) {
            throw new Error(
                `Type mismatch: expected ${VariableType[type]}, got ${VariableType[secondType]}`
            );
        }

        return [second, first] as const;
    }

    private add([_type]: TypeLiteral[]) {
        const type = stringToType(_type);
        const [first, second] = this.getOpNums(type);
        if (type === VariableType.INT) {
            this.stack.push((first as number) + (second as number));
        } else {
            //float
            this.stack.push(
                new Float((first as Float).getValue() + (second as Float).getValue())
            );
        }
    }

    private sub([_type]: TypeLiteral[]) {
        const type = stringToType(_type);
        const [first, second] = this.getOpNums(type);
        if (type === VariableType.INT) {
            this.stack.push((first as number) - (second as number));
        } else {
            //float
            this.stack.push(
                new Float((first as Float).getValue() - (second as Float).getValue())
            );
        }
    }

    private mul([_type]: TypeLiteral[]) {
        const type = stringToType(_type);
        const [first, second] = this.getOpNums(type);
        if (type === VariableType.INT) {
            this.stack.push((first as number) * (second as number));
        } else {
            //float
            this.stack.push(
                new Float((first as Float).getValue() * (second as Float).getValue())
            );
        }
    }

    private div([_type]: TypeLiteral[]) {
        const type = stringToType(_type);
        const [first, second] = this.getOpNums(type);
        if (type === VariableType.INT) {
            this.stack.push(Math.floor((first as number) / (second as number)));
        } else {
            //float
            this.stack.push(
                new Float((first as Float).getValue() / (second as Float).getValue())
            );
        }
    }

    private mod() {
        const [first, second] = this.getOpNums(VariableType.INT);
        this.stack.push((first as number) % (second as number));
    }

    private uminus([_type]: TypeLiteral[]) {
        const type = stringToType(_type);
        const value = this.toNumber(this.pop());
        if (this.getType(value) !== type) {
            throw new Error(
                `Type mismatch: expected ${VariableType[type]}, got ${VariableType[this.getType(value)]}`
            );
        }
        this.stack.push(-value);
    }

    private concat() {
        const first = this.pop();
        const second = this.pop();
        if (typeof first !== 'string' || typeof second !== 'string') {
            throw new Error('Type mismatch: expected string');
        }

        this.stack.push(first + second);
    }

    private getOpBool() {
        const first = this.pop();
        const second = this.pop();
        if (typeof first !== 'boolean' || typeof second !== 'boolean') {
            throw new Error('Type mismatch: expected boolean');
        }

        return [second, first] as const;
    }

    private and() {
        const [first, second] = this.getOpBool();
        this.stack.push(first && second);
    }

    private or() {
        const [first, second] = this.getOpBool();
        this.stack.push(first || second);
    }

    private gt([_type]: TypeLiteral[]) {
        const type = stringToType(_type);
        const [first, second] = this.getOpNums(type);
        this.stack.push(first > second);
    }

    private lt([_type]: TypeLiteral[]) {
        const type = stringToType(_type);
        const [first, second] = this.getOpNums(type);
        this.stack.push(first < second);
    }

    private eq([_type]: TypeLiteral[]) {
        const type = stringToType(_type);
        const [first, second] = this.getOpNums(type);
        this.stack.push(first === second);
    }

    private not() {
        const value = this.pop();
        if (typeof value !== 'boolean') {
            throw new Error('Type mismatch: expected boolean');
        }
        this.stack.push(!value);
    }

    private itof() {
        const value = this.pop();
        if (typeof value !== 'number') {
            throw new Error('Type mismatch: expected number');
        }
        this.stack.push(new Float(value.toString()));
    }

    private parseInput(value: string) {
        if (value === 'true') {
            return true;
        }
        if (value === 'false') {
            return false;
        }
        if (value.includes('.') && !isNaN(parseFloat(value))) {
            return new Float(value);
        }
        if (!isNaN(parseInt(value))) {
            return parseInt(value);
        }
        if (value.startsWith('"') && value.endsWith('"')) {
            return value.slice(1, -1);
        }

        //string fallback
        return value;
    }

    private push([_type]: TypeLiteral[], value: any) {
        const type = stringToType(_type);
        const parsedValue = this.parseInput(value);
        const valueType = this.getType(parsedValue);

        if (type !== valueType) {
            throw new Error(
                `Type mismatch: expected ${VariableType[type]}, got ${VariableType[valueType]}`
            );
        }
        this.stack.push(parsedValue);
    }

    private load([varName]: string[]) {
        this.stack.push(this.variables.get(varName)!);
    }

    private save([varName]: string[]) {
        const value = this.pop();
        this.variables.set(varName, value);
    }

    private label([_labelName]: string[]) { }

    private jmp([labelName]: string[]) {
        const targetIp = this.labels.get(labelName)!;
        this.ip = targetIp;
    }

    private fjmp([labelName]: string[]) {
        const value = this.pop();
        if (typeof value !== 'boolean') {
            throw new Error('Type mismatch: expected boolean');
        }

        if (value === false) {
            this.jmp([labelName]);
        }
    }

    private print([_count]: string[]) {
        const count = parseInt(_count);

        let str = '';

        for (let i = 0; i < count; ++i) {
            let value = this.pop();
            if (value instanceof Float) {
                value = value.getValue();
            }
            str = value + str;
        }

        console.log(str);
    }

    private async read([_type]: TypeLiteral[]) {
        const result = await this.io.question('');
        const type = stringToType(_type);
        const parsed = this.parseInput(result);
        const valueType = this.getType(parsed);

        if (type !== valueType) {
            throw new Error(
                `Type mismatch: expected ${VariableType[type]}, got ${VariableType[valueType]}`
            );
        }

        this.stack.push(parsed);
    }
}
