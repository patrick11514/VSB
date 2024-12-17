import { CastDTO } from '../../dto/cast';
import type { MovieDTO } from '../../dto/movie';
import { GlobalConnector } from '../../globalConnector';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { CastDAO } from '../interfaces/castDao';
import path from 'node:path';

export class CastJsonDAO implements CastDAO {
    private db = new JsonDBAbstract<CastDTO>(path.join(GlobalConnector.jsonDBPath, 'cast.json'));

    async getCasts(movie: MovieDTO): Promise<CastDTO[]> {
        return this.db.getAll().filter((item) => item.movieId === movie.id);
    }
    async addCast(cast: CastDTO): Promise<void> {
        this.db.addItem(cast);
    }
}
