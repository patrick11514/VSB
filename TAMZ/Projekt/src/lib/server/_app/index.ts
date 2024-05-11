import { router } from '../api';
import bank from './bank';

export const Router = router({
    bank
});

export type AppRouter = typeof Router;
