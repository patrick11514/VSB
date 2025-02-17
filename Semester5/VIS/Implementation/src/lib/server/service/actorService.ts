import { MovieDTO } from '../data/dto/movie';
import { GlobalConnector } from '../data/globalConnector';
import { ActorMapper } from '../domain/mappers/actorMapper';

export class ActorService {
    private mapper = new ActorMapper();

    async getActors(movieId?: number) {
        if (movieId !== undefined) {
            const castDto = await GlobalConnector.connector.castDao.getCasts(new MovieDTO(movieId, '', '', 0));
            const actorsDto = await Promise.all(castDto.map((cast) => GlobalConnector.connector.actorDao.getActor(cast.actorId)));
            return actorsDto.filter((actor) => actor !== undefined).map(this.mapper.toDomainModel);
        }

        const dto = await GlobalConnector.connector.actorDao.getActors();
        return dto.map(this.mapper.toDomainModel);
    }
}
