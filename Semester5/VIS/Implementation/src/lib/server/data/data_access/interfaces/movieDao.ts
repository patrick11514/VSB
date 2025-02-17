import type { MovieDTO } from '../../dto/movie';

export interface MovieDAO {
    getMovies(): Promise<MovieDTO[]>;
    getMovie(id: number): Promise<MovieDTO | undefined>;
    createMovie(movie: MovieDTO): Promise<bigint>;
}
