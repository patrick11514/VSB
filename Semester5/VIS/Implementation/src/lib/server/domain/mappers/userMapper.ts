import { UserDTO } from '../../data/dto/user';
import { UserDomainModel } from '../models/userDomainModel';
import type { BaseMapper } from './baseMapper';

export class UserMapper implements BaseMapper {
    toDomainModel(dto: UserDTO): UserDomainModel {
        return new UserDomainModel(dto.id, dto.username, dto.email, dto.password);
    }
    toDTO(domain: UserDomainModel): UserDTO {
        return new UserDTO(domain.id, domain.username, domain.email, domain.password);
    }
}
