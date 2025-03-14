import { Cite } from '$/types/types';

export const gen = (citations: Cite[]): string => {
    let final: string[] = [];

    for (const cite of citations) {
        let line = `@${cite.type}{${cite.name},\n`;
        if (cite.author) {
            line += `    author\t\t\t= {${cite.author}},\n`;
        }
        line += `    date\t\t\t= {${cite.date}},\n`;
        line += `    title\t\t\t= {${cite.title}},\n`;
        line += `    lang\t\t\t= {${cite.lang}},\n`;
        switch (cite.lang) {
            case 'english':
                line += `    langidopts\t\t= {variant=american},\n`;
                break;
        }
        line += `    urldate\t\t\t= {${cite.citeDate}},\n`;

        switch (cite.type) {
            case 'online':
                line += `    url\t\t\t\t= {${cite.url}},\n`;
                break;
        }

        line += `}\n`;
        final.push(line);
    }

    return final.join('\n');
};
