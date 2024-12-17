export class ReviewDTO {
    constructor(
        public id: number,
        public user: number,
        public movie: number,
        public text: string
    ) { }
}
