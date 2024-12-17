import { CastDTO } from '../../dto/cast';
import type { MovieDTO } from '../../dto/movie';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { CastDAO } from '../interfaces/castDao';

export class CastJsonDAO implements CastDAO {
    private db = new JsonDBAbstract<CastDTO>('cast.json');

    async getCasts(movie: MovieDTO): Promise<CastDTO[]> {
        return this.db.getAll().filter((item) => item.movieId === movie.id);
    }
    async addCast(cast: CastDTO): Promise<bigint> {
        return this.db.addItem(cast);
    }
}
