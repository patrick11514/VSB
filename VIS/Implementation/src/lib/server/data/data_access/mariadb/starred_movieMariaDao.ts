import { conn } from '$/lib/server/variables';
import { StarredMovieDTO } from '../../dto/starred_movie';
import type { UserDTO } from '../../dto/user';
import type { StarredMovieDAO } from '../interfaces/starred_movieDao';

export class StarredMovieMariaDAO implements StarredMovieDAO {
    async getStarred(user: UserDTO): Promise<StarredMovieDTO[]> {
        const data = await conn.selectFrom('starred_movie').selectAll().where('user_id', '=', user.id).execute();
        return data.map((item) => {
            return new StarredMovieDTO(item.movie_id, item.user_id);
        });
    }
    async setStarred(user: number, movie: number): Promise<bigint> {
        const data = await conn
            .insertInto('starred_movie')
            .values({
                user_id: user,
                movie_id: movie
            })
            .executeTakeFirst();

        return data.insertId ?? BigInt(-1);
    }
    async removeStarred(user: number, movie: number): Promise<void> {
        await conn
            .deleteFrom('starred_movie')
            .where((eb) => eb.or([eb('user_id', '=', user), eb('movie_id', '=', movie)]))
            .execute();
    }
}
