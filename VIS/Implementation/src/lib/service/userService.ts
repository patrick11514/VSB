import { isValid } from '../functions';
import { UserGateway } from '../gateway/userGateway';

export class UserService {
    private gateway = new UserGateway();

    async registerUser(username: string, email: string, password: string) {
        return await this.gateway.registerUser(username, email, password);
    }

    async loginUser(username: string, password: string) {
        return await this.gateway.loginUser(username, password);
    }
}
