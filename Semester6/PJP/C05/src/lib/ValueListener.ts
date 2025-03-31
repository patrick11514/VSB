import ArrayInitListener from './Generated/ArrayInitListener';
import { ValueContext } from './Generated/ArrayInitParser';

export default class ValueListener extends ArrayInitListener {
    exitValue = (ctx: ValueContext) => {
        if (ctx.INT()) {
            console.log(ctx.INT().getText());
        }
    };
}
