import { API } from '../api';

export class StarredMovieGateway {
    async getStarred(movie: number) {
        return API.getBookmark(movie);
    }

    async toggleStarred(movie: number) {
        return API.toggleBookmark(movie);
    }
}
