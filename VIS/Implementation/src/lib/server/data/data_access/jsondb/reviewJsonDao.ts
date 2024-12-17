import type { MovieDTO } from '../../dto/movie';
import { ReviewDTO } from '../../dto/review';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { ReviewDAO } from '../interfaces/reviewDao';

export class ReviewJsonDAO implements ReviewDAO {
    private db = new JsonDBAbstract<ReviewDTO>('review.json');

    async getReviews(movie: MovieDTO): Promise<ReviewDTO[]> {
        return this.db.getAll().filter((item) => item.movie === movie.id);
    }
    async createReview(review: ReviewDTO): Promise<bigint> {
        return this.db.addItem(review);
    }
}
