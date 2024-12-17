import type { MovieDTO } from '../../dto/movie';
import { RatingDTO } from '../../dto/rating';
import { GlobalConnector } from '../../globalConnector';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { RatingDAO } from '../interfaces/ratingDao';
import path from 'node:path';

export class RatingJsonDAO implements RatingDAO {
    private db = new JsonDBAbstract<RatingDTO>(path.join(GlobalConnector.jsonDBPath, 'actor.json'));

    async getRatings(movie: MovieDTO): Promise<RatingDTO[]> {
        return this.db.getAll().filter((item) => item.id === movie.id);
    }
    async createRating(rating: RatingDTO): Promise<void> {
        this.db.addItem(rating);
    }
}
