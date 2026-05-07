<script lang="ts">
	import type { Auction } from '$lib/types';
	import AuctionCard from './AuctionCard.svelte';

	let { auctions = [], isLoading = false } = $props<{
		auctions?: Auction[];
		isLoading?: boolean;
	}>();

	const placeholderAuctions: Auction[] = [
		{
			id: '0x742d35Cc6634C0532925a3b844Bc92d426B0A93d',
			address: '0x742d35Cc6634C0532925a3b844Bc92d426B0A93d',
			name: 'Digital Art NFT Collection',
			description: 'Rare digital art collection with 10 unique pieces',
			endAt: Math.floor(Date.now() / 1000) + 86400 * 2,
			maxBid: 5.5,
			owner: '0x742d35Cc6634C0532925a3b844Bc92d426B0A93d'
		},
		{
			id: '0x2222222222222222222222222222222222222222',
			address: '0x2222222222222222222222222222222222222222',
			name: 'Trading Bot Source Code',
			description: 'Algorithmic trading bot for Uniswap swaps',
			endAt: Math.floor(Date.now() / 1000) + 3600,
			maxBid: 2.1,
			owner: '0x2222222222222222222222222222222222222222'
		},
		{
			id: '0x1234567890123456789012345678901234567890',
			address: '0x1234567890123456789012345678901234567890',
			name: 'ML Model for Price Prediction',
			description: 'Trained ML model for crypto price forecasting',
			endAt: Math.floor(Date.now() / 1000) + 86400 * 7,
			maxBid: 3.8,
			owner: '0x1234567890123456789012345678901234567890'
		}
	];

	const displayAuctions: Auction[] = $derived(
		auctions && auctions.length > 0 ? auctions : placeholderAuctions
	);
</script>

<div class="space-y-4">
	{#if isLoading}
		<div class="py-8 text-center">
			<p class="text-slate-600">Loading auctions...</p>
		</div>
	{:else if displayAuctions.length === 0}
		<div class="py-8 text-center">
			<p class="text-slate-600">No auctions found.</p>
		</div>
	{:else}
		<div class="grid gap-4 md:grid-cols-2 lg:grid-cols-3">
			{#each displayAuctions as auction (auction.id)}
				<AuctionCard {auction} />
			{/each}
		</div>
	{/if}
</div>
