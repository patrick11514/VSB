import type { MovieDTO } from '../../dto/movie';
import type { ReviewDTO } from '../../dto/review';

export interface ReviewDAO {
    getReviews(movie: MovieDTO): Promise<ReviewDTO[]>;
    createReview(review: ReviewDTO): Promise<bigint>;
}
