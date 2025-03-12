import { Epsilon } from './Epsilon';
import { NonTerminal } from './NonTerminal';
import { Rule } from './Rule';
import { Symbol } from './Symbol';
import { Terminal } from './Terminal';

export class Grammar {
    constructor(
        public readonly terminals: Terminal[],
        public readonly nonTerminals: NonTerminal[],
        public readonly rules: Rule[]
    ) { }

    computeNullable(): Set<NonTerminal> {
        const nullable = new Set<NonTerminal>();
        let changed = true;

        while (changed) {
            changed = false;
            for (const rule of this.rules) {
                if (
                    !nullable.has(rule.lhs) &&
                    (rule.rhs.every(
                        (symbol) => symbol instanceof NonTerminal && nullable.has(symbol)
                    ) ||
                        (rule.rhs.length === 1 && rule.rhs[0] instanceof Epsilon))
                ) {
                    nullable.add(rule.lhs);
                    changed = true;
                }
            }
        }

        return nullable;
    }

    getFirstTable(withEpsilon = false) {
        const nullable = this.computeNullable();
        const sets = new Map<NonTerminal, Set<Symbol>>();

        //fill table with basics
        for (const rule of this.rules) {
            if (!sets.has(rule.lhs)) {
                sets.set(rule.lhs, new Set());
            }

            const set = sets.get(rule.lhs)!;
            let walked = 0;

            for (const symbol of rule.rhs) {
                ++walked;
                if (withEpsilon || !(symbol instanceof Epsilon)) {
                    set.add(symbol);
                }
                if (symbol instanceof Terminal) {
                    break;
                }

                //if symbol is nullable, continue to another symbol
                if (nullable.has(symbol)) {
                    continue;
                }

                break;
            }
        }

        //iterativelly fill table
        let changed = true;
        while (changed) {
            changed = false;

            for (const set of sets.values()) {
                for (const symbol of set) {
                    if (symbol instanceof Terminal) continue;

                    const nonTerminalSet = sets.get(symbol)!;
                    //add this nonTerminal's set to the set of the current nonTerminal
                    for (const terminal of nonTerminalSet) {
                        if (!set.has(terminal)) {
                            set.add(terminal);
                            changed = true;
                        }
                    }
                }
            }
        }

        return sets;
    }

    computeFirstSets(): Map<Rule, Set<Terminal>> {
        const nullable = this.computeNullable();
        const sets = this.getFirstTable();

        const first = new Map<Rule, Set<Terminal>>();

        for (const rule of this.rules) {
            const set = new Set<Terminal>();

            let walked = 0;
            for (const symbol of rule.rhs) {
                ++walked;
                if (symbol instanceof Terminal) {
                    set.add(symbol);
                    break;
                }

                for (const _symbol of sets.get(symbol)!) {
                    if (_symbol instanceof NonTerminal) continue;
                    set.add(_symbol);
                }

                if (!nullable.has(symbol)) {
                    break;
                }
            }

            if (
                //if we walk all symbols
                walked === rule.rhs.length &&
                //and the last one is nonterminal, which can be rewritten to epsilon,
                //this means, that all symbols are nullable
                rule.rhs[walked - 1] instanceof NonTerminal &&
                nullable.has(rule.rhs[walked - 1])
            ) {
                set.add(new Epsilon());
            }

            first.set(rule, set);
        }

        //sort
        for (const key of first.keys()) {
            first.set(
                key,
                new Set([...first.get(key)!].sort((a, b) => a.name.localeCompare(b.name)))
            );
        }

        return first;
    }

    computeFollowSets(): Map<NonTerminal, Set<Terminal>> {
        const first = this.getFirstTable(true);
        const sets = new Map<NonTerminal, Set<Symbol>>();
        //first rule is start, so add epsilon to it
        sets.set(this.rules[0].lhs, new Set([new Epsilon()]));

        //fill table with basics
        for (const rule of this.rules) {
            if (!sets.has(rule.lhs)) {
                sets.set(rule.lhs, new Set());
            }

            for (let i = 0; i < rule.rhs.length; ++i) {
                const symbol = rule.rhs[i];
                if (symbol instanceof Terminal) continue;

                if (!sets.has(symbol)) {
                    sets.set(symbol, new Set());
                }

                //look after the symbol
                const next = rule.rhs[i + 1];
                //the nonterminal is at the end
                if (!next) {
                    sets.get(symbol)!.add(rule.lhs);
                    continue;
                }

                if (next instanceof Terminal) {
                    sets.get(symbol)!.add(next);
                    continue;
                }

                //next is nonterminal
                //we need to add first of next to follow of symbol
                for (const _symbol of first.get(next)!) {
                    if (_symbol instanceof NonTerminal) continue;
                    if (_symbol instanceof Epsilon) {
                        //if we can rewrite this nonterminal to epsilon,
                        //we need to add follow of next to follow of symbol
                        sets.get(symbol)!.add(next);
                        continue;
                    }
                    sets.get(symbol)!.add(_symbol);
                }
            }
        }

        let changed = true;
        while (changed) {
            changed = false;

            for (const set of sets.values()) {
                for (const symbol of set) {
                    if (symbol instanceof Terminal) continue;

                    const nonTerminalSet = sets.get(symbol)!;
                    //add this nonTerminal's set to the set of the current nonTerminal
                    for (const terminal of nonTerminalSet) {
                        if (!set.has(terminal)) {
                            set.add(terminal);
                            changed = true;
                        }
                    }
                }
            }
        }

        const follow = new Map<NonTerminal, Set<Terminal>>();
        for (const nonTerminal of this.nonTerminals) {
            follow.set(
                nonTerminal,
                new Set(
                    [...sets.get(nonTerminal)!]
                        .filter((s) => s instanceof Terminal)
                        .sort((a, b) => a.name.localeCompare(b.name))
                )
            );
        }

        return follow;
    }
}
