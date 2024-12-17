import { StarredMovieDTO } from '../../dto/starred_movie';
import type { UserDTO } from '../../dto/user';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { StarredMovieDAO } from '../interfaces/starred_movieDao';

export class StarredMovieJsonDAO implements StarredMovieDAO {
    private db = new JsonDBAbstract<StarredMovieDTO>('starred.json');

    async getStarred(user: UserDTO): Promise<StarredMovieDTO[]> {
        return this.db.getAll().filter((item) => item.userId === user.id);
    }
}
