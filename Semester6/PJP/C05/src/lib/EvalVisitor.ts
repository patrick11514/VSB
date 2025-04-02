import {
    AddContext,
    HexContext,
    IntContext,
    MulContext,
    OctContext,
    ParContext,
    StatContext
} from './Generated/GrammarParser';
import GrammarVisitor from './Generated/GrammarVisitor';

export default class EvalVisitor extends GrammarVisitor<number> {
    visitInt = (ctx: IntContext) => {
        return parseInt(ctx.INT().getText());
    };

    visitOct = (ctx: OctContext) => {
        return parseInt(ctx.OCT().getText(), 8);
    };

    visitHex = (ctx: HexContext) => {
        return parseInt(ctx.HEX().getText(), 16);
    };

    visitPar = (ctx: ParContext) => {
        return this.visit(ctx.expr());
    };

    visitAdd = (ctx: AddContext) => {
        const left = this.visit(ctx.expr(0));
        const right = this.visit(ctx.expr(1));
        if (ctx._op.text === '+') {
            return left + right;
        }
        return left - right;
    };

    visitMul = (ctx: MulContext) => {
        const left = this.visit(ctx.expr(0));
        const right = this.visit(ctx.expr(1));
        if (ctx._op.text === '*') {
            return left * right;
        }
        return left / right;
    };

    visitStat = (ctx: StatContext) => {
        for (const expr of ctx.expr_list()) {
            console.log(this.visit(expr));
        }
        return 0;
    };
}
