import { z } from 'zod';
import 'dotenv/config';

const schema = z.object({
    PORT: z.coerce.number()
});

export const env = schema.parse(process.env);
