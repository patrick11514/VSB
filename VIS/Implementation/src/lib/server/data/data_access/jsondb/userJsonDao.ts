import { UserDTO } from '../../dto/user';
import { GlobalConnector } from '../../globalConnector';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { UserDAO } from '../interfaces/userDao';
import path from 'node:path';

export class UserJsonDAO implements UserDAO {
    private db = new JsonDBAbstract<UserDTO>(path.join(GlobalConnector.jsonDBPath, 'user.json'));

    async getUsers(): Promise<UserDTO[]> {
        return this.db.getAll();
    }
    async getUser(id: number): Promise<UserDTO | undefined> {
        return this.db.getAll().find((item) => item.id === id);
    }
    async createUser(user: UserDTO): Promise<bigint> {
        return this.db.addItem(user);
    }
}
