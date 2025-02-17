import { MovieService } from '$/lib/server/service/movieService';
import { redirect } from '@sveltejs/kit';
import type { PageServerLoad } from './$types';

export const load = (async ({ params }) => {
    //check if exists
    const service = new MovieService();
    if (!(await service.getMovie(parseInt(params.id)))) {
        redirect(302, '/');
    }
}) satisfies PageServerLoad;
