import { StarredMovieDTO } from '../../data/dto/starred_movie';
import { StarredMovieDomainModel } from '../models/starred_movieDomainModel';
import type { BaseMapper } from './baseMapper';

export class StarredMovieMapper implements BaseMapper {
    toDomainModel(dto: StarredMovieDTO): StarredMovieDomainModel {
        return new StarredMovieDomainModel(dto.movieId, dto.userId);
    }
    toDTO(domain: StarredMovieDomainModel): StarredMovieDTO {
        return new StarredMovieDTO(domain.movieId, domain.userId);
    }
}
