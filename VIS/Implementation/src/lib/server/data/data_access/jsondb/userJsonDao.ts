import { UserDTO } from '../../dto/user';
import { JsonDBAbstract } from '../helpers/jsonDBAbstract';
import type { UserDAO } from '../interfaces/userDao';

export class UserJsonDAO implements UserDAO {
    private db = new JsonDBAbstract<UserDTO>('user.json');

    async getUsers(): Promise<UserDTO[]> {
        return this.db.getAll();
    }
    async getUser(id: number): Promise<UserDTO | undefined> {
        return this.db.getAll().find((item) => item.id === id);
    }
    async createUser(user: UserDTO): Promise<bigint> {
        return this.db.addItem(user);
    }

    async existsByNameOrEmail(name: string, email: string): Promise<boolean> {
        return this.db.getAll().some((item) => item.username === name || item.email === email);
    }

    async getUserByUsername(username: string): Promise<UserDTO | undefined> {
        return this.db.getAll().find((item) => item.username === username);
    }
}
