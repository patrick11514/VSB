import type { ActorDAO } from '../interfaces/actorDao';
import type { CastDAO } from '../interfaces/castDao';
import type { MovieDAO } from '../interfaces/movieDao';
import type { RatingDAO } from '../interfaces/ratingDao';
import type { ReviewDAO } from '../interfaces/reviewDao';
import type { StarredMovieDAO } from '../interfaces/starred_movieDao';
import type { UserDAO } from '../interfaces/userDao';

export interface DataConnector {
    actorDao: ActorDAO;
    castDao: CastDAO;
    movieDao: MovieDAO;
    ratingDao: RatingDAO;
    reviewDao: ReviewDAO;
    starredMovieDao: StarredMovieDAO;
    userDao: UserDAO;
}
