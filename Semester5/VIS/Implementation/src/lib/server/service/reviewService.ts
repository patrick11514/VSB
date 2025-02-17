import { ReviewDTO } from '$/lib/dto/reviewDTO';
import { MovieDTO } from '../data/dto/movie';
import { GlobalConnector } from '../data/globalConnector';
import { ReviewMapper } from '../domain/mappers/reviewMapper';

export class ReviewService {
    private mapper = new ReviewMapper();

    async getReviews(movieId: number) {
        const dto = await GlobalConnector.connector.reviewDao.getReviews(new MovieDTO(movieId, '', '', 0));
        return dto.map(this.mapper.toDomainModel);
    }
    async addReview(movieId: number, userId: number, value: string) {
        return await GlobalConnector.connector.reviewDao.createReview(new ReviewDTO(-1, userId, movieId, value))
    }
}
