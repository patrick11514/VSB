import { Cite } from '$/types/types';

export const gen = (citations: Cite[]): string => {
    let final: string[] = [];

    for (const cite of citations) {
        let line = `@${cite.type}{${cite.name},\n`;

        if (cite.author) {
            line += `    author          = {${cite.author}},\n`;
        }

        line += `    title           = {${cite.title.replaceAll('&', '\\&')}},\n`;
        line += `    lang            = {${cite.lang}},\n`;

        if (cite.lang === 'english') {
            line += `    langidopts      = {variant=american},\n`;
        }

        switch (cite.type) {
            case 'online':
                line += `    date            = {${cite.date}},\n`;
                line += `    urldate         = {${cite.citeDate}},\n`;
                line += `    url             = {${cite.url}},\n`;
                break;

            case 'book':
                line += `    publisher       = {${cite.publisher}},\n`;
                line += `    isbn            = {${cite.isbn}},\n`;
                line += `    year            = {${cite.year}},\n`;
                break;
        }

        line += `}\n`;
        final.push(line);
    }

    return final.join('\n');
};
