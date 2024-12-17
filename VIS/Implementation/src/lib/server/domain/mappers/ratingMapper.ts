import { RatingDTO } from '../../data/dto/rating';
import { RatingDomainModel } from '../models/ratingDomainModel';
import type { BaseMapper } from './baseMapper';

export class RatingMapper implements BaseMapper {
    toDomainModel(dto: RatingDTO): RatingDomainModel {
        return new RatingDomainModel(dto.id, dto.user, dto.movie, dto.rating);
    }
    toDTO(domain: RatingDomainModel): RatingDTO {
        return new RatingDTO(domain.id, domain.user, domain.movie, domain.rating);
    }
}
