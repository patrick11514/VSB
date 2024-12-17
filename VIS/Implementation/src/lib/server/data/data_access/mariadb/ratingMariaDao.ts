import { conn } from '$/lib/server/variables';
import type { MovieDTO } from '../../dto/movie';
import { RatingDTO } from '../../dto/rating';
import type { RatingDAO } from '../interfaces/ratingDao';

export class RatingMariaDAO implements RatingDAO {
    async getRatings(movie: MovieDTO): Promise<RatingDTO[]> {
        const data = await conn.selectFrom('rating').selectAll().where('movie_id', '=', movie.id).execute();
        return data.map((item) => {
            return new RatingDTO(item.id, item.user_id, item.movie_id, item.rating);
        });
    }
    async createRating(rating: RatingDTO): Promise<bigint> {
        const data = await conn
            .insertInto('rating')
            .values({
                user_id: rating.user,
                movie_id: rating.movie,
                rating: rating.rating
            })
            .executeTakeFirst();
        return data.insertId ?? BigInt(-1);
    }
}
