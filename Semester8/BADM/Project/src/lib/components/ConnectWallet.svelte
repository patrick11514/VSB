<script lang="ts">
	import {
		Card,
		CardContent,
		CardDescription,
		CardHeader,
		CardTitle
	} from '$lib/components/ui/card';
	import { connectMetaMask } from '$lib/services/web3';
	import {
		error,
		isAuthenticated,
		isLoading,
		network,
		provider,
		walletAddress
	} from '$lib/stores/session';
	import { Loader2 } from 'lucide-svelte';

	async function handleConnect() {
		error.set(null);
		isLoading.set(true);
		try {
			const p = await connectMetaMask();
			if (!p) {
				error.set('Failed to connect MetaMask. Make sure it is installed.');
				isLoading.set(false);
				return;
			}

			const net = await p.getNetwork();
			if (Number(net.chainId) !== 11155111) {
				error.set('Please switch to Sepolia testnet in MetaMask.');
				isLoading.set(false);
				return;
			}

			const signer = await p.getSigner();
			const address = await signer.getAddress();

			provider.set(p);
			network.set(net);
			walletAddress.set(address);
			isAuthenticated.set(true);
			isLoading.set(false);
		} catch (e) {
			const message = e instanceof Error ? e.message : 'Failed to connect';
			error.set(message);
			isLoading.set(false);
		}
	}
</script>

<div
	class="flex min-h-screen items-center justify-center bg-linear-to-br from-slate-900 to-slate-800"
>
	<Card class="w-full max-w-md shadow-xl">
		<CardHeader class="text-center">
			<CardTitle class="text-2xl">EtherZar</CardTitle>
			<CardDescription>Ethereum Bazaar for Encrypted Digital Content</CardDescription>
		</CardHeader>
		<CardContent class="space-y-4">
			<div class="text-center text-sm text-slate-600">
				<p>Connect your MetaMask wallet to continue.</p>
				<p class="mt-2 text-xs">This app requires Sepolia testnet.</p>
			</div>

			{#if $error}
				<div class="rounded border border-red-200 bg-red-50 p-3 text-sm text-red-700">
					{$error}
				</div>
			{/if}

			<button
				onclick={handleConnect}
				disabled={$isLoading}
				class="inline-flex h-10 w-full items-center justify-center rounded-md bg-primary px-8 text-sm font-medium whitespace-nowrap text-primary-foreground hover:bg-primary/90 focus-visible:outline-none disabled:pointer-events-none disabled:opacity-50"
			>
				{#if $isLoading}
					<Loader2 class="mr-2 h-4 w-4 animate-spin" />
					Connecting...
				{:else}
					Connect MetaMask
				{/if}
			</button>

			<div class="text-center text-xs text-slate-500">
				<p>
					No MetaMask? <a
						href="https://metamask.io"
						target="_blank"
						class="text-blue-600 hover:underline">Install it here</a
					>
				</p>
			</div>
		</CardContent>
	</Card>
</div>
