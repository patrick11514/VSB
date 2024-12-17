export class UserDomainModel {
    constructor(
        public id: number,
        public username: string,
        public email: string,
        public password: string
    ) { }
}
