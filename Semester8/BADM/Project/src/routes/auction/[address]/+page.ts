import { error } from '@sveltejs/kit';
import { isValidAuctionAddress } from '$lib/services/auction-contract';

export function load({ params }) {
	if (!isValidAuctionAddress(params.address)) {
		throw error(404, 'Invalid auction address');
	}

	return {
		address: params.address
	};
}
