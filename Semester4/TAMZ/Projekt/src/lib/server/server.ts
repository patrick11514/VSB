import { APIServer } from '@patrick115/sveltekitapi';
import { Router } from './_app/index';
import { createContext } from './context';

export const Server = new APIServer({
    router: Router,
    context: createContext,
    path: '/api'
});
