import { conn } from '$/lib/server/variables';
import { MovieDTO } from '../../dto/movie';
import type { MovieDAO } from '../interfaces/movieDao';

export class MovieMariaDAO implements MovieDAO {
    async getMovies(): Promise<MovieDTO[]> {
        const data = await conn.selectFrom('movie').selectAll().execute();
        return data.map((item) => {
            return new MovieDTO(item.id, item.name, item.studio);
        });
    }
    async getMovie(id: number): Promise<MovieDTO | undefined> {
        const data = await conn.selectFrom('movie').selectAll().where('id', '=', id).executeTakeFirst();
        if (!data) return data;
        return new MovieDTO(data.id, data.name, data.studio);
    }
    async createMovie(movie: MovieDTO): Promise<void> {
        await conn
            .insertInto('movie')
            .values({
                id: movie.id,
                name: movie.name,
                studio: movie.studio
            })
            .execute();
    }
}
