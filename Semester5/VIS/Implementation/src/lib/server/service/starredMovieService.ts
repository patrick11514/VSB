import { UserDTO } from '../data/dto/user';
import { GlobalConnector } from '../data/globalConnector';
import { StarredMovieMapper } from '../domain/mappers/starred_movieMapper';

export class StarredMovieService {
    private mapper = new StarredMovieMapper();

    async getBookmark(user: number, movie: number) {
        const stars = await GlobalConnector.connector.starredMovieDao.getStarred(new UserDTO(user, '', '', ''));
        return !!stars.map(this.mapper.toDomainModel).find((star) => star.movieId === movie);
    }

    async setBookmark(user: number, movie: number) {
        return GlobalConnector.connector.starredMovieDao.setStarred(user, movie);
    }

    async removeBookmark(user: number, movie: number) {
        return GlobalConnector.connector.starredMovieDao.removeStarred(user, movie);
    }
}
