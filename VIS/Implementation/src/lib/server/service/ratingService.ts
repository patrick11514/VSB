import { MovieDTO } from '../data/dto/movie';
import { RatingDTO } from '../data/dto/rating';
import { GlobalConnector } from '../data/globalConnector';
import { RatingMapper } from '../domain/mappers/ratingMapper';

export class RatingService {
    private mapper = new RatingMapper();

    async getRating(movieId: number) {
        const dto = await GlobalConnector.connector.ratingDao.getRatings(new MovieDTO(movieId, '', '', 0));
        return dto.map(this.mapper.toDomainModel);
    }

    async addRating(movieId: number, userId: number, value: number) {
        return await GlobalConnector.connector.ratingDao.createRating(new RatingDTO(-1, userId, movieId, value))
    }
}
