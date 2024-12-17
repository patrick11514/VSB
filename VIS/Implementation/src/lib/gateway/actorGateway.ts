import { API } from '../api';

export class ActorGateway {
    async getActors() {
        return API.actor.GET();
    }
}
