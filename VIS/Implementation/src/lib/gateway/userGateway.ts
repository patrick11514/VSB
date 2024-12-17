import { API } from '../api';

export class UserGateway {
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
}
