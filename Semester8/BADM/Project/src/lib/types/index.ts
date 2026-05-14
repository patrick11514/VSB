export interface Auction {
	id: string;
	address?: string;
	name: string;
	description: string;
	endAt: string | number;
	maxBid?: number;
	owner: string;
	ipfsHandle?: string;
	originalFileHash?: string;
	hashedFileHash?: string;
	minBid?: string | number;
	createdAt?: string;
}
