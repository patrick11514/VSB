import GrammarListener from './Generated/GrammarListener';
import {
    AddContext,
    HexContext,
    IntContext,
    MulContext,
    OctContext,
    ParContext,
    StatContext
} from './Generated/GrammarParser';
import { ParseTreeProperty } from './ParseTreeProperty';

export default class EvalListener extends GrammarListener {
    private values = new ParseTreeProperty<number>();

    exitInt = (ctx: IntContext) => {
        this.values.set(ctx, parseInt(ctx.INT().getText()));
    };

    exitOct = (ctx: OctContext) => {
        this.values.set(ctx, parseInt(ctx.OCT().getText(), 8));
    };

    exitHex = (ctx: HexContext) => {
        this.values.set(ctx, parseInt(ctx.HEX().getText(), 16));
    };

    exitAdd = (ctx: AddContext) => {
        const left = this.values.get(ctx.expr_list()[0])!;
        const right = this.values.get(ctx.expr_list()[1])!;

        if (ctx._op.text === '+') {
            return this.values.set(ctx, left + right);
        }
        this.values.set(ctx, left - right);
    };

    exitMul = (ctx: MulContext) => {
        const left = this.values.get(ctx.expr_list()[0])!;
        const right = this.values.get(ctx.expr_list()[1])!;

        if (ctx._op.text === '*') {
            return this.values.set(ctx, left * right);
        }
        this.values.set(ctx, left / right);
    };

    exitPar = (ctx: ParContext) => {
        this.values.set(ctx, this.values.get(ctx.expr())!);
    };

    exitStat = (ctx: StatContext) => {
        for (const expr of ctx.expr_list()) {
            console.log(this.values.get(expr));
        }
    };
}
