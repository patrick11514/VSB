import { ActorDTO } from '../../dto/actor';
import type { ActorDAO } from '../interfaces/actorDao';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';

export class ActorJsonDAO implements ActorDAO {
    private db = new JsonDBAbstract<ActorDTO>('actor.json');

    async getActors(): Promise<ActorDTO[]> {
        return this.db.getAll();
    }
    async getActor(id: number): Promise<ActorDTO | undefined> {
        return this.db.getAll().find((item) => item.id === id);
    }
}
