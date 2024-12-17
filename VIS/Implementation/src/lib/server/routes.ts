import { z } from 'zod';
import { loginProcedure, procedure, router } from './api';
import { UserService } from './service/userService';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import { UserDomainModel } from './domain/models/userDomainModel';
import type { Response, ResponseWithData } from '$/types/response';
import type { UserData } from '$/types/types';
import { COOKIE_EXPIRE } from '$env/static/private';
import type { MovieDomainModel } from './domain/models/movieDomainModel';
import { MovieService } from './service/movieService';
import { RatingService } from './service/ratingService';
import { ActorService } from './service/actorService';
import type { ActorDomainModel } from './domain/models/actorDomainModel';
import { ReviewService } from './service/reviewService';
import type { RatingDomainModel } from './domain/models/ratingDomainModel';
import type { ReviewDomainModel } from './domain/models/reviewDomainModel';

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
        }),
        login: procedure.POST.input(
            z.object({
                username: z.string(),
                password: z.string()
            })
        ).query(async ({ input, ev: { cookies } }) => {
            const service = new UserService();
            const data = await service.login(input.username, input.password);
            if (!data) {
                return {
                    status: false,
                    code: 400,
                    message: 'Neplatné uživatelské jméno, nebo heslo'
                } satisfies ErrorApiResponse;
            }

            cookies.set('session', data.cookie, {
                path: '/',
                maxAge: parseInt(COOKIE_EXPIRE)
            });

            return {
                status: true,
                data: data.data
            } satisfies ResponseWithData<UserData>;
        })
    },
    movies: [
        procedure.GET.query(async () => {
            const service = new MovieService();

            return {
                status: true,
                data: await service.getMovies()
            } satisfies ResponseWithData<MovieDomainModel[]>;
        }),
        procedure.POST.input(z.string()).query(async ({ input }) => {
            const service = new MovieService();

            return {
                status: true,
                data: await service.getMovies(input)
            } satisfies ResponseWithData<MovieDomainModel[]>;
        }),
        loginProcedure.PUT.input(
            z.object({
                name: z.string(),
                studio: z.string(),
                year: z.number(),
                actors: z.array(z.number())
            })
        ).query(async ({ input }) => {
            const service = new MovieService();
            if (!(await service.addMovie(input.name, input.studio, input.year, input.actors))) {
                return {
                    status: false,
                    code: 500,
                    message: 'Nepovedlo se vytvořit film, zkus to prosím později'
                } satisfies ErrorApiResponse;
            }

            return {
                status: true
            } satisfies Response;
        })
    ],
    rating: procedure.POST.input(z.number()).query(async ({ input }) => {
        const service = new RatingService();
        return {
            status: true,
            data: await service.getRating(input)
        } satisfies ResponseWithData<RatingDomainModel[]>;
    }),
    review: procedure.POST.input(z.number()).query(async ({ input }) => {
        const service = new ReviewService();
        return {
            status: true,
            data: await service.getReviews(input)
        } satisfies ResponseWithData<ReviewDomainModel[]>;
    }),
    actor: [
        procedure.GET.query(async () => {
            const service = new ActorService();
            return {
                status: true,
                data: await service.getActors()
            } satisfies ResponseWithData<ActorDomainModel[]>;
        }),
        procedure.POST.input(z.number()).query(async ({ input }) => {
            const service = new ActorService();
            return {
                status: true,
                data: await service.getActors(input)
            } satisfies ResponseWithData<ActorDomainModel[]>;
        })
    ]
});

export type AppRouter = typeof r;
