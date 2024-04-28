import { createAPIClient } from '@patrick115/sveltekitapi';
import type { AppRouter } from './server/_app';

export const API = createAPIClient<AppRouter>('/api');
