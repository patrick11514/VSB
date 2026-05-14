<script lang="ts">
	import CardFooter from '$/lib/components/ui/card/card-footer.svelte';
	import { browser } from '$app/environment';
	import { Button } from '$lib/components/ui/button';
	import {
		Card,
		CardContent,
		CardDescription,
		CardHeader,
		CardTitle
	} from '$lib/components/ui/card';
	import { Input } from '$lib/components/ui/input';
	import {
		bidOnAuction,
		endAuction,
		formatAuctionState,
		getAuctionPermissions,
		getWinnerFileDetails,
		readAuction,
		refundAuction,
		revealPassphrase,
		type AuctionDetails,
		type AuctionRevealDetails
	} from '$lib/services/auction-contract';
	import { ipfsToGatewayUrl } from '$lib/services/ipfs';
	import { formatEth, parseEth } from '$lib/services/web3';
	import {
		accountSwitchPromptOpen,
		pendingWalletAddress,
		provider,
		walletAddress
	} from '$lib/stores/session';
	import { onMount } from 'svelte';

	let { data } = $props<{ data: { address: string } }>();

	let auction = $state<AuctionDetails | null>(null);
	let isOwner = $state(false);
	let isWinner = $state(false);
	let canEndAuction = $state(false);
	let refundAmount = $state(0n);
	let isLoading = $state(true);
	let actionMessage = $state('');
	let actionError = $state('');
	let bidAmount = $state('0.1');
	let revealPassphraseInput = $state('');
	let isWorking = $state(false);
	let winnerDetails = $state<AuctionRevealDetails | null>(null);
	let isFilePopupOpen = $state(false);
	let hasRejectedAccountSwitch = $state(false);
	let htmlDescription = $state('');

	function isActionsBlocked() {
		return hasRejectedAccountSwitch && Boolean($pendingWalletAddress);
	}

	async function loadAuction() {
		actionError = '';
		isOwner = false;
		isWinner = false;
		canEndAuction = false;
		refundAmount = 0n;
		if (!$provider) {
			isLoading = false;
			return;
		}

		isLoading = true;
		try {
			const signer = await $provider.getSigner();
			const [auctionData, permissions] = await Promise.all([
				readAuction(data.address, signer),
				getAuctionPermissions(data.address, signer)
			]);

			auction = auctionData;
			// render description as sanitized Markdown on client
			if (browser) {
				try {
					const m = await import('marked');
					const d = await import('dompurify');
					const mdToHtml = m.marked?.parse ?? m.parse ?? ((s: string) => s);
					htmlDescription = d.default.sanitize(mdToHtml(auction.description || ''));
					//eslint-disable-next-line
				} catch (err) {
					htmlDescription = auction.description || '';
				}
			}
			isOwner = permissions.isOwner;
			isWinner = permissions.isWinner;
			canEndAuction = permissions.canEndAuction;
			refundAmount = permissions.refundAmount;
			return permissions;
		} catch (error) {
			actionError = error instanceof Error ? error.message : 'Failed to load auction.';
		} finally {
			isLoading = false;
		}
	}

	async function performAction(action: () => Promise<void>, successMessage: string) {
		actionError = '';
		actionMessage = '';
		isWorking = true;
		try {
			await action();
			actionMessage = successMessage;
			await loadAuction();
		} catch (error) {
			actionError = error instanceof Error ? error.message : 'Action failed.';
		} finally {
			isWorking = false;
		}
	}

	async function handleBid() {
		if (isActionsBlocked()) {
			actionError = 'Please accept the switched MetaMask account to continue.';
			return;
		}

		if (!$provider || !auction) {
			return;
		}

		const signer = await $provider.getSigner();
		await performAction(
			() => bidOnAuction(data.address, signer, parseEth(bidAmount.toString())),
			'Bid submitted.'
		);
	}

	async function handleEndAuction() {
		if (isActionsBlocked()) {
			actionError = 'Please accept the switched MetaMask account to continue.';
			return;
		}

		if (!$provider) {
			return;
		}

		const signer = await $provider.getSigner();
		await performAction(() => endAuction(data.address, signer), 'Auction ended.');
	}

	async function handleRevealPassphrase() {
		if (isActionsBlocked()) {
			actionError = 'Please accept the switched MetaMask account to continue.';
			return;
		}

		if (!$provider) {
			return;
		}

		if (!revealPassphraseInput.trim()) {
			actionError = 'Enter the passphrase before revealing it.';
			return;
		}

		const signer = await $provider.getSigner();
		await performAction(
			() => revealPassphrase(data.address, signer, revealPassphraseInput.trim()),
			'Passphrase revealed.'
		);
		revealPassphraseInput = '';
	}

	async function handleRefund() {
		if (isActionsBlocked()) {
			actionError = 'Please accept the switched MetaMask account to continue.';
			return;
		}

		if (!$provider) {
			return;
		}

		const signer = await $provider.getSigner();
		await performAction(() => refundAuction(data.address, signer), 'Refund requested.');
	}

	async function openWinnerDetails() {
		if (isActionsBlocked()) {
			actionError = 'Please accept the switched MetaMask account to continue.';
			return;
		}

		if (!$provider) {
			return;
		}

		const signer = await $provider.getSigner();
		winnerDetails = await getWinnerFileDetails(data.address, signer);
		isFilePopupOpen = true;
	}

	onMount(() => {
		void loadAuction();
	});

	async function acceptAccountSwitch() {
		if (!$pendingWalletAddress) {
			accountSwitchPromptOpen.set(false);
			hasRejectedAccountSwitch = false;
			return;
		}

		walletAddress.set($pendingWalletAddress);
		pendingWalletAddress.set(null);
		accountSwitchPromptOpen.set(false);
		hasRejectedAccountSwitch = false;
		await loadAuction();
	}

	function rejectAccountSwitch() {
		accountSwitchPromptOpen.set(false);
		hasRejectedAccountSwitch = true;
	}
