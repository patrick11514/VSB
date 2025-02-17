import type { ActorDTO } from '../../dto/actor';

export interface ActorDAO {
    getActors(): Promise<ActorDTO[]>;
    getActor(id: number): Promise<ActorDTO | undefined>;
}
