import type { CastDTO } from '../../dto/cast';
import type { MovieDTO } from '../../dto/movie';

export interface CastDAO {
    getCasts(movie: MovieDTO): Promise<CastDTO[]>;
    addCast(cast: CastDTO): Promise<void>;
}