</script>

<svelte:head>
	<title>Auction {data.address}</title>
</svelte:head>

<div class="bg-slate-50 px-4 py-10">
	<div class="mx-auto flex max-w-5xl flex-col gap-6">
		<div>
			<p class="text-sm font-medium tracking-[0.25em] text-slate-500 uppercase">Auction</p>
			<h1 class="text-3xl font-bold text-slate-950">Contract details</h1>
			<p class="mt-2 text-sm text-slate-600">{data.address}</p>
		</div>

		{#if isLoading}
			<Card>
				<CardContent class="py-8 text-center text-sm text-slate-600">Loading auction...</CardContent
				>
			</Card>
		{:else if auction}
			<div class="grid gap-6 lg:grid-cols-[1.4fr_0.9fr]">
				<Card>
					<CardHeader>
						<div class="flex items-center justify-between gap-4">
							<CardTitle class="text-2xl">{auction.name}</CardTitle>
							<div
								class="rounded-full border border-slate-200 bg-slate-100 px-3 py-1 text-xs font-medium text-slate-700"
							>
								{formatAuctionState(auction.state)}
							</div>
						</div>
					</CardHeader>
					<CardContent class="space-y-5">
						<div class="grid gap-4 sm:grid-cols-2">
							<div class="rounded-lg border border-slate-200 bg-white p-4">
								<p class="text-xs tracking-wide text-slate-500 uppercase">Highest bid</p>
								<p class="mt-2 text-lg font-semibold text-slate-950">
									{formatEth(auction.maxBid)} ETH
								</p>
							</div>
							<div class="rounded-lg border border-slate-200 bg-white p-4">
								<p class="text-xs tracking-wide text-slate-500 uppercase">End date</p>
								<p class="mt-2 text-sm font-medium text-slate-950">
									{new Date(Number(auction.endAt) * 1000).toLocaleString()}
								</p>
							</div>
						</div>

						<div class="grid gap-3 text-sm text-slate-600">
							<p><span class="font-medium text-slate-950">Owner:</span> {auction.owner}</p>
							<p>
								<span class="font-medium text-slate-950">Max bid:</span>
								{auction.maxBid || 'None yet'}
							</p>
							<div class="break-all">
								<p>
									<span class="font-medium text-slate-950">IPFS handle:</span>
								</p>
								<a
									target="_blank"
									class="break-all text-blue-600 underline"
									href={ipfsToGatewayUrl(auction?.ipfsHandle ?? '')}>{auction.ipfsHandle}</a
								>
							</div>
							<p class="break-all">
								<span class="font-medium text-slate-950">Hashed file hash:</span>
								{auction.hashedFileHash}
							</p>
						</div>

						{#if actionMessage}
							<div
								class="rounded-md border border-emerald-200 bg-emerald-50 px-3 py-2 text-sm text-emerald-700"
							>
								{actionMessage}
							</div>
						{/if}

						{#if actionError}
							<div
								class="rounded-md border border-red-200 bg-red-50 px-3 py-2 text-sm text-red-700"
							>
								{actionError}
							</div>
						{/if}
					</CardContent>
					<hr />
					<CardFooter>
						{#if htmlDescription}
							<!-- eslint-disable-next-line -->
							<div class="prose mt-1 max-w-none text-sm">{@html htmlDescription}</div>
						{:else}
							<CardDescription class="mt-1">{auction.description}</CardDescription>
						{/if}
					</CardFooter>
				</Card>

				<div class="space-y-6">
					{#if auction.state === 0 && !isOwner}
						<Card>
							<CardHeader>
								<CardTitle>Bid</CardTitle>
								<CardDescription>Place a higher offer than the current bid.</CardDescription>
							</CardHeader>
							<CardContent class="space-y-4">
								<div class="grid gap-2">
									<label class="text-sm font-medium text-slate-700" for="bidAmount"
										>Bid amount in ETH</label
									>
									<Input id="bidAmount" bind:value={bidAmount} type="number" min="0" step="0.01" />
								</div>
								<Button
									class="w-full"
									disabled={isWorking || isActionsBlocked()}
									onclick={handleBid}>Submit bid</Button
								>
							</CardContent>
						</Card>
					{/if}

					{#if refundAmount > 0n}
						<Card>
							<CardHeader>
								<CardTitle>Refund</CardTitle>
								<CardDescription>
									You have {formatEth(refundAmount)} ETH available to withdraw.
								</CardDescription>
							</CardHeader>
							<CardContent class="space-y-3">
								<p class="text-sm text-slate-600">
									Refund amount: {formatEth(refundAmount)} ETH
								</p>
								<Button
									class="w-full"
									variant="outline"
									disabled={isWorking || isActionsBlocked()}
									onclick={handleRefund}
								>
									Get money back
								</Button>
							</CardContent>
						</Card>
					{/if}

					{#if isOwner}
						<Card>
							<CardHeader>
								<CardTitle>Owner actions</CardTitle>
								<CardDescription
									>End the auction or reveal the passphrase once bidding is closed.</CardDescription
								>
							</CardHeader>
							<CardContent class="space-y-3">
								{#if auction.state === 0}
									{#if canEndAuction}
										<Button
											class="w-full"
											disabled={isWorking || isActionsBlocked()}
											onclick={handleEndAuction}
										>
											End auction
										</Button>
									{/if}
								{/if}
								{#if auction.state === 1}
									<div class="grid gap-2">
										<label class="text-sm font-medium text-slate-700" for="revealPassphrase"
											>Reveal passphrase</label
										>
										<Input
											id="revealPassphrase"
											bind:value={revealPassphraseInput}
											type="password"
											placeholder="Enter the auction passphrase"
										/>
									</div>
									<Button
										class="w-full"
										disabled={isWorking || isActionsBlocked()}
										onclick={handleRevealPassphrase}
									>
										Reveal and complete auction
									</Button>
								{/if}
							</CardContent>
						</Card>
					{/if}

					{#if auction.state === 2 && isWinner}
						<Card>
							<CardHeader>
								<CardTitle>Winner file access</CardTitle>
								<CardDescription
									>Open the reveal popup if you are the winning bidder.</CardDescription
								>
							</CardHeader>
							<CardContent class="space-y-3">
								<Button
									class="w-full"
									disabled={isWorking || isActionsBlocked()}
									onclick={openWinnerDetails}
								>
									Get file
								</Button>
							</CardContent>
						</Card>
					{/if}
				</div>
			</div>
		{/if}
	</div>
</div>

{#if $accountSwitchPromptOpen && $pendingWalletAddress}
	<div class="fixed inset-0 z-50 flex items-center justify-center bg-slate-950/60 px-4">
		<div class="w-full max-w-md rounded-2xl bg-white p-6 shadow-2xl">
			<h2 class="text-lg font-semibold text-slate-950">Account changed</h2>
			<p class="mt-2 text-sm text-slate-600">
				You changed account in metamask, you want to use that account on page?
			</p>
			<p class="mt-2 text-xs break-all text-slate-500">{$pendingWalletAddress}</p>

			<div class="mt-5 flex justify-end gap-2">
				<Button type="button" variant="outline" onclick={rejectAccountSwitch}>No</Button>
				<Button type="button" onclick={acceptAccountSwitch}>Yes</Button>
			</div>
		</div>
	</div>
{/if}

{#if isFilePopupOpen && winnerDetails}
	<div class="fixed inset-0 z-50 flex items-center justify-center bg-slate-950/60 px-4">
		<div class="w-full max-w-lg rounded-2xl bg-white p-6 shadow-2xl">
			<div class="flex items-start justify-between gap-4">
				<div>
					<h2 class="text-xl font-semibold text-slate-950">File details</h2>
					<p class="text-sm text-slate-600">Visible only to the winning bidder.</p>
				</div>
				<Button variant="ghost" onclick={() => (isFilePopupOpen = false)}>Close</Button>
			</div>

			<div class="mt-6 space-y-3 text-sm text-slate-700">
				<div>
					<p>
						<span class="font-medium text-slate-950">IPFS handle:</span>
					</p>
					<p class="break-all">{winnerDetails.ipfsHandle}</p>
					<div class="mt-2 flex items-center gap-3">
						<a
							class="text-sm text-sky-600 hover:underline"
							href={ipfsToGatewayUrl(winnerDetails?.ipfsHandle ?? '')}
							target="_blank"
							rel="noreferrer">Open in gateway</a
						>
					</div>
				</div>
				<p>
					<span class="font-medium text-slate-950">Passphrase:</span>
					{winnerDetails.revealedPassphrase}
				</p>
				<p class="break-all">
					<span class="font-medium text-slate-950">Original hash:</span>
					{winnerDetails.originalFileHash}
				</p>
				<p class="break-all">
					<span class="font-medium text-slate-950">Passphrase hash:</span>
					{winnerDetails.passphraseHash}
				</p>
			</div>
		</div>
	</div>
{/if}
