import { HASH_ROUNDS } from '$env/static/private';
import { GlobalConnector } from '../data/globalConnector';
import { UserMapper } from '../domain/mappers/userMapper';
import type { UserDomainModel } from '../domain/models/userDomainModel';
import bcrypt from 'bcrypt';
import { jwt } from '../variables';
import type { UserData } from '$/types/types';

export class UserService {
    private mapper = new UserMapper();

    async createUser(user: UserDomainModel) {
        //hash password
        user.password = bcrypt.hashSync(user.password, parseInt(HASH_ROUNDS));
        const dto = this.mapper.toDTO(user);
        return GlobalConnector.connector.userDao.createUser(dto);
    }

    async exists(name: string, email: string) {
        return GlobalConnector.connector.userDao.existsByNameOrEmail(name, email);
    }

    async login(username: string, password: string) {
        const dto = await GlobalConnector.connector.userDao.getUserByUsername(username);
        if (!dto) {
            return undefined;
        }

        const user = this.mapper.toDomainModel(dto);

        if (!bcrypt.compareSync(password, user.password)) {
            return undefined;
        }

        const userData = {
            id: user.id,
            username: user.username,
            email: user.email
        } satisfies UserData;

        return {
            cookie: jwt.setCookie(userData),
            data: userData
        };
    }
}
