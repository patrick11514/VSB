import { ActorJsonDAO } from '../jsondb/actorJsonDao';
import { CastJsonDAO } from '../jsondb/castJsonDao';
import { MovieJsonDAO } from '../jsondb/movieJsonDao';
import { RatingJsonDAO } from '../jsondb/ratingJsonDao';
import { ReviewJsonDAO } from '../jsondb/reviewJsonDao';
import { StarredMovieJsonDAO } from '../jsondb/starred_movieJsonDao';
import { UserJsonDAO } from '../jsondb/userJsonDao';
import type { DataConnector } from './DataConnector';

export class JsonDBConnector implements DataConnector {
    actorDao = new ActorJsonDAO();
    castDao = new CastJsonDAO();
    movieDao = new MovieJsonDAO();
    ratingDao = new RatingJsonDAO();
    reviewDao = new ReviewJsonDAO();
    starredMovieDao = new StarredMovieJsonDAO();
    userDao = new UserJsonDAO();
}
