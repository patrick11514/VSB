import { HASH_ROUNDS } from '$env/static/private';
import { GlobalConnector } from '../data/globalConnector';
import { UserMapper } from '../domain/mappers/userMapper';
import type { UserDomainModel } from '../domain/models/userDomainModel';
import bcrypt from 'bcrypt';

export class UserService {
    private mapper = new UserMapper();

    async createUser(user: UserDomainModel) {
        //hash password
        user.password = bcrypt.hashSync(user.password, parseInt(HASH_ROUNDS));
        const dto = this.mapper.toDTO(user);
        return GlobalConnector.connector.userDao.createUser(dto);
    }

    async exists(name: string, email: string) {
        const usersDTO = await GlobalConnector.connector.userDao.getUsers();
        const users = usersDTO.map(this.mapper.toDomainModel);
        return users.some((user) => user.username === name || user.email === email);
    }
}
