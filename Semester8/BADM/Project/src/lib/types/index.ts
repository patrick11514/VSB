export interface Auction {
	id: string;
	address?: string;
	name: string;
	description: string;
	endAt: number;
	maxBid: number;
	owner: string;
}
