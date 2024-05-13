import { router } from '../api';
import bank from './bank';
import ex from './export';
import im from './import';
import targets from './targets';
import transactions from './transactions';

export const Router = router({
    bank,
    transactions,
    targets,
    export: ex,
    import: im
});

export type AppRouter = typeof Router;
