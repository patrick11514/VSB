import { ValueContext } from './Generated/ArrayInitParser';
import ArrayInitVisitor from './Generated/ArrayInitVisitor';

export default class TransformVisitor extends ArrayInitVisitor<string> {
    visitValue = (ctx: ValueContext) => {
        if (ctx.INT()) {
            return ctx.INT().getText();
        }
        return this.visit(ctx.init());
    };
}
