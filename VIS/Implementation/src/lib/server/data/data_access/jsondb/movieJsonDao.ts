import { MovieDTO } from '../../dto/movie';
import { GlobalConnector } from '../../globalConnector';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { MovieDAO } from '../interfaces/movieDao';
import path from 'node:path';

export class MovieJsonDAO implements MovieDAO {
    private db = new JsonDBAbstract<MovieDTO>(path.join(GlobalConnector.jsonDBPath, 'movie.json'));

    async getMovies(): Promise<MovieDTO[]> {
        const data = this.db.getAll();
        return data;
    }
    async getMovie(id: number): Promise<MovieDTO | undefined> {
        return this.db.getAll().find((item) => item.id === id);
    }
    async createMovie(movie: MovieDTO): Promise<void> {
        this.db.addItem(movie);
    }
}
