import type { RatingDTO } from './ratingDTO';
import type { ReviewDTO } from './reviewDTO';

export class MovieDetailedDTO {
    constructor(
        public id: number,
        public name: string,
        public studio: string,
        public year: number,
        public ratings: RatingDTO[],
        public reviews: ReviewDTO[]
    ) {}
}
