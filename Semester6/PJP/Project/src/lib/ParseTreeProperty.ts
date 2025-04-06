import { ParseTree } from 'antlr4';
import { z } from 'zod';

export const startSchema = z.object({
    start: z.object({
        tokenIndex: z.number(),
        line: z.number(),
        column: z.number()
    })
});

export class ParseTreeProperty<$Value> {
    private map: Map<string, $Value> = new Map();

    private getUniqueKey = (node: ParseTree) => {
        let tokenIndex = -1;
        const parsed = startSchema.safeParse(node);
        if (parsed.success) {
            tokenIndex = parsed.data.start.tokenIndex;
        }
        return `${tokenIndex};;${node.getText()}`;
    };

    set(node: ParseTree, value: $Value) {
        this.map.set(this.getUniqueKey(node), value);
    }

    get(node: ParseTree): $Value | undefined {
        return this.map.get(this.getUniqueKey(node));
    }
}
