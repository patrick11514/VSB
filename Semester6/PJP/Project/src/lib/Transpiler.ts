import { z } from 'zod';
import {
    ADDContext,
    ANDContext,
    ASSIGNContext,
    BLOCKContext,
    BOOLContext,
    CMDContext,
    CMDEXPRContext,
    CMDREADContext,
    CMDVARContext,
    CMDWRITEContext,
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
    StartContext,
    STRContext,
    VARIDContext,
    WHILEContext
} from './Generated/ProjectParser';
import ProjectVisitor from './Generated/ProjectVisitor';
import {
    TypeCheckRes,
    TypeLiteral,
    VariableType,
    checkTypes as _checkTypes,
    defaultValueToString,
    getDefaultValue,
    parseType,
    typeToString
} from './utilities';

const ResultSchema = z.object({
    code: z.array(z.string()),
    resultType: z
        .union([
            z.literal(VariableType.INT),
            z.literal(VariableType.FLOAT),
            z.literal(VariableType.BOOL),
            z.literal(VariableType.STRING),
            z.literal(VariableType.NULL)
        ])
        .optional()
});

type VisitResult = {
    code: string[];
    resultType: VariableType;
    pop: boolean;
};

export type TranspilerInput = {
    variableTypes: Map<string, VariableType>;
};

export class Transpiler extends ProjectVisitor<VisitResult> {
    private labelCounter = 0;

    constructor(private _: TranspilerInput) {
        super();
    }

    //MAIN
    private iterate(ctx: StartContext | BLOCKContext) {
        const lines: string[] = [];
        for (const stmt of ctx.statement_list()) {
            const result = this.visit(stmt);
            if (ResultSchema.safeParse(result).success === true) {
                lines.push(...result.code);
                if (result.pop) {
                    lines.push('pop');
                }
            } else {
                console.log(result);
                throw new Error('Invalid result');
            }
        }
        return {
            code: lines,
            resultType: VariableType.NULL,
            pop: false
        };
    }

    visitStart = (ctx: StartContext) => {
        return this.iterate(ctx);
    };

    //Statments
    visitCMD = (ctx: CMDContext) => {
        const cmd = ctx.command();
        if (cmd) {
            return this.visit(ctx.command());
        }
        return this.makeEmptyResult();
    };

    visitBLOCK = (ctx: BLOCKContext) => {
        return this.iterate(ctx);
    };

    private getLabel() {
        return `label ${this.labelCounter++}`;
    }

    visitIF = (ctx: IFContext) => {
        const lines: string[] = [];

        const expr = this.visit(ctx.expr());
        lines.push(...expr.code);
        lines.push(`fjmp ${this.labelCounter}`);

        const statement = this.visit(ctx.statement());
        lines.push(...statement.code);
        if (statement.pop) {
            lines.push('pop');
        }
        lines.push(this.getLabel());

        return {
            code: lines,
            resultType: VariableType.NULL,
            pop: false
        };
    };

    visitIFELSE = (ctx: IFELSEContext) => {
        const lines = [];

        const expr = this.visit(ctx.expr());
        lines.push(...expr.code);
        //false label tzn jump to else
        lines.push(`fjmp ${this.labelCounter}`);
        const ifStmt = this.visit(ctx.statement(0));
        lines.push(...ifStmt.code);
        if (ifStmt.pop) {
            lines.push('pop');
        }
        //end label tzn jump to end
        lines.push(`jmp ${this.labelCounter + 1}`);
        const elseStmt = this.visit(ctx.statement(1));
        //else label
        lines.push(this.getLabel());
        lines.push(...elseStmt.code);
        if (elseStmt.pop) {
            lines.push('pop');
        }
        //end label
        lines.push(this.getLabel());

        return {
            code: lines,
            resultType: VariableType.NULL,
            pop: false
        };
    };

    visitWHILE = (ctx: WHILEContext) => {
        const lines = [];

        const expr = this.visit(ctx.expr());
        //label before expression
        const beforeLabel = this.labelCounter;
        lines.push(this.getLabel());
        lines.push(...expr.code);

        const body = this.visit(ctx.statement());
        //jump to end if false
        lines.push(`fjmp ${this.labelCounter}`);
        lines.push(...body.code);
        if (body.pop) {
            lines.push('pop');
        }
        lines.push(`jmp ${beforeLabel}`); //jump to expression
        //end label
        lines.push(this.getLabel());

        return {
            code: lines,
            resultType: VariableType.NULL,
            pop: false
        };
    };

