import { router } from './api';

import data from './routes/data';

export const r = router({
    data
});

export type AppRouter = typeof r;
