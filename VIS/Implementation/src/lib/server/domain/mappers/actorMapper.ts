import { ActorDTO } from '../../data/dto/actor';
import { ActorDomainModel } from '../models/actorDomainModel';
import type { BaseMapper } from './baseMapper';

export class ActorMapper implements BaseMapper {
    toDomainModel(dto: ActorDTO): ActorDomainModel {
        return new ActorDomainModel(dto.id, dto.firstName, dto.lastName, dto.age);
    }
    toDTO(domain: ActorDomainModel): ActorDTO {
        return new ActorDTO(domain.id, domain.firstName, domain.lastName, domain.age);
    }
}
