import { ActorGateway } from '../gateway/actorGateway';

export class ActorService {
    private gateway = new ActorGateway();

    async getActors() {
        const data = await this.gateway.getActors();
        if (!data.status) return [];
        return data.data;
    }
}
