import type { UserDTO } from '../../dto/user';

export interface UserDAO {
    getUsers(): Promise<UserDTO[]>;
    getUser(id: number): Promise<UserDTO | undefined>;
    createUser(user: UserDTO): Promise<bigint>;
    existsByNameOrEmail(name: string, email: string): Promise<boolean>;
    getUserByUsername(username: string): Promise<UserDTO | undefined>;
}
