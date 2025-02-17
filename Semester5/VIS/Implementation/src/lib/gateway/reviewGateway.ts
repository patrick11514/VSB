import { API } from '../api';

export class ReviewGateway {
    async getReviews(movie: number) {
        return API.review.POST(movie);
    }

    async setReview(movie: number, value: string) {
        return API.review.PUT({
            value,
            movie_id: movie
        });
    }
}
