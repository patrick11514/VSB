import { API } from '../api';

export class RatingGateway {
    async getRating(movie: number) {
        return API.rating.POST(movie);
    }

    async setRating(movie: number, value: number) {
        return API.rating.PUT({
            value,
            movie_id: movie
        });
    }
}
