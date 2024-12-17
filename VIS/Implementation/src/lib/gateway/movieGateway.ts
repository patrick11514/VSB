import { API } from '../api';

export class MovieGateway {
    async getMovies(search?: string) {
        if (!search) {
            return API.movies.GET();
        }
        return API.movies.POST(search);
    }

    async getMovie(id: number) {
        return API.movie(id);
    }

    async addMovie(name: string, studio: string, year: number, actors: number[]) {
        return API.movies.PUT({ name, studio, year, actors });
    }
}
