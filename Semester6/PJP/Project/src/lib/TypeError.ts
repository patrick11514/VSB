import clc from 'cli-color';
import bare from 'cli-color/bare';

type Location = {
    line: number;
    char: number;
};

type Message = {
    start: Location;
    end: Location;
    message: string;
};

const repeat = (ch: string, times: number) => {
    return Array.from({ length: times })
        .map(() => ch)
        .join('');
};

const getDigits = (num: number) => {
    let digs = 1;
    while (num > 10) {
        digs++;
        num = num / 10;
    }
    return digs;
};

const colorPalette = [clc.cyan, clc.yellow, clc.magenta, clc.blue];

export class TypeError {
    static fileName: string;
    static sourceCode: string[] = [];
    private messages: (Message & {
        color: bare.Format;
    })[] = [];

    constructor(
        private location: Location,
        private short: string,
        private long: string,
        messages: Message[]
    ) {
        this.messages = messages.map((msg, idx) => ({
            ...msg,
            color: colorPalette[idx % colorPalette.length]
        }));
    }

    toString() {
        const len = 45 + TypeError.fileName.length;
        const lines: string[] = [
            `╭─ ${clc.red('●')} ${clc.yellow('●')} ${clc.green('●')} ───────────────── ${TypeError.fileName} ──────────────────`,
            `│  ${clc.red(`!! [${this.location.line}:${this.location.char}] ${this.short}: ${this.long}`)}`,
            `├` + repeat('─', len)
        ];

        //print 2 lines before + line with error
        let line = this.location.line - 2;
        const digits = getDigits(TypeError.sourceCode.length);

        const putLine = (n: number | string, text: string) => {
            lines.push(`│ ${n.toString().padStart(digits, ' ')} │ ${text}`);
        };

        for (let i = 0; i < 2; ++i) {
            if (line < 0) break;
            putLine(line, TypeError.sourceCode[line - 1]);
            line++;
        }

        //print messages
        const sorted = this.messages.toSorted((a, b) => {
            if (a.start.line === b.start.line) {
                return a.start.char - b.start.char;
            }
            return a.start.line - b.start.line;
        });

        const firstLine = sorted[0].start.line;
        const lastLine = sorted[sorted.length - 1].end.line;

        //print lines with messages
        for (let i = firstLine; i <= lastLine; ++i) {
            putLine(line, TypeError.sourceCode[i - 1]);
            ++line;

            const messages = sorted.filter((msg) => msg.start.line === i);
            if (messages.length === 0) continue;

            const centers: Record<
                string,
                {
                    color: bare.Format;
                    char: number;
                }
            > = {};
            {
                let line = '';
                let char = 0;

                //print bays under the line
                for (const msgIdx in messages) {
                    const message = messages[msgIdx];

                    line += repeat(' ', message.start.char - char);
                    const startChar = message.start.char;
                    const endChar =
                        message.end.line === i
                            ? message.end.char
                            : TypeError.sourceCode[i - 1].length;

                    // Calculate the length of the pointer
                    const length = endChar - startChar;
                    let pointer = '';
                    let centerLocOffset = 0;

                    if (length == 1) {
                        // For a single character or a very small range
                        pointer = message.color('┬');
                    } else if (length == 2) {
                        // For a two-character range, just place ┬─
                        pointer = message.color('┬') + message.color('─');
                    } else {
                        // For longer ranges, create the full pointer
                        const half = (length - 2) / 2;
                        pointer = message.color('╰');
                        pointer += repeat(message.color('─'), Math.floor(half));
                        pointer += message.color('┬');
                        centerLocOffset = Math.floor(half) + 1;
                        pointer += repeat(message.color('─'), Math.ceil(half));
                        pointer += message.color('╯');
                    }

                    // Update the line with the pointer
                    line += pointer;

                    // Store the position of the center of the pointer
                    centers[message.start.char + ';' + message.start.line] = {
                        color: message.color,
                        char: startChar + centerLocOffset
                    };
                    char = message.end.char;
                }

                putLine('⸾', line);
            }
            {
                //put line with one spacing
                const line = Array.from({
                    length: TypeError.sourceCode[i - 1].length
                }).map(() => ' ');
                let l = 0;
                for (const centerIdx in centers) {
                    const center = centers[centerIdx];
                    line[center.char] = center.color('│');
                    ++l;
                }
                putLine('⸾', line.join(''));
            }

            //print real messages
            const msgs = this.messages.filter((msg) => msg.start.line === i);
            for (const msgIdx in msgs) {
                const message = msgs[msgIdx];
                let line = message.color(`╰ ${message.message}`);

                //fill line with spaces in the beginning with spaces
                line =
                    repeat(
                        ' ',
                        centers[message.start.char + ';' + message.start.line].char
                    ) + line;

                let arrLine = Array.from(line);
                for (const [key, value] of Object.entries(centers)) {
                    if (
                        parseInt(key) != message.start.char &&
                        value.char < message.end.char
                    ) {
                        arrLine[value.char] = value.color('│');
                    }
                }

                putLine('⸾', arrLine.join(''));
            }
        }

        //print 2 lines after
        for (let i = 0; i < 2; ++i) {
            if (TypeError.sourceCode[line - 1] === undefined) break;
            putLine(line, TypeError.sourceCode[line - 1]);
            line++;
        }

        lines.push('╰' + repeat('─', len));
        return lines.join('\n');
    }
}
