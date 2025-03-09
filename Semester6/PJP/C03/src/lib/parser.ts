import { Epsilon } from './Epsilon';
import { Grammar } from './Grammar';
import { NonTerminal } from './NonTerminal';
import { Rule } from './Rule';
import { Terminal } from './Terminal';

export const parseGrammar = (grammar: string): Grammar => {
    const terminals = new Map<string, Terminal>();
    const nonTerminals = new Map<string, NonTerminal>();
    const rules: Rule[] = [];

    const getTerminal = (name: string): Terminal => {
        if (!terminals.has(name)) terminals.set(name, new Terminal(name));
        return terminals.get(name)!;
    };

    const getNonTerminal = (name: string): NonTerminal => {
        if (!nonTerminals.has(name)) nonTerminals.set(name, new NonTerminal(name));
        return nonTerminals.get(name)!;
    };

    const lines = grammar
        .split('\n')
        .map((line) => line.replace(/\{.*?\}/g, '').trim()) //remove comments
        .filter((line) => line.length > 0);

    for (const line of lines) {
        const clearLine = line.replace(/;$/, '').trim(); //remove cemicolon at the end
        const [lhsPart, rhsPart] = clearLine.split(':').map((part) => part.trim());
        const lhs = getNonTerminal(lhsPart);
        const rhsRules = rhsPart.split('|').map((rule) => rule.trim());

        for (const _rule of rhsRules) {
            const rule = new Rule(lhs);
            for (const part of _rule.split(/\s+/)) {
                if (/[A-Z]/.test(part[0])) {
                    rule.rhs.push(getNonTerminal(part));
                } else if (part !== '') {
                    rule.rhs.push(getTerminal(part));
                } else {
                    rule.rhs.push(new Epsilon());
                }
            }
            rules.push(rule);
        }
    }

    return new Grammar(
        [...terminals.values()].sort((a, b) => (a.name > b.name ? 1 : -1)),
        [...nonTerminals.values()].sort((a, b) => (a.name > b.name ? 1 : -1)),
        rules
    );
};
