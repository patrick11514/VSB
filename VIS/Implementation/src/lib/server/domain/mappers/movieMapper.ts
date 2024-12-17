import { MovieDTO } from '../../data/dto/movie';
import { MovieDomainModel } from '../models/movieDomainModel';
import type { BaseMapper } from './baseMapper';

export class MovieMapper implements BaseMapper {
    toDomainModel(dto: MovieDTO): MovieDomainModel {
        return new MovieDomainModel(dto.id, dto.name, dto.studio);
    }
    toDTO(domain: MovieDomainModel): MovieDTO {
        return new MovieDTO(domain.id, domain.name, domain.studio);
    }
}
