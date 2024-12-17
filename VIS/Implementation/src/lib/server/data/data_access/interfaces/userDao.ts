import type { UserDTO } from '../../dto/user';

export interface UserDAO {
    getUsers(): Promise<UserDTO[]>;
    getUser(id: number): Promise<UserDTO | undefined>;
    createUser(user: UserDTO): Promise<void>;
}
