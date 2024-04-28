import { APIServer } from '@patrick115/sveltekitapi';
import { r } from './_app';
import { context } from './context';

export const Server = new APIServer({
    router: r,
    path: '/api',
    context
});
