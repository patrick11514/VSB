import { StarredMovieDTO } from '../../dto/starred_movie';
import type { UserDTO } from '../../dto/user';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { StarredMovieDAO } from '../interfaces/starred_movieDao';

export class StarredMovieJsonDAO implements StarredMovieDAO {
    private db = new JsonDBAbstract<StarredMovieDTO>('starred.json');

    async getStarred(user: UserDTO): Promise<StarredMovieDTO[]> {
        return this.db.getAll().filter((item) => item.userId === user.id);
    }
    async setStarred(user: number, movie: number): Promise<bigint> {
        return this.db.addItem(new StarredMovieDTO(movie, user));
    }
    async removeStarred(user: number, movie: number): Promise<void> {
        const item = this.db.getAll().find((item) => item.userId === user && item.movieId === movie);
        if (!item) return;
        this.db.removeItem(item);
    }
}
