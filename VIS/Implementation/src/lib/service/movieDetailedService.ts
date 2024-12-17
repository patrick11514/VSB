import type { MovieDetailedDTO } from '../dto/movieDetailedDTO';
import { MovieGateway } from '../gateway/movieGateway';
import { RatingGateway } from '../gateway/ratingGateway';
import { ReviewGateway } from '../gateway/reviewGateway';

export class MovieDetailedService {
    private gateway = new MovieGateway();

    async getMovies() {
        const result = await this.gateway.getMovies();
        if (!result.status) return undefined;
        if (result.data.length == 0) return null;

        //add ratings
        const ratingGateway = new RatingGateway();
        const reviewGateway = new ReviewGateway();
        return Promise.all(
            result.data.map(async (movie) => {
                const ratings = await ratingGateway.getRating(movie.id);
                const reviews = await reviewGateway.getReviews(movie.id);
                return {
                    ...movie,
                    ratings: ratings.status ? ratings.data : [],
                    reviews: reviews.status ? reviews.data : []
                } satisfies MovieDetailedDTO;
            })
        );
    }

    getAvgRating(movie: MovieDetailedDTO) {
        if (movie.ratings.length == 0) return 5.0;
        return movie.ratings.map((rating) => rating.rating).reduce((a, b) => a + b, 0) / movie.ratings.length;
    }
}
