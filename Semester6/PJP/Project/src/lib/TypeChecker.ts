import { Float } from './Float';
import ProjectListener from './Generated/ProjectListener';
import {
    ADDContext,
    ANDContext,
    ASSIGNContext,
    BOOLContext,
    CMDVARContext,
    EQUALContext,
    FLOATContext,
    FORContext,
    IFContext,
    IFELSEContext,
    INTContext,
    MINUSContext,
    MULContext,
    NOTContext,
    ORContext,
    PARENContext,
    RELContext,
    STRContext,
    // TERNARYContext,
    VARIDContext,
    WHILEContext
} from './Generated/ProjectParser';
import { ParseTreeProperty } from './ParseTreeProperty';
import { TranspilerInput } from './Transpiler';
import { TypeError } from './TypeError';
import {
    checkTypes as _checkTypes,
    getDefaultValue,
    parseType,
    TypeCheckRes,
    unParseType,
    VariableType,
    VarType
} from './utilities';

export class TypeChecker extends ProjectListener {
    private variableTypes = new Map<string, VariableType>();
    private variableContent = new Map<string, VarType | null>();
    private variableDecInfo = new Map<
        string,
        {
            line: number;
            char: number;
            len: number;
        }
    >();

    private values = new ParseTreeProperty<VarType | null>();

    errors: TypeError[] = [];

    getTranspilerInput() {
        return {
            variableTypes: this.variableTypes
        } satisfies TranspilerInput;
    }

