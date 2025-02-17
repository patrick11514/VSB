import { CastDTO } from '../../data/dto/cast';
import { CastDomainModel } from '../models/castDomainModel';
import type { BaseMapper } from './baseMapper';

export class CastMapper implements BaseMapper {
    toDomainModel(dto: CastDTO): CastDomainModel {
        return new CastDomainModel(dto.actorId, dto.movieId);
    }
    toDTO(domain: CastDomainModel): CastDTO {
        return new CastDTO(domain.actorId, domain.movieId);
    }
}
