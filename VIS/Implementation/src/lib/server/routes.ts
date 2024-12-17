import { z } from 'zod';
import { procedure, router } from './api';

export const r = router({
    auth: {
        register: procedure.POST.input(
            z.object({
                username: z.string(),
                email: z.string().email(),
                password: z.string()
            })
        ).query(async ({ input }) => {
            //check db
        })
    }
});

export type AppRouter = typeof r;
