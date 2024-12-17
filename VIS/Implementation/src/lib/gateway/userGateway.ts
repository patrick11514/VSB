import { API } from '../api';
import type { UserDomainModel } from '../server/domain/models/userDomainModel';

export class UserGateway {
    private cachedUsers: UserDomainModel[] = [];

    async registerUser(username: string, email: string, password: string) {
        return API.auth.register({
            username,
            email,
            password
        });
    }

    async loginUser(username: string, password: string) {
        return API.auth.login({
            username,
            password
        });
    }

    async getUser(id: number) {
        const find = this.cachedUsers.find((user) => user.id === id);
        if (find) return { status: true, data: find };
        return API.user(id);
    }
}
