import type { MovieDetailedDTO } from '../dto/movieDetailedDTO';
import { ActorGateway } from '../gateway/actorGateway';
import { MovieGateway } from '../gateway/movieGateway';
import { RatingGateway } from '../gateway/ratingGateway';
import { ReviewGateway } from '../gateway/reviewGateway';
import { StarredMovieGateway } from '../gateway/starredMovieGateway';
import { UserGateway } from '../gateway/userGateway';
import type { MovieDomainModel } from '../server/domain/models/movieDomainModel';
import type { UserDomainModel } from '../server/domain/models/userDomainModel';

export class MovieDetailedService {
    private gateway = new MovieGateway();
    private ratingGateway = new RatingGateway();
    private reviewGateway = new ReviewGateway();
    private actorGateways = new ActorGateway();
    private starredMovieGateway = new StarredMovieGateway();

    private async enrich(movie: MovieDomainModel): Promise<MovieDetailedDTO> {
        const ratings = await this.ratingGateway.getRating(movie.id);
        const reviews = await this.reviewGateway.getReviews(movie.id);
        return {
            ...movie,
            ratings: ratings.status ? ratings.data : [],
            reviews: reviews.status ? reviews.data : []
        };
    }

    async getMovies() {
        const result = await this.gateway.getMovies();
        if (!result.status) return undefined;
        if (result.data.length == 0) return null;

        return Promise.all(result.data.map(this.enrich));
    }

    getAvgRating(movie: MovieDetailedDTO) {
        if (movie.ratings.length == 0) return 5.0;
        return movie.ratings.map((rating) => rating.rating).reduce((a, b) => a + b, 0) / movie.ratings.length;
    }

    async getMovie(movie: number) {
        const result = await this.gateway.getMovie(movie);
        if (!result.status) return undefined;

        return this.enrich(result.data);
    }

    async getActors(movie: MovieDetailedDTO) {
        const data = await this.actorGateways.getActorForMovie(movie.id);
        return data.status ? data.data : [];
    }

    async resolveUsers<$Type extends { user: number }>(
        propetyWithUser: $Type[]
    ): Promise<
        (Omit<$Type, 'user'> & {
            user: UserDomainModel | undefined;
        })[]
    > {
        const userGateway = new UserGateway();
        const promises = propetyWithUser.map((item) => userGateway.getUser(item.user));
        const resolved = await Promise.all(promises);
        const users = resolved.map((user) => (user.status ? user.data : undefined));
        return propetyWithUser.map((item, i) => ({ ...item, user: users[i] }));
    }

    async setRating(movie: MovieDetailedDTO, value: number) {
        return this.ratingGateway.setRating(movie.id, value);
    }

    async setReview(movie: MovieDetailedDTO, value: string) {
        return this.reviewGateway.setReview(movie.id, value);
    }

    async isBookmarked(movie: MovieDetailedDTO) {
        return this.starredMovieGateway.getStarred(movie.id);
    }

    async toggleBookmarked(movie: MovieDetailedDTO) {
        return this.starredMovieGateway.toggleStarred(movie.id);
    }
}
