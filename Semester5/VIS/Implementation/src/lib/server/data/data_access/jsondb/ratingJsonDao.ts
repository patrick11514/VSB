import type { MovieDTO } from '../../dto/movie';
import { RatingDTO } from '../../dto/rating';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { RatingDAO } from '../interfaces/ratingDao';

export class RatingJsonDAO implements RatingDAO {
    private db = new JsonDBAbstract<RatingDTO>('rating.json');

    async getRatings(movie: MovieDTO): Promise<RatingDTO[]> {
        return this.db.getAll().filter((item) => item.id === movie.id);
    }
    async createRating(rating: RatingDTO): Promise<bigint> {
        return this.db.addItem(rating);
    }
}
