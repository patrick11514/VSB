<script lang="ts">
	import { Badge } from '$lib/components/ui/badge';
	import { Button } from '$lib/components/ui/button';
	import {
		Card,
		CardContent,
		CardDescription,
		CardHeader,
		CardTitle
	} from '$lib/components/ui/card';
	import type { Auction } from '$lib/types';
	import { formatDistanceToNow } from 'date-fns';

	let { auction } = $props<{ auction: Auction }>();

	const isEnded = $derived(Date.now() > auction.endAt);
	const timeRemaining = $derived(Math.max(auction.endAt, Date.now()));
	const auctionAddress = $derived(auction.address ?? auction.id);
	const detailHref = $derived(`/auction/${auctionAddress}`);
</script>

<Card class="transition-shadow hover:shadow-lg">
	<CardHeader>
		<div class="flex items-start justify-between">
			<div class="flex-1">
				<CardTitle class="text-lg">{auction.name}</CardTitle>
				<CardDescription class="mt-1 text-xs"
					>{auction.owner.slice(0, 6)}...{auction.owner.slice(-4)}</CardDescription
				>
			</div>
			{#if isEnded}
				<Badge variant="secondary">Ended</Badge>
			{:else}
				<Badge variant="outline">Bidding</Badge>
			{/if}
		</div>
	</CardHeader>
	<CardContent class="space-y-4">
		<p class="line-clamp-2 text-sm text-slate-600">{auction.description}</p>

		<div class="grid grid-cols-2 gap-4 text-sm">
			<div>
				<p class="text-xs text-slate-500">Highest Bid</p>
				<p class="font-semibold">{auction.maxBid} ETH</p>
			</div>
			<div>
				<p class="text-xs text-slate-500">Time Left</p>
				<p class="text-xs font-semibold">
					{formatDistanceToNow(timeRemaining, { addSuffix: true })}
				</p>
			</div>
		</div>

		<Button class="w-full" href={detailHref} variant="outline">View Auction</Button>
	</CardContent>
</Card>
