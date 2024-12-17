import { API } from '../api';

export class MovieGateway {
    async getMovies(search?: string) {
        if (!search) {
            return API.movies.GET();
        }
        return API.movies.POST(search);
    }
}
