import type { MovieDTO } from '../dto/movieDTO';
import { MovieGateway } from '../gateway/movieGateway';
import { RatingGateway } from '../gateway/ratingGateway';

export class MovieService {
    private gateway = new MovieGateway();
    async getMovies(term?: string) {
        const result = await this.gateway.getMovies(term);
        if (!result.status) return undefined;
        if (result.data.length == 0) return null;

        //add ratings
        const ratingGateway = new RatingGateway();
        return Promise.all(
            result.data.map(async (movie) => {
                const ratings = await ratingGateway.getRating(movie.id);
                let rating: number;
                if (!ratings.status || ratings.data.length == 0) {
                    rating = 5.0;
                } else {
                    rating = ratings.data.map((r) => r.rating).reduce((a, b) => a + b, 0) / ratings.data.length;
                }
                return {
                    ...movie,
                    rating
                } satisfies MovieDTO;
            })
        );
    }

    async addMovie(name: string, studio: string, year: number, actors: number[]) {
        return this.gateway.addMovie(name, studio, year, actors);
    }
}
