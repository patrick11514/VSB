<script lang="ts">
	import type { Auction } from '$lib/types';
	import AuctionCard from './AuctionCard.svelte';

	let { auctions = [], isLoading = false } = $props<{
		auctions?: Auction[];
		isLoading?: boolean;
	}>();

	console.log(auctions);

	const displayAuctions: Auction[] = $derived(auctions && auctions.length > 0 ? auctions : []);
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
			{#each displayAuctions as auction (auction.address)}
				<AuctionCard {auction} />
			{/each}
		</div>
	{/if}
</div>
