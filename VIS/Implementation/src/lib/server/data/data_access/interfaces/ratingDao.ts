import type { MovieDTO } from '../../dto/movie';
import type { RatingDTO } from '../../dto/rating';

export interface RatingDAO {
    getRatings(movie: MovieDTO): Promise<RatingDTO[]>;
    createRating(rating: RatingDTO): Promise<void>;
}
