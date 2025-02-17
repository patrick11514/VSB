import { item, schema } from '$/types/types';
import fs from 'node:fs';
import { z } from 'zod';
import { procedure } from '../api';

const getItems = () => {
    const source = fs.readFileSync('data.json', 'utf-8');
    const parsed = schema.safeParse(JSON.parse(source));

    if (!parsed.success) {
        throw new Error('Invalid data.json');
    }

    return parsed.data;
};

const saveItems = (items: z.infer<typeof schema>) => {
    fs.writeFileSync('data.json', JSON.stringify(items, null, 4));
};

export default [
    procedure.GET.query(async () => {
        return getItems();
    }),
    procedure.POST.input(item).query(async ({ input }) => {
        const items = getItems();
        items.push(input);
        saveItems(items);

        return items;
    }),
    procedure.PATCH.input(
        z.object({
            index: z.number(),
            newData: item
        })
    ).query(async ({ input: { index, newData } }) => {
        const items = getItems();
        items[index] = newData;
        saveItems(items);

        return items;
    }),
    procedure.DELETE.input(z.number()).query(async ({ input }) => {
        const items = getItems();
        const newItems = items.filter((_, i) => i !== input);
        saveItems(newItems);

        return newItems;
    })
];
