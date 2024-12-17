import { API } from '../api';

export class ReviewGateway {
    async getReviews(movie: number) {
        return API.review(movie);
    }
}
