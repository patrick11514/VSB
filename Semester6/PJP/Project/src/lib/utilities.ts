import { Float } from './Float';

export enum VariableType {
    INT,
    FLOAT,
    BOOL,
    STRING,
    NULL
}

export const parseType = (type: string): VariableType => {
    return {
        int: VariableType.INT,
        float: VariableType.FLOAT,
        bool: VariableType.BOOL,
        string: VariableType.STRING
    }[type]!;
};

export const unParseType = (type: VariableType): string => {
    return {
        [VariableType.INT]: 'int',
        [VariableType.FLOAT]: 'float',
        [VariableType.BOOL]: 'bool',
        [VariableType.STRING]: 'string',
        [VariableType.NULL]: 'null'
    }[type]!;
};

export const getDefaultValue = (type: VariableType) => {
    switch (type) {
        case VariableType.STRING:
            return '';
        case VariableType.INT:
            return 0;
        case VariableType.FLOAT:
            return 0.0;
        case VariableType.BOOL:
            return false;
        case VariableType.NULL:
            return null;
    }
};

export const defaultValueToString = (type: VariableType) => {
    switch (type) {
        case VariableType.STRING:
            return '""';
        case VariableType.INT:
            return '0';
        case VariableType.FLOAT:
            return '0.0';
        case VariableType.BOOL:
            return 'false';
        case VariableType.NULL:
            return 'null';
    }
};

export type VarType = string | number | Float | boolean;

export enum TypeCheckRes {
    OK,
    CONVERTABLE,
    MISMATCH
}

export const checkTypes = (aType: VariableType, bType: VariableType): TypeCheckRes => {
    const typeConv = [VariableType.INT, VariableType.FLOAT];

    if (aType === bType) return TypeCheckRes.OK;
    if (typeConv.includes(aType) && typeConv.includes(bType))
        return TypeCheckRes.CONVERTABLE;
    return TypeCheckRes.MISMATCH;
};

export type TypeLiteral = 'I' | 'B' | 'S' | 'F' | 'N';

export const typeToString = (type: VariableType): TypeLiteral => {
    return (
        {
            [VariableType.INT]: 'I',
            [VariableType.FLOAT]: 'F',
            [VariableType.BOOL]: 'B',
            [VariableType.STRING]: 'S',
            [VariableType.NULL]: 'N'
        } as const
    )[type];
};

export const stringToType = (type: TypeLiteral): VariableType => {
    return (
        {
            I: VariableType.INT,
            F: VariableType.FLOAT,
            B: VariableType.BOOL,
            S: VariableType.STRING,
            N: VariableType.NULL
        } as const
    )[type];
};
