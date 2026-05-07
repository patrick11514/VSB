<script lang="ts">
	import { goto } from '$app/navigation';
	import { Button } from '$lib/components/ui/button';
	import { Card, CardContent, CardHeader, CardTitle } from '$lib/components/ui/card';
	import { Input } from '$lib/components/ui/input';
	import { deployAuctionContract } from '$lib/services/auction-contract';
	import { sha256Hex } from '$lib/services/crypto';
	import { formatEth } from '$lib/services/web3';
	import { provider, walletAddress } from '$lib/stores/session';

	const defaultEndAt = new Date(Date.now() + 2 * 24 * 60 * 60 * 1000).toISOString().slice(0, 16);

	let name = $state('Example contract');
	let description = $state('Simple contract presentation auction');
	let endAt = $state(defaultEndAt);
	let ipfsHandle = $state('ipfs://example-handle');
	let passphrase = $state('open-sesame');
	let isSubmitting = $state(false);
	let errorMessage = $state('');
	let successMessage = $state('');

	async function handleSubmit(event: SubmitEvent) {
		event.preventDefault();
		errorMessage = '';
		successMessage = '';

		if (!$provider) {
			errorMessage = 'Connect MetaMask first.';
			return;
		}

		if (!name.trim() || !description.trim() || !ipfsHandle.trim() || !passphrase.trim() || !endAt) {
			errorMessage = 'All fields are required.';
			return;
		}

		isSubmitting = true;
		try {
			const signer = await $provider.getSigner();
			const passphraseHash = `0x${await sha256Hex(passphrase.trim())}`;
			const originalFileHash = `0x${await sha256Hex(
				`${name.trim()}|${description.trim()}|${ipfsHandle.trim()}|${passphraseHash}`
			)}`;
			const endTimestamp = BigInt(Math.floor(new Date(endAt).getTime() / 1000));

			const address = await deployAuctionContract(signer, {
				minBid: 0n,
				name: name.trim(),
				description: description.trim(),
				originalFileHash,
				ipfsHandle: ipfsHandle.trim(),
				passphraseHash,
				endAt: endTimestamp
			});

			successMessage = `Auction deployed at ${address}`;
			await goto(`/auction/${address}`);
		} catch (error) {
			errorMessage = error instanceof Error ? error.message : 'Failed to create auction.';
		} finally {
			isSubmitting = false;
		}
	}
</script>

<svelte:head>
	<title>Create Auction</title>
</svelte:head>

<div class="min-h-screen bg-slate-50 px-4 py-10">
	<div class="mx-auto flex max-w-3xl flex-col gap-6">
		<div>
			<p class="text-sm font-medium tracking-[0.25em] text-slate-500 uppercase">Create</p>
			<h1 class="text-3xl font-bold text-slate-950">Deploy contract</h1>
		</div>

		<Card>
			<CardHeader>
				<CardTitle>New auction</CardTitle>
			</CardHeader>
			<CardContent>
				<form class="space-y-5" onsubmit={handleSubmit}>
					<div class="grid gap-2">
						<label class="text-sm font-medium text-slate-700" for="name">Name</label>
						<Input id="name" bind:value={name} placeholder="Auction name" />
					</div>

					<div class="grid gap-2">
						<label class="text-sm font-medium text-slate-700" for="description">Description</label>
						<textarea
							id="description"
							bind:value={description}
							rows="4"
							class="min-h-28 rounded-md border border-input bg-transparent px-3 py-2 text-sm shadow-xs transition-colors outline-none placeholder:text-muted-foreground focus-visible:border-ring focus-visible:ring-3 focus-visible:ring-ring/50"
							placeholder="What can the contract owner present?"
						></textarea>
					</div>

					<div class="grid gap-2">
						<label class="text-sm font-medium text-slate-700" for="endAt">End date</label>
						<Input id="endAt" bind:value={endAt} type="datetime-local" />
					</div>

					<div class="grid gap-2">
						<label class="text-sm font-medium text-slate-700" for="ipfsHandle">IPFS handle</label>
						<Input id="ipfsHandle" bind:value={ipfsHandle} placeholder="ipfs://..." />
					</div>

					<div class="grid gap-2">
						<label class="text-sm font-medium text-slate-700" for="passphrase">Passphrase</label>
						<Input
							id="passphrase"
							bind:value={passphrase}
							type="password"
							placeholder="Secret passphrase"
						/>
					</div>

					<div
						class="rounded-lg border border-dashed border-slate-200 bg-slate-100/70 p-4 text-sm text-slate-600"
					>
						<p>Connected wallet: {$walletAddress ?? 'Not connected'}</p>
						<p class="mt-1">Min bid: {formatEth(0n)} ETH</p>
					</div>

					{#if errorMessage}
						<div class="rounded-md border border-red-200 bg-red-50 px-3 py-2 text-sm text-red-700">
							{errorMessage}
						</div>
					{/if}

					{#if successMessage}
						<div
							class="rounded-md border border-emerald-200 bg-emerald-50 px-3 py-2 text-sm text-emerald-700"
						>
							{successMessage}
						</div>
					{/if}

					<div class="flex items-center justify-end gap-3">
						<Button type="submit" disabled={isSubmitting}>
							{isSubmitting ? 'Deploying...' : 'Deploy auction'}
						</Button>
					</div>
				</form>
			</CardContent>
		</Card>
	</div>
</div>
