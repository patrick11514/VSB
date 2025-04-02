import { ParseTree } from 'antlr4';
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

export default class EvalListener extends GrammarListener {
    value = new Map<ParseTree, number>();

    exitInt = (ctx: IntContext) => {
        this.value.set(ctx, parseInt(ctx.INT().getText()));
    };

    exitOct = (ctx: OctContext) => {
        this.value.set(ctx, parseInt(ctx.OCT().getText(), 8));
    };

    exitHex = (ctx: HexContext) => {
        this.value.set(ctx, parseInt(ctx.HEX().getText(), 16));
    };

    exitAdd = (ctx: AddContext) => {
        const left = this.value.get(ctx.expr_list()[0])!;
        const right = this.value.get(ctx.expr_list()[1])!;

        if (ctx._op.text === '+') {
            return left + right;
        }
        return left - right;
    };

    exitMul = (ctx: MulContext) => {
        const left = this.value.get(ctx.expr_list()[0])!;
        const right = this.value.get(ctx.expr_list()[1])!;

        if (ctx._op.text === '*') {
            return left * right;
        }
        return left / right;
    };

    exitPar = (ctx: ParContext) => {
        this.value.set(ctx, this.value.get(ctx.expr())!);
    };

    exitStat = (ctx: StatContext) => {
        console.log(this.value.values());
        for (const expr of ctx.expr_list()) {
            console.log(this.value.get(expr));
        }
    };
}
