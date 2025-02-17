import type { StarredMovieDTO } from '../../dto/starred_movie';
import type { UserDTO } from '../../dto/user';

export interface StarredMovieDAO {
    getStarred(user: UserDTO): Promise<StarredMovieDTO[]>;
    setStarred(user: number, movie: number): Promise<bigint>;
    removeStarred(user: number, movie: number): Promise<void>;
}
