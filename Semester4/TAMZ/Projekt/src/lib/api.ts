import { createAPIClient } from '@patrick115/sveltekitapi';
import type { AppRouter } from './server/_app/index';

export const API = createAPIClient<AppRouter>('/api');