    visitFOR = (ctx: FORContext) => {
        const lines = [];
        const pre = this.visit(ctx.expr(0));
        lines.push(...pre.code);
        if (pre.pop) {
            lines.push('pop');
        }
        const condition = this.visit(ctx.expr(1));
        const preConditionLabel = this.labelCounter;
        lines.push(this.getLabel());
        lines.push(...condition.code);
        lines.push(`fjmp ${this.labelCounter}`);
        const body = this.visit(ctx.statement());
        lines.push(...body.code);
        if (body.pop) {
            lines.push('pop');
        }
        const increment = this.visit(ctx.expr(2));
        lines.push(...increment.code);
        if (increment.pop) {
            lines.push('pop');
        }
        lines.push(`jmp ${preConditionLabel}`);
        lines.push(this.getLabel());

        return {
            code: lines,
            resultType: VariableType.NULL,
            pop: false
        };
    };

    //commands
    visitCMDEXPR = (ctx: CMDEXPRContext) => {
        return this.visit(ctx.expr());
    };

    visitCMDREAD = (ctx: CMDREADContext) => {
        const lines: string[] = [];

        for (const variable of ctx.VARID_list()) {
            const varType = this._.variableTypes.get(variable.getText());
            const typeLiteral = typeToString(varType!);
            lines.push(`read ${typeLiteral}`);
            lines.push(`save ${variable.getText()}`);
        }

        return {
            code: lines,
            resultType: VariableType.NULL,
            pop: false
        };
    };

    visitCMDWRITE = (ctx: CMDWRITEContext) => {
        const lines: string[] = [];

        for (const expr of ctx.expr_list()) {
            const result = this.visit(expr);
            lines.push(...result.code);
        }

        lines.push(`print ${ctx.expr_list().length}`);

        return {
            code: lines,
            resultType: VariableType.NULL,
            pop: false
        };
    };

    //empty result statements
    private makeEmptyResult(): VisitResult {
        return {
            code: [],
            resultType: VariableType.NULL,
            pop: false
        };
    }

    visitCMDVAR = (ctx: CMDVARContext) => {
        const lines: string[] = [];

        const type = parseType(ctx.vartype().getText());
        const typeLiteral = typeToString(type);

        for (const variable of ctx.VARID_list()) {
            lines.push(`push ${typeLiteral} ${defaultValueToString(type)}`);
            lines.push(`save ${variable.getText()}`);
        }

        return {
            code: lines,
            resultType: VariableType.NULL,
            pop: false
        };
    };

    private singleLineCode(
        code: string,
        other: Partial<Omit<VisitResult, 'code'>> = {}
    ): VisitResult {
        return {
            code: [code],
            resultType: VariableType.NULL,
            pop: true,
            ...other
        };
    }

    //Basic statements
    visitINT = (ctx: INTContext) => {
        return this.singleLineCode(`push I ${ctx.INT().getText()}`, {
            resultType: VariableType.INT
        });
    };

    visitFLOAT = (ctx: FLOATContext) => {
        return this.singleLineCode(`push F ${ctx.FLOAT().getText()}`, {
            resultType: VariableType.FLOAT
        });
    };

    visitSTR = (ctx: STRContext) => {
        return this.singleLineCode(`push S ${ctx.STR().getText()}`, {
            resultType: VariableType.STRING
        });
    };

    visitBOOL = (ctx: BOOLContext) => {
        return this.singleLineCode(`push B ${ctx.BOOL().getText()}`, {
            resultType: VariableType.BOOL
        });
    };

    visitVARID = (ctx: VARIDContext) => {
        return this.singleLineCode(`load ${ctx.VARID().getText()}`, {
            resultType: this._.variableTypes.get(ctx.VARID().getText())
        });
    };

    visitPAREN = (ctx: PARENContext) => {
        return this.visit(ctx.expr());
    };

    visitASSIGN = (ctx: ASSIGNContext) => {
        const name = ctx.VARID().getText();
        const right = ctx.expr();
        const rightResult = this.visit(right);

        const variableType = this._.variableTypes.get(name)!;
        const rightType = rightResult.resultType;
        if (rightType === undefined) {
            throw new Error('Type is undefined');
        }

        this.checkTypes(variableType, rightType, [], rightResult.code);

        return {
            code: [...rightResult.code, `save ${name}`, `load ${name}`],
            resultType: rightResult.resultType,
            pop: true
        };
    };

