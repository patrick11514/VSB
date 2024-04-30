import { z } from 'zod';

type BaseColor = 'primary' | 'secondary' | 'success' | 'danger' | 'warning' | 'info' | 'light' | 'dark';
type OtherColor = 'body' | 'body-secondary' | 'body-tertiary' | 'black' | 'white' | 'transparent';

export type Color = BaseColor | `${BaseColor}-subtle` | OtherColor;

const item = z.object({
    name: z.string(),
    manufacturer: z.string(),
    size: z.number(),
    price: z.number(),
    weight: z.number(),
    images: z.object({
        main: z.string(),
        other: z.array(z.string())
    })
});

export const schema = z.array(item);

export type Item = z.infer<typeof item>;
