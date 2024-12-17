export class RatingDomainModel {
    constructor(
        public id: number,
        public user: number,
        public movie: number,
        public rating: number
    ) { }
}
