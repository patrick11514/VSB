import { MovieDTO } from '../data/dto/movie';
import { GlobalConnector } from '../data/globalConnector';
import { ReviewMapper } from '../domain/mappers/reviewMapper';

export class ReviewService {
    private mapper = new ReviewMapper();

    async getReviews(movieId: number) {
        const dto = await GlobalConnector.connector.reviewDao.getReviews(new MovieDTO(movieId, '', '', 0));
        return dto.map(this.mapper.toDomainModel);
    }
}
