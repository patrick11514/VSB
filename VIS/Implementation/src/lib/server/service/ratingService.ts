import { MovieDTO } from '../data/dto/movie';
import { GlobalConnector } from '../data/globalConnector';
import { RatingMapper } from '../domain/mappers/ratingMapper';

export class RatingService {
    private mapper = new RatingMapper();

    async getRating(movieId: number) {
        const dto = await GlobalConnector.connector.ratingDao.getRatings(new MovieDTO(movieId, '', ''));
        return dto.map(this.mapper.toDomainModel);
    }
}
