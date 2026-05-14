<script lang="ts">
	import { goto } from '$app/navigation';
	import AuctionList from '$lib/components/AuctionList.svelte';
	import AuctionSearch from '$lib/components/AuctionSearch.svelte';
	import { Button } from '$lib/components/ui/button';
	import type { Auction } from '$lib/types';
	import { Plus } from 'lucide-svelte';
	import { onMount } from 'svelte';

	let auctions = $state<Auction[]>([]);
	let isLoading = $state(false);

	async function fetchAuctions() {
		isLoading = true;
		try {
			const response = await fetch('/api/auctions');
			if (response.ok) {
				const data = await response.json();
				auctions = Array.isArray(data) ? data : [];
			}
		} catch (err) {
			console.error('Failed to fetch auctions:', err);
			auctions = [];
		} finally {
			isLoading = false;
		}
	}

	function handleSearch(query: string, type: 'name' | 'address') {
		console.log('Search:', query, type);
		// Implement search logic later
	}

	function handleSort(sortBy: 'newest' | 'highest-bid' | 'ending-soon') {
		console.log('Sort:', sortBy);
		// Implement sort logic later
	}

	function handleCreateAuction() {
		goto('/create-auction');
	}

	const onSearch = handleSearch;
	const onSort = handleSort;

	onMount(() => {
		void fetchAuctions();
	});
</script>

<div class="bg-slate-50 px-4 py-8">
	<main class="container mx-auto">
		<div class="space-y-6">
			<!-- Action Bar -->
			<div class="flex items-center justify-between">
				<div>
					<h2 class="text-xl font-semibold text-slate-900">Active Auctions</h2>
					<p class="text-sm text-slate-600">Browse and bid on encrypted digital assets</p>
				</div>
				<Button onclick={handleCreateAuction}>
					<Plus class="mr-2 h-4 w-4" />
					Create Auction
				</Button>
			</div>

			<!-- Search & Filter -->
			<AuctionSearch {onSearch} {onSort} />

			<!-- Auction List -->
			<AuctionList {auctions} {isLoading} />
		</div>
	</main>
</div>
