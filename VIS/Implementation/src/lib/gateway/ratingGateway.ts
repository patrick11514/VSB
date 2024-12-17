import { API } from '../api';

export class RatingGateway {
    async getRating(movie: number) {
        return API.rating(movie);
    }
}
