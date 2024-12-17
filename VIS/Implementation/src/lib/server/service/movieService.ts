import { CastDTO } from '../data/dto/cast';
import { MovieDTO } from '../data/dto/movie';
import { GlobalConnector } from '../data/globalConnector';
import { MovieMapper } from '../domain/mappers/movieMapper';

export class MovieService {
    private mapper = new MovieMapper();

    async getMovies(name?: string) {
        const dto = await GlobalConnector.connector.movieDao.getMovies();
        const movies = dto.map(this.mapper.toDomainModel);

        return movies.filter((movie) => (!name ? true : movie.name.toLocaleLowerCase().includes(name.toLowerCase())));
    }

    async addMovie(name: string, studio: string, year: number, actors: number[]) {
        let newId = await GlobalConnector.connector.movieDao.createMovie(new MovieDTO(0, name, studio, year));
        if (newId < 0) {
            return false;
        }

        await Promise.all(actors.map((actor) => GlobalConnector.connector.castDao.addCast(new CastDTO(actor, Number(newId)))));

        return true;
    }
}
