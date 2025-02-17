import { ActorMariaDAO } from '../mariadb/actorMariaDao';
import { CastMariaDAO } from '../mariadb/castMariaDao';
import { MovieMariaDAO } from '../mariadb/movieMariaDao';
import { RatingMariaDAO } from '../mariadb/ratingMariaDao';
import { ReviewMariaDAO } from '../mariadb/reviewMariaDao';
import { StarredMovieMariaDAO } from '../mariadb/starred_movieMariaDao';
import { UserMariaDAO } from '../mariadb/userMariaDao';
import type { DataConnector } from './DataConnector';

export class MariaDBConnector implements DataConnector {
    actorDao = new ActorMariaDAO();
    castDao = new CastMariaDAO();
    movieDao = new MovieMariaDAO();
    ratingDao = new RatingMariaDAO();
    reviewDao = new ReviewMariaDAO();
    starredMovieDao = new StarredMovieMariaDAO();
    userDao = new UserMariaDAO();
}