    private getType(varNameOfLiteral: VarType): VariableType {
        if (typeof varNameOfLiteral === 'string') {
            if (this.variableTypes.has(varNameOfLiteral)) {
                return this.variableTypes.get(varNameOfLiteral)!;
            }
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

    private checkTypes(a: VarType, b: VarType): TypeCheckRes {
        const aType = this.getType(a);
        const bType = this.getType(b);
        return _checkTypes(aType, bType);
    }

    private checkEq(
        ctx: MULContext | ADDContext | RELContext | EQUALContext | ANDContext | ORContext,
        invalidTypes: VariableType[],
        valid: string,
        _op: string = ''
    ) {
        const left = this.values.get(ctx.expr(0))!;
        const right = this.values.get(ctx.expr(1))!;

        //cannot apply to string or boolean
        const leftType = this.getType(left);
        const rightType = this.getType(right);

        if (invalidTypes.includes(leftType) || invalidTypes.includes(rightType)) {
            let op = '_op' in ctx ? ctx._op.text : _op;
            this.errors.push(
                new TypeError(
                    {
                        line: ctx.start.line,
                        char: ctx.start.column
                    },
                    'type mismatch',
                    `Cannot apply '${op}' to type '${unParseType(leftType)}' and '${unParseType(rightType)}'`,
                    [
                        {
                            start: {
                                line: ctx.start.line,
                                char: ctx.start.column
                            },
                            end: {
                                line: ctx.start.line,
                                char: ctx.start.column + ctx.getText().length
                            },
                            message: `Valid types for '${op}' are: ${valid}`
                        }
                    ]
                )
            );
            return true;
        }
        return false;
    }

    exitBOOL = (ctx: BOOLContext) => {
        this.values.set(ctx, ctx.BOOL().getText() === 'true');
    };

    exitINT = (ctx: INTContext) => {
        this.values.set(ctx, parseInt(ctx.INT().getText()));
    };

    exitFLOAT = (ctx: FLOATContext) => {
        this.values.set(ctx, new Float(ctx.FLOAT().getText()));
    };

    exitSTR = (ctx: STRContext) => {
        this.values.set(ctx, ctx.STR().getText());
    };

    exitCMDVAR = (ctx: CMDVARContext) => {
        const type = parseType(ctx.vartype().getText());
        const defValue = getDefaultValue(type);

        let idx = 0;
        for (const variable of ctx.VARID_list()) {
            if (this.variableTypes.has(variable.getText())) {
                const prevVarData = this.variableDecInfo.get(variable.getText())!;

                const varStart =
                    ctx.start.column + ctx.getText().search(variable.getText()) + 1;
                this.errors.push(
                    new TypeError(
                        {
                            line: ctx.start.line,
                            char: ctx.start.column
                        },
                        'variable already declared',
                        `Variable '${variable.getText()}' already declared`,
                        [
                            {
                                start: {
                                    line: ctx.start.line,
                                    char: varStart
                                },
                                end: {
                                    line: ctx.start.line,
                                    char: varStart + variable.getText().length
                                },
                                message: `Variable '${variable.getText()}' declared here`
                            },
                            {
                                start: {
                                    line: prevVarData.line,
                                    char: prevVarData.char
                                },
                                end: {
                                    line: prevVarData.line,
                                    char: prevVarData.char + prevVarData.len
                                },
                                message: 'but it was already declared here'
                            }
                        ]
                    )
                );
                continue;
            }

            this.variableTypes.set(variable.getText(), type);
            this.variableContent.set(variable.getText(), defValue);
            this.variableDecInfo.set(variable.getText(), {
                line: ctx.start.line,
                char: ctx.getText().search(variable.getText()) + ctx.start.column + 1,
                len: variable.getText().length
            });
            ++idx;
        }
    };

    exitPAREN = (ctx: PARENContext) => {
        this.values.set(ctx, this.values.get(ctx.expr())!);
    };

    exitVARID = (ctx: VARIDContext) => {
        this.values.set(ctx, this.variableContent.get(ctx.getText())!);
    };

    exitMINUS = (ctx: MINUSContext) => {
        const value = this.values.get(ctx.expr())!;
        const type = this.getType(value);

        if (type !== VariableType.INT && type !== VariableType.FLOAT) {
            this.errors.push(
                new TypeError(
                    {
                        line: ctx.start.line,
                        char: ctx.start.column
                    },
                    'type mismatch',
                    `Cannot apply '-' to type '${unParseType(type)}'`,
                    [
                        {
                            start: {
                                line: ctx.start.line,
                                char: ctx.start.column
                            },
                            end: {
                                line: ctx.start.line,
                                char: ctx.start.column + ctx.getText().length
                            },
                            message: `Trying to apply '-' to '${unParseType(type)}'`
                        }
                    ]
                )
            );
            return;
        }
        this.values.set(ctx, (value as number) * -1);
    };

    exitASSIGN = (ctx: ASSIGNContext) => {
        const left = ctx.VARID();
        const right = ctx.expr();

        const value = this.values.get(right)!;
        const rightType = this.getType(value);
        const leftType = this.variableTypes.get(left.getText());

        if (leftType === undefined) {
            this.errors.push(
                new TypeError(
                    {
                        line: ctx.start.line,
                        char: ctx.start.column
                    },
                    'variable not declared',
                    `Variable '${left.getText()}' not declared`,
                    [
                        {
                            start: {
                                line: ctx.start.line,
                                char: ctx.start.column
                            },
                            end: {
                                line: ctx.start.line,
                                char: ctx.start.column + left.getText().length
                            },
                            message: `Variable '${left.getText()}' not declared`
                        }
                    ]
                )
            );
            return;
        }

        const compare = this.checkTypes(left.getText(), value);
        if (
            compare === TypeCheckRes.MISMATCH ||
            (leftType === VariableType.INT &&
                rightType === VariableType.FLOAT) /* because we can't cut float */
        ) {
            this.errors.push(
                new TypeError(
                    {
                        line: ctx.start.line,
                        char: ctx.start.column
                    },
                    'type mismatch',
                    `Cannot assign type '${unParseType(rightType)}' to type '${unParseType(leftType)}'`,
                    [
                        {
                            start: {
                                line: right.start.line,
                                char: right.start.column
                            },
                            end: {
                                line: right.start.line,
                                char: right.start.column + right.getText().length
                            },
                            message: `Trying to assign '${unParseType(rightType)}'`
                        },
                        {
                            start: {
                                line: ctx.start.line,
                                char: ctx.start.column
                            },
                            end: {
                                line: ctx.start.line,
                                char: ctx.start.column + left.getText().length
                            },
                            message: `while this is type of '${unParseType(leftType)}'`
                        }
                    ]
                )
            );
            return;
        }

        this.variableContent.set(left.getText(), value);
        this.values.set(ctx, value);
    };

    exitNOT = (ctx: NOTContext) => {
        const value = this.values.get(ctx.expr())!;
        const type = this.getType(value);
        if (type !== VariableType.BOOL) {
            this.errors.push(
                new TypeError(
                    {
                        line: ctx.start.line,
                        char: ctx.start.column
                    },
                    'type mismatch',
                    `Cannot apply '!' to type '${unParseType(type)}'`,
                    [
                        {
                            start: {
                                line: ctx.start.line,
                                char: ctx.start.column
                            },
                            end: {
                                line: ctx.start.line,
                                char: ctx.start.column + ctx.getText().length
                            },
                            message: `Trying to apply '!' to '${unParseType(type)}'`
                        }
                    ]
                )
            );
            return;
        }

        this.values.set(ctx, !value);
    };

    exitMUL = (ctx: MULContext) => {
        const left = this.values.get(ctx.expr(0))!;
        const right = this.values.get(ctx.expr(1))!;

        if (ctx._op.text === '%') {
            const invalidTypes = [
                VariableType.STRING,
                VariableType.BOOL,
                VariableType.FLOAT
            ];
            if (this.checkEq(ctx, invalidTypes, "'int'")) return;
            const first = left as number;
            const second = right as number;

            this.values.set(ctx, first % second);
            return;
        }

        const invalidTypes = [VariableType.STRING, VariableType.BOOL];

        if (this.checkEq(ctx, invalidTypes, "'int' and 'float'")) return;

        let floatResult = false;

        let first = left as number | Float;
        if (first instanceof Float) {
            first = first.getValue();
            floatResult = true;
        }
        let second = right as number | Float;
        if (second instanceof Float) {
            second = second.getValue();
            floatResult = true;
        }

        let res: number | Float;
        if (ctx._op.text === '*') {
            res = first * second;
        } else {
            res = first / second;
        }

        if (floatResult) {
            res = new Float(res.toString());
        }

        this.values.set(ctx, res);
    };

    exitADD = (ctx: ADDContext) => {
        const left = this.values.get(ctx.expr(0))!;
        const right = this.values.get(ctx.expr(1))!;

        if (ctx._op.text === '.') {
            const invalidTypes = [
                VariableType.INT,
                VariableType.FLOAT,
                VariableType.BOOL
            ];
            if (this.checkEq(ctx, invalidTypes, "'string'")) return;

            this.values.set(ctx, `${left}${right}`);
            return;
        }

        let floatResult = false;

        let first = left as number | Float;
        if (first instanceof Float) {
            first = first.getValue();
            floatResult = true;
        }
        let second = right as number | Float;
        if (second instanceof Float) {
            second = second.getValue();
            floatResult = true;
        }

        const invalidTypes = [VariableType.STRING, VariableType.BOOL];
        if (this.checkEq(ctx, invalidTypes, "'int' and 'float'")) return;

        let res: number | Float;
        if (ctx._op.text === '+') {
            res = first + second;
        } else {
            res = first - second;
        }

        if (floatResult) {
            res = new Float(res.toString());
        }

        this.values.set(ctx, res);
    };

    exitREL = (ctx: RELContext) => {
        const left = this.values.get(ctx.expr(0))!;
        const right = this.values.get(ctx.expr(1))!;

        const invalidTypes = [VariableType.STRING, VariableType.BOOL];
        if (this.checkEq(ctx, invalidTypes, "'int' and 'float'")) return;

        let first = left as number | Float;
        if (first instanceof Float) {
            first = first.getValue();
        }
        let second = right as number | Float;
        if (second instanceof Float) {
            second = second.getValue();
        }

        if (ctx._op.text === '<') {
            this.values.set(ctx, first < second);
            return;
        }

        this.values.set(ctx, first > second);
    };

    exitEQUAL = (ctx: EQUALContext) => {
        const left = this.values.get(ctx.expr(0))!;
        const right = this.values.get(ctx.expr(1))!;

        const invalidTypes = [VariableType.BOOL];
        if (this.checkEq(ctx, invalidTypes, "'string', 'float' and 'int'")) return;

        const leftType = this.getType(left);
        const rightType = this.getType(right);
        const comp = this.checkTypes(left, right);
        if (comp === TypeCheckRes.MISMATCH) {
            this.errors.push(
                new TypeError(
                    {
                        line: ctx.start.line,
                        char: ctx.start.column
                    },
                    'type mismatch',
                    `Cannot apply '${ctx._op.text}' to type '${unParseType(leftType)}' and '${unParseType(rightType)}'`,
                    [
                        {
                            start: {
                                line: ctx.start.line,
                                char: ctx.start.column
                            },
                            end: {
                                line: ctx.start.line,
                                char: ctx.start.column + ctx.getText().length
                            },
                            message: `Trying to apply '${ctx._op.text}' to '${unParseType(leftType)}' and '${unParseType(rightType)}'`
                        }
                    ]
                )
            );
            return;
        }

        let first = left as number | Float | string;
        if (first instanceof Float) {
            first = first.getValue();
        }
        let second = right as number | Float | string;
        if (second instanceof Float) {
            second = second.getValue();
        }

        if (ctx._op.text === '==') {
            this.values.set(ctx, left === right);
            return;
        }

        this.values.set(ctx, left !== right);
    };

    exitAND = (ctx: ANDContext) => {
        const left = this.values.get(ctx.expr(0))!;
        const right = this.values.get(ctx.expr(1))!;

        const invalidTypes = [VariableType.STRING, VariableType.INT, VariableType.FLOAT];
        if (this.checkEq(ctx, invalidTypes, "'bool'", '&&')) return;

        this.values.set(ctx, left && right);
    };

    exitOR = (ctx: ORContext) => {
        const left = this.values.get(ctx.expr(0))!;
        const right = this.values.get(ctx.expr(1))!;

        const invalidTypes = [VariableType.STRING, VariableType.INT, VariableType.FLOAT];
        if (this.checkEq(ctx, invalidTypes, "'bool'", '||')) return;

        this.values.set(ctx, left || right);
    };

    private checkBool(
        ctx: IFContext | IFELSEContext | WHILEContext /*| TERNARYContext*/,
        cmd: string
    ) {
        const condition = ctx.expr(/*0*/);
        const value = this.values.get(condition)!;
        const type = this.getType(value);
        if (type != VariableType.BOOL) {
            this.errors.push(
                new TypeError(
                    {
                        line: ctx.start.line,
                        char: ctx.start.column
                    },
                    'type mismatch',
                    `Cannot apply '${cmd}' to type '${unParseType(type)}'`,
                    [
                        {
                            start: {
                                line: condition.start.line,
                                char: condition.start.column
                            },
                            end: {
                                line: condition.start.line,
                                char: condition.start.column + condition.getText().length
                            },
                            message: `Valid type for '${cmd}' is: "bool"`
                        }
                    ]
                )
            );
            return;
        }
    }

    exitIF = (ctx: IFContext) => {
        this.checkBool(ctx, 'if');
    };

    exitIFFELSE = (ctx: IFELSEContext) => {
        this.checkBool(ctx, 'if');
    };

    exitWHILE = (ctx: WHILEContext) => {
        this.checkBool(ctx, 'while');
    };

    /* exitTERNARY = (ctx: TERNARYContext) => {
         this.checkBool(ctx, '? :');
         const condition = ctx.expr(0);
 
         const trueIsh = ctx.expr(1);
         const trueIshValue = this.values.get(trueIsh)!;
         const trueIshType = this.getType(trueIshValue);
 
         const falseIsh = ctx.expr(2);
         const falseIshValue = this.values.get(falseIsh)!;
         const falseIshType = this.getType(falseIshValue);
 
         const compare = this.checkTypes(trueIshValue, falseIshValue);
 
         if (compare === TypeCheckRes.MISMATCH) {
             this.errors.push(
                 new TypeError(
                     {
                         line: ctx.start.line,
                         char: ctx.start.column
                     },
                     'type mismatch',
                     `Cannot combine type '${unParseType(trueIshType)}' and type '${unParseType(falseIshType)}'`,
                     [
                         {
                             start: {
                                 line: falseIsh.start.line,
                                 char: falseIsh.start.column
                             },
                             end: {
                                 line: falseIsh.start.line,
                                 char: falseIsh.start.column + falseIsh.getText().length
                             },
                             message: `while this is type of '${unParseType(falseIshType)}'`
                         },
                         {
                             start: {
                                 line: trueIsh.start.line,
                                 char: trueIsh.start.column
                             },
                             end: {
                                 line: trueIsh.start.line,
                                 char: trueIsh.start.column + trueIsh.getText().length
                             },
                             message: `This is type of '${unParseType(trueIshType)}'`
                         }
                     ]
                 )
             );
             return;
         }
 
         const cond = this.values.get(condition)! as boolean;
         if (cond) {
             this.values.set(ctx, this.values.get(trueIsh)!);
         } else {
             this.values.set(ctx, this.values.get(falseIsh)!);
         }
     };*/
    exitFOR = (ctx: FORContext) => {
        const condition = ctx.expr(1);
        const value = this.values.get(condition)!;
        const type = this.getType(value);
        if (type !== VariableType.BOOL) {
            this.errors.push(
                new TypeError(
                    {
                        line: ctx.start.line,
                        char: ctx.start.column
                    },
                    'type mismatch',
                    `Condition in for cycle need to return 'bool'`,
                    [
                        {
                            start: {
                                line: condition.start.line,
                                char: condition.start.column
                            },
                            end: {
                                line: condition.start.line,
                                char: condition.start.column + condition.getText().length
                            },
                            message: `Valid type for for cycle condition is 'bool'`
                        }
                    ]
                )
            );
            return;
        }
    };
}
