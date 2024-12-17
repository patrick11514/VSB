import { ReviewDTO } from '../../data/dto/review';
import { ReviewDomainModel } from '../models/reviewDomainModel';
import type { BaseMapper } from './baseMapper';

export class ReviewMapper implements BaseMapper {
    toDomainModel(dto: ReviewDTO): ReviewDomainModel {
        return new ReviewDomainModel(dto.id, dto.user, dto.movie, dto.text);
    }
    toDTO(domain: ReviewDomainModel): ReviewDTO {
        return new ReviewDTO(domain.id, domain.user, domain.movie, domain.text);
    }
}
