<script lang="ts">
	import { goto } from '$app/navigation';
	import { Button } from '$lib/components/ui/button';
	import { Card, CardContent, CardHeader, CardTitle } from '$lib/components/ui/card';
	import { Input } from '$lib/components/ui/input';
	import { deployAuctionContract } from '$lib/services/auction-contract';
	import {
		encryptWithPassphrase,
		generateRandomPassphrase,
		packageEncryptedFile,
		sha256Hex
	} from '$lib/services/crypto';
	import { uploadFile } from '$lib/services/ipfs';
	import { parseEth } from '$lib/services/web3';
	import { pendingWalletAddress, provider, walletAddress } from '$lib/stores/session';

	const defaultEndAt = new Date(Date.now() + 2 * 24 * 60 * 60 * 1000).toISOString().slice(0, 16);
	type SourceMode = 'manual' | 'file';

	let name = $state('Example contract');
	let description = $state('Simple contract presentation auction');
	let endAt = $state(defaultEndAt);
	let sourceMode = $state<SourceMode>('manual');
	let ipfsHandle = $state('ipfs://example-handle');
	let originalFileHash = $state('');
	let hashedFileHash = $state('');
	let selectedFile = $state<File | null>(null);
	let minBidEth = $state('0');
	let passphrase = $state('open-sesame');
	let isSubmitting = $state(false);
	let errorMessage = $state('');
	let successMessage = $state('');

	const footerWalletAddress = $derived($pendingWalletAddress ?? $walletAddress);

	function isValidBytes32(value: string) {
		return /^0x[a-fA-F0-9]{64}$/.test(value);
	}

	function handleFileChange(event: Event) {
		const input = event.currentTarget as HTMLInputElement;
		selectedFile = input.files?.[0] ?? null;
	}

	function handleGeneratePassphrase() {
		passphrase = generateRandomPassphrase(16);
	}

	async function handleSubmit(event: SubmitEvent) {
		event.preventDefault();
		errorMessage = '';
		successMessage = '';

		if (!$provider) {
			errorMessage = 'Connect MetaMask first.';
			return;
		}

		if (!name.trim() || !description.trim() || !passphrase.trim() || !endAt || !minBidEth.trim()) {
			errorMessage = 'All fields are required.';
			return;
		}

		if (sourceMode === 'manual') {
			if (!ipfsHandle.trim() || !originalFileHash.trim() || !hashedFileHash.trim()) {
				errorMessage = 'IPFS handle and both hash fields are required in manual mode.';
				return;
			}

			if (!isValidBytes32(originalFileHash.trim()) || !isValidBytes32(hashedFileHash.trim())) {
				errorMessage = 'originalFileHash and hashedFileHash must be 32-byte hex values.';
				return;
			}
		}

		if (sourceMode === 'file' && !selectedFile) {
			errorMessage = 'Select a file in file mode.';
			return;
		}

		isSubmitting = true;
		try {
			const signer = await $provider.getSigner();
			const passphraseHash = `0x${await sha256Hex(passphrase.trim())}`;
			const minBid = parseEth(minBidEth.trim());

			let resolvedIpfsHandle = ipfsHandle.trim();
			let resolvedOriginalFileHash = originalFileHash.trim();
			let resolvedHashedFileHash = hashedFileHash.trim();

			if (sourceMode === 'file' && selectedFile) {
				const fileBytes = await selectedFile.arrayBuffer();
				resolvedOriginalFileHash = `0x${await sha256Hex(fileBytes)}`;

				const { cipher, iv, salt } = await encryptWithPassphrase(fileBytes, passphrase.trim());
				const encryptedPayload = packageEncryptedFile(cipher, iv, salt);
				resolvedHashedFileHash = `0x${await sha256Hex(encryptedPayload)}`;

				const encryptedFile = new File([encryptedPayload], selectedFile.name, {
					type: selectedFile.type || 'application/octet-stream'
				});
				const cid = await uploadFile(encryptedFile);
				resolvedIpfsHandle = `ipfs://${cid}`;
			}
			const endTimestamp = BigInt(Math.floor(new Date(endAt).getTime() / 1000));

			const address = await deployAuctionContract(signer, {
				minBid,
				name: name.trim(),
				description: description.trim(),
				originalFileHash: resolvedOriginalFileHash,
				hashedFileHash: resolvedHashedFileHash,
				ipfsHandle: resolvedIpfsHandle,
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
						<p class="text-sm font-medium text-slate-700">Source mode</p>
						<div class="grid grid-cols-2 gap-2">
							<Button
								type="button"
								variant={sourceMode === 'manual' ? 'default' : 'outline'}
								onclick={() => (sourceMode = 'manual')}
							>
								IPFS handle (manual)
							</Button>
							<Button
								type="button"
								variant={sourceMode === 'file' ? 'default' : 'outline'}
								onclick={() => (sourceMode = 'file')}
							>
								File input
							</Button>
						</div>
					</div>

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
						<label class="text-sm font-medium text-slate-700" for="minBidEth">Min bid (ETH)</label>
						<Input id="minBidEth" bind:value={minBidEth} type="number" min="0" step="0.0001" />
					</div>

					{#if sourceMode === 'manual'}
						<div class="grid gap-2">
							<label class="text-sm font-medium text-slate-700" for="ipfsHandle">IPFS handle</label>
							<Input id="ipfsHandle" bind:value={ipfsHandle} placeholder="ipfs://..." />
						</div>

						<div class="grid gap-2">
							<label class="text-sm font-medium text-slate-700" for="originalFileHash"
								>Original file hash</label
							>
							<Input
								id="originalFileHash"
								bind:value={originalFileHash}
								placeholder="0x... (32-byte hex)"
							/>
						</div>

						<div class="grid gap-2">
							<label class="text-sm font-medium text-slate-700" for="hashedFileHash"
								>Hashed file hash</label
							>
							<Input
								id="hashedFileHash"
								bind:value={hashedFileHash}
								placeholder="0x... (32-byte hex)"
							/>
						</div>
					{:else}
						<div class="grid gap-2">
							<label class="text-sm font-medium text-slate-700" for="sourceFile">Source file</label>
							<input
								id="sourceFile"
								type="file"
								onchange={handleFileChange}
								class="block w-full rounded-md border border-input bg-transparent px-3 py-2 text-sm text-slate-700 file:mr-4 file:rounded-md file:border-0 file:bg-slate-900 file:px-3 file:py-2 file:text-sm file:font-medium file:text-white"
							/>
							{#if selectedFile}
								<p class="text-xs text-slate-600">Selected: {selectedFile.name}</p>
							{/if}
						</div>
					{/if}

					<div class="grid gap-2">
						<label class="text-sm font-medium text-slate-700" for="passphrase">Passphrase</label>
						<div class="flex items-center gap-2">
							<Input
								id="passphrase"
								bind:value={passphrase}
								type="password"
								placeholder="Secret passphrase"
							/>
							{#if sourceMode === 'file'}
								<Button type="button" variant="outline" onclick={handleGeneratePassphrase}
									>Generate</Button
								>
							{/if}
						</div>
					</div>

					<div
						class="rounded-lg border border-dashed border-slate-200 bg-slate-100/70 p-4 text-sm text-slate-600"
					>
						<p>Connected wallet: {footerWalletAddress ?? 'Not connected'}</p>
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
