import { GlobalConnector } from '../data/globalConnector';
import { MovieMapper } from '../domain/mappers/movieMapper';

export class MovieService {
    private mapper = new MovieMapper();

    async getMovies(name?: string) {
        const dto = await GlobalConnector.connector.movieDao.getMovies();
        const movies = dto.map(this.mapper.toDomainModel);

        return movies.filter((movie) => (!name ? true : movie.name.toLocaleLowerCase().includes(name.toLowerCase())));
    }
}