    private checkTypes<$SRC extends undefined | string[]>(
        aType: VariableType,
        bType: VariableType,
        srcA: $SRC,
        srcB: $SRC
    ): TypeLiteral {
        const result = _checkTypes(aType, bType);
        //if convertable, inject conversion code
        if (
            result === TypeCheckRes.CONVERTABLE &&
            srcA !== undefined &&
            srcB !== undefined
        ) {
            if (aType === VariableType.INT && bType === VariableType.FLOAT) {
                srcA.push('itof');
            } else if (aType === VariableType.FLOAT && bType === VariableType.INT) {
                srcB.push('itof');
            }
        }
        //if mismatch, throw error
        //but it passed the type checker so it should be ok
        if (result === TypeCheckRes.MISMATCH) {
            throw new Error(
                `Type mismatch: ${typeToString(aType)} and ${typeToString(bType)}`
            );
        }

        if (result === TypeCheckRes.OK) {
            return typeToString(aType);
        }

        //here we should return only FLOAT since its CONVERTABLE
        return typeToString(VariableType.FLOAT);
    }

    private checkTypeFromResult(a: VisitResult, b: VisitResult) {
        if (a.resultType === undefined || b.resultType === undefined) {
            throw new Error('Type is undefined');
        }

        return this.checkTypes(a.resultType, b.resultType, a.code, b.code);
    }

    visitADD = (ctx: ADDContext) => {
        const lines: string[] = [];
        const left = this.visit(ctx.expr(0));

        const right = this.visit(ctx.expr(1));

        const typeLiteral = this.checkTypeFromResult(left, right);
        lines.push(...left.code, ...right.code);

        switch (ctx._op.text) {
            case '.':
                lines.push('concat');
                break;
            case '+':
                lines.push(`add ${typeLiteral}`);
                break;
            case '-':
                lines.push(`sub ${typeLiteral}`);
                break;
        }

        return {
            code: lines,
            resultType: left.resultType,
            pop: true
        };
    };

    visitMUL = (ctx: MULContext) => {
        const lines: string[] = [];
        const left = this.visit(ctx.expr(0));
        const right = this.visit(ctx.expr(1));
        const typeLiteral = this.checkTypeFromResult(left, right);

        lines.push(...left.code, ...right.code);

        switch (ctx._op.text) {
            case '*':
                lines.push(`mul ${typeLiteral}`);
                break;
            case '/':
                lines.push(`div ${typeLiteral}`);
                break;
            case '%':
                lines.push(`mod`);
                break;
        }

        return {
            code: lines,
            resultType: left.resultType,
            pop: true
        };
    };

    visitMINUS = (ctx: MINUSContext) => {
        const lines: string[] = [];
        const right = this.visit(ctx.expr());

        if (right.resultType === undefined) {
            throw new Error('Type is undefined');
        }
        const typeLiteral = typeToString(right.resultType);

        lines.push(...right.code);
        lines.push(`uminus ${typeLiteral}`);

        return {
            code: lines,
            resultType: right.resultType,
            pop: true
        };
    };

    visitNOT = (ctx: NOTContext) => {
        const lines = [];
        const right = this.visit(ctx.expr());

        if (right.resultType === undefined) {
            throw new Error('Type is undefined');
        }

        lines.push(...right.code);
        lines.push(`not`);

        return {
            code: lines,
            resultType: right.resultType,
            pop: true
        };
    };

    visitREL = (ctx: RELContext) => {
        const lines = [];
        const left = this.visit(ctx.expr(0));
        const right = this.visit(ctx.expr(1));

        const typeLiteral = this.checkTypeFromResult(left, right);

        lines.push(...left.code, ...right.code);

        if (ctx._op.text === '>') {
            lines.push(`gt ${typeLiteral}`);
        } else {
            lines.push(`lt ${typeLiteral}`);
        }

        return {
            code: lines,
            resultType: VariableType.BOOL,
            pop: true
        };
    };

    visitAND = (ctx: ANDContext) => {
        const lines = [];
        const left = this.visit(ctx.expr(0));
        const right = this.visit(ctx.expr(1));

        lines.push(...left.code, ...right.code, 'and');

        return {
            code: lines,
            resultType: VariableType.BOOL,
            pop: true
        };
    };

    visitOR = (ctx: ORContext) => {
        const lines = [];
        const left = this.visit(ctx.expr(0));
        const right = this.visit(ctx.expr(1));

        lines.push(...left.code, ...right.code, 'or');

        return {
            code: lines,
            resultType: VariableType.BOOL,
            pop: true
        };
    };

    visitEQUAL = (ctx: EQUALContext) => {
        const lines = [];
        const left = this.visit(ctx.expr(0));
        const right = this.visit(ctx.expr(1));

        const typeLiteral = this.checkTypeFromResult(left, right);

        lines.push(...left.code, ...right.code);

        lines.push(`eq ${typeLiteral}`);

        if (ctx._op.text === '!=') {
            lines.push('not');
        }

        return {
            code: lines,
            resultType: VariableType.BOOL,
            pop: true
        };
    };
}
