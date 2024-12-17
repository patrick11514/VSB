import { conn } from '$/lib/server/variables';
import { UserDTO } from '../../dto/user';
import type { UserDAO } from '../interfaces/userDao';

export class UserMariaDAO implements UserDAO {
    async getUsers(): Promise<UserDTO[]> {
        const data = await conn.selectFrom('user').selectAll().execute();
        return data.map((item) => {
            return new UserDTO(item.id, item.username, item.email, item.password);
        });
    }
    async getUser(id: number): Promise<UserDTO | undefined> {
        const data = await conn.selectFrom('user').selectAll().where('id', '=', id).executeTakeFirst();
        if (!data) return data;
        return new UserDTO(data.id, data.username, data.email, data.password);
    }
    async createUser(user: UserDTO): Promise<bigint> {
        const data = await conn
            .insertInto('user')
            .values({
                username: user.username,
                email: user.email,
                password: user.password
            })
            .executeTakeFirst();
        return data.insertId ?? BigInt(-1);
    }
}
