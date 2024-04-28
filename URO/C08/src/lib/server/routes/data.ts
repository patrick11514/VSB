import { schema } from '$/types/types';
import fs from 'node:fs';
import { procedure } from '../api';

export default [
    procedure.GET.query(async () => {
        const source = fs.readFileSync('data.json', 'utf-8');
        const parsed = schema.safeParse(JSON.parse(source));

        if (!parsed.success) {
            throw new Error('Invalid data.json');
        }

        return parsed.data;
    })
];
