<script lang="ts">
	import favicon from '$lib/assets/favicon.svg';
	import ConnectWallet from '$lib/components/ConnectWallet.svelte';
	import { removeMetaMaskListeners, setupMetaMaskListeners } from '$lib/services/web3';
	import { isAuthenticated, walletAddress } from '$lib/stores/session';
	import { onMount } from 'svelte';
	import './layout.css';

	let { children } = $props();

	onMount(() => {
		// Try to reconnect on mount if user was previously connected
		setupMetaMaskListeners(
			(accounts: string[]) => {
				console.log('Accounts changed:', accounts);
				if (accounts.length === 0) {
					// User disconnected
					isAuthenticated.set(false);
					walletAddress.set(null);
				}
			},
			(chainId: string) => {
				console.log('Chain changed:', chainId);
				// User switched networks, re-check authentication
				isAuthenticated.set(false);
				walletAddress.set(null);
			}
		);

		return () => {
			removeMetaMaskListeners(
				() => {},
				() => {}
			);
		};
	});
</script>

<svelte:head><link rel="icon" href={favicon} /></svelte:head>

{#if !$isAuthenticated}
	<ConnectWallet />
{:else}
	{@render children()}
{/if}
