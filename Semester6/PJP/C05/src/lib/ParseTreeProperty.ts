import { ParseTree } from 'antlr4';

export class ParseTreeProperty<$Value> {
    private map: Map<string, $Value> = new Map();

    private getUniqueKey = (node: ParseTree) => {
        let tokenIndex = -1;
        if (
            'start' in node &&
            typeof node.start === 'object' &&
            node.start !== null &&
            'tokenIndex' in node.start &&
            typeof node.start.tokenIndex === 'number'
        ) {
            tokenIndex = node.start.tokenIndex;
        }
        return `${tokenIndex};;${node.constructor.name};;${node.getText()}`;
    };

    set(node: ParseTree, value: $Value) {
        this.map.set(this.getUniqueKey(node), value);
    }

    get(node: ParseTree): $Value | undefined {
        return this.map.get(this.getUniqueKey(node));
    }
}
