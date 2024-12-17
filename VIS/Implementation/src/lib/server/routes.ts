import { z } from 'zod';
import { procedure, router } from './api';
import { UserService } from './service/userService';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import { UserDomainModel } from './domain/models/userDomainModel';
import type { Response } from '$/types/response';

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
            const service = new UserService();
            if (await service.exists(input.username, input.email)) {
                return {
                    status: false,
                    code: 400,
                    message: 'Uživatel s tímto jménem/emailem již existuje'
                } satisfies ErrorApiResponse;
            }

            const id = await service.createUser(new UserDomainModel(-1, input.username, input.email, input.password));
            if (id == BigInt(-1)) {
                return {
                    status: false,
                    code: 400,
                    message: 'Nepovedlo se vytvořit účet, zkus to prosím později'
                } satisfies ErrorApiResponse;
            }

            return {
                status: true
            } satisfies Response;
        })
    }
});

export type AppRouter = typeof r;
