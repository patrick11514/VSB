<script lang="ts">
	import { goto } from '$app/navigation';
	import AuctionList from '$lib/components/AuctionList.svelte';
	import AuctionSearch from '$lib/components/AuctionSearch.svelte';
	import { Badge } from '$lib/components/ui/badge';
	import { resetAuth, walletAddress } from '$lib/stores/session';
	import type { Auction } from '$lib/types';
	import { LogOut, Plus } from 'lucide-svelte';

	let auctions: Auction[] = [];
	let isLoading = false;

	function formatAddress(addr: string) {
		return `${addr.slice(0, 6)}...${addr.slice(-4)}`;
	}

	async function handleDisconnect() {
		resetAuth();
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
</script>

<div class="min-h-screen bg-slate-50">
	<!-- Header -->
	<header class="sticky top-0 z-40 border-b bg-white shadow-sm">
		<div class="container mx-auto flex items-center justify-between px-4 py-4">
			<div>
				<h1 class="text-2xl font-bold text-slate-900">EtherZar</h1>
				<p class="text-xs text-slate-500">Ethereum Bazaar for Encrypted Digital Assets</p>
			</div>

			<div class="flex items-center gap-4">
				{#if $walletAddress}
					<div class="flex items-center gap-2">
						<Badge variant="secondary">{formatAddress($walletAddress)}</Badge>
					</div>
					<button
						class="inline-flex h-9 items-center justify-center rounded-md border border-input bg-background px-2.5 text-sm font-medium whitespace-nowrap hover:bg-accent hover:text-accent-foreground focus-visible:outline-none disabled:pointer-events-none disabled:opacity-50"
						onclick={handleDisconnect}
					>
						<LogOut class="mr-2 h-4 w-4" />
						Disconnect
					</button>
				{/if}
			</div>
		</div>
	</header>

	<!-- Main Content -->
	<main class="container mx-auto px-4 py-8">
		<div class="space-y-6">
			<!-- Action Bar -->
			<div class="flex items-center justify-between">
				<div>
					<h2 class="text-xl font-semibold text-slate-900">Active Auctions</h2>
					<p class="text-sm text-slate-600">Browse and bid on encrypted digital assets</p>
				</div>
				<button
					class="inline-flex h-10 items-center justify-center rounded-md bg-primary px-8 text-sm font-medium whitespace-nowrap text-primary-foreground hover:bg-primary/90 focus-visible:outline-none disabled:pointer-events-none disabled:opacity-50"
					onclick={handleCreateAuction}
				>
					<Plus class="mr-2 h-4 w-4" />
					Create Auction
				</button>
			</div>

			<!-- Search & Filter -->
			<AuctionSearch {onSearch} {onSort} />

			<!-- Auction List -->
			<AuctionList {auctions} {isLoading} />
		</div>
	</main>
</div>
