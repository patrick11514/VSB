import { conn } from '$/lib/server/variables';
import { ActorDTO } from '../../dto/actor';
import type { ActorDAO } from '../interfaces/actorDao';

export class ActorMariaDAO implements ActorDAO {
    async getActors(): Promise<ActorDTO[]> {
        const data = await conn.selectFrom('actor').selectAll().execute();
        return data.map((item) => {
            return new ActorDTO(item.id, item.firstname, item.lastname, item.age);
        });
    }
    async getActor(id: number): Promise<ActorDTO | undefined> {
        const data = await conn.selectFrom('actor').selectAll().where('id', '=', id).executeTakeFirst();
        if (!data) return data;

        return new ActorDTO(data.id, data.firstname, data.lastname, data.age);
    }
}
