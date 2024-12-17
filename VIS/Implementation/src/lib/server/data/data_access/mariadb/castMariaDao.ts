import { conn } from '$/lib/server/variables';
import { CastDTO } from '../../dto/cast';
import type { MovieDTO } from '../../dto/movie';
import type { CastDAO } from '../interfaces/castDao';

export class CastMariaDAO implements CastDAO {
    async getCasts(movie: MovieDTO): Promise<CastDTO[]> {
        const data = await conn.selectFrom('cast').selectAll().where('movie_id', '=', movie.id).execute();
        return data.map((item) => {
            return new CastDTO(item.actor_id, item.movie_id);
        });
    }
    async addCast(cast: CastDTO): Promise<void> {
        await conn
            .insertInto('cast')
            .values({
                actor_id: cast.actorId,
                movie_id: cast.movieId
            })
            .execute();
    }
}
