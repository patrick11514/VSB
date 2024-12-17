import { MovieDTO } from '../../dto/movie';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { MovieDAO } from '../interfaces/movieDao';

export class MovieJsonDAO implements MovieDAO {
    private db = new JsonDBAbstract<MovieDTO>('movie.json');

    async getMovies(): Promise<MovieDTO[]> {
        const data = this.db.getAll();
        return data;
    }
    async getMovie(id: number): Promise<MovieDTO | undefined> {
        return this.db.getAll().find((item) => item.id === id);
    }
    async createMovie(movie: MovieDTO): Promise<bigint> {
        return this.db.addItem(movie);
    }
}
