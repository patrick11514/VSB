import { conn } from '$/lib/server/variables';
import type { MovieDTO } from '../../dto/movie';
import { ReviewDTO } from '../../dto/review';
import type { ReviewDAO } from '../interfaces/reviewDao';

export class ReviewMariaDAO implements ReviewDAO {
    async getReviews(movie: MovieDTO): Promise<ReviewDTO[]> {
        const data = await conn.selectFrom('review').selectAll().where('movie_id', '=', movie.id).execute();
        return data.map((item) => {
            return new ReviewDTO(item.id, item.user_id, item.movie_id, item.text);
        });
    }
    async createReview(review: ReviewDTO): Promise<bigint> {
        const data = await conn
            .insertInto('review')
            .values({
                user_id: review.user,
                movie_id: review.movie,
                text: review.text
            })
            .executeTakeFirst();
        return data.insertId ?? BigInt(-1);
    }
}
