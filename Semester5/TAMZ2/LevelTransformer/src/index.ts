import Logger from './lib/logger';
import fs from 'node:fs';
//logger for main messages
const l = new Logger('MyCoolApp', 'cyan');

const lines = fs.readFileSync('Levels.txt', 'utf8').split('\n');

const final: string[] = [];

let width = 0;
let height = 0;
let level: string[] = [];

for (const line of lines) {
    if (line.startsWith('Level')) {
        width = 0;
        height = 0;
        level = [];
        continue;
    }
    if (line.startsWith(';') || line.startsWith("'")) {
        continue;
    }

    if (line.length == 0) {
        //end;
        final.push(`${width} ${height}`);
        final.push(
            ...level.map(
                (level) =>
                    level +
                    Array.from({ length: width - level.length })
                        .map(() => '0')
                        .join('')
            )
        );
        continue;
    }

    if (line.length > width) width = line.length;
    height++;

    level.push(line.replaceAll(' ', '0').replaceAll('#', '1').replaceAll('$', '2').replaceAll('.', '3').replaceAll('*', '5').replaceAll('@', '4').replaceAll('+', '6'));
}

fs.writeFileSync('out.txt', final.join('\n'));
