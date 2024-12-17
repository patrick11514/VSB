import { API } from '../api';

export class ActorGateway {
    async getActors() {
        return API.actor.GET();
    }
    async getActorForMovie(movie: number) {
        return API.actor.POST(movie);
    }
}
