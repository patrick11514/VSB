import { gen } from './lib/gen';
import { prompt } from './lib/prompt';
import { BaseCite, Cite } from './types/types';
import fs from 'node:fs';

const types: Cite['type'][] = ['online', 'book'];

const toYYYYMMDD = (date: Date): string => {
    return date.toISOString().split('T')[0];
};

if (process.argv.includes('--generate')) {
    const cites: Cite[] = JSON.parse(fs.readFileSync('src.json', 'utf-8'));
    fs.writeFileSync('sources.bib', gen(cites));
    process.exit(0);
} else if (process.argv.includes('--add')) {
    console.log('Select type of source: ');
    for (let i = 0; i < types.length; i++) {
        console.log(`${i + 1}. ${types[i]}`);
    }

    const { type, name, title, author, lang } = await prompt([
        {
            type: 'select',
            name: 'type',
            message: 'Select type of source: ',
            choices: types
        },
        { type: 'input', name: 'name', message: 'Enter the name of the source: ' },
        {
            type: 'input',
            name: 'title',
            message: 'Enter the title of the source: '
        },
        {
            type: 'input',
            name: 'author',
            message: 'Enter the author of the source: (optional)',
            initial: ''
        },
        {
            type: 'select',
            name: 'lang',
            message: 'Enter the language of the source: ',
            choices: ['english'] satisfies Cite['lang'][]
        }
    ] as const);

    const baseCite = {
        name,
        title,
        author: author === '' ? undefined : author,
        lang
    } satisfies BaseCite;

    let cite: Cite;
    switch (type) {
        case 'online':
            const { date, citeDate, url } = await prompt([
                {
                    type: 'input',
                    name: 'date',
                    message: 'Enter the date of the source: ',
                    initial: toYYYYMMDD(new Date())
                },
                {
                    type: 'input',
                    name: 'citeDate',
                    message: 'Enter the date you cited the source: ',
                    initial: toYYYYMMDD(new Date())
                },
                {
                    type: 'input',
                    name: 'url',
                    message: 'Enter the URL of the source: '
                }
            ] as const);

            cite = {
                type,
                date,
                citeDate,
                url,
                ...baseCite
            } satisfies Cite;
            break;
        case 'book':
            const { year, publisher, isbn } = await prompt([
                {
                    type: 'input',
                    name: 'year',
                    message: 'Enter the year of the source: '
                },
                {
                    type: 'input',
                    name: 'publisher',
                    message: 'Enter the publisher: '
                },
                {
                    type: 'input',
                    name: 'isbn',
                    message: 'Enter the ISBN (optional): ',
                    initial: ''
                }
            ] as const);
            cite = {
                type,
                year,
                publisher,
                isbn: isbn === '' ? undefined : isbn,
                ...baseCite
            };
            break;
    }

    const cites: Cite[] = JSON.parse(fs.readFileSync('src.json', 'utf-8'));
    cites.push(cite);

    fs.writeFileSync('src.json', JSON.stringify(cites, null, 4));

    console.log('Source added successfully!');
}
