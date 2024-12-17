import type { MovieDTO } from '../../dto/movie';
import { ReviewDTO } from '../../dto/review';
import { GlobalConnector } from '../../globalConnector';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { ReviewDAO } from '../interfaces/reviewDao';
import path from 'node:path';

export class ReviewJsonDAO implements ReviewDAO {
    private db = new JsonDBAbstract<ReviewDTO>(path.join(GlobalConnector.jsonDBPath, 'review.json'));

    async getReviews(movie: MovieDTO): Promise<ReviewDTO[]> {
        return this.db.getAll().filter((item) => item.movie === movie.id);
    }
    async createReview(review: ReviewDTO): Promise<bigint> {
        return this.db.addItem(review);
    }
}
