<script lang="ts">
	import favicon from '$lib/assets/favicon.svg';
	import ConnectWallet from '$lib/components/ConnectWallet.svelte';
	import Header from '$lib/components/Header.svelte';
	import { removeMetaMaskListeners, setupMetaMaskListeners } from '$lib/services/web3';
	import {
		accountSwitchPromptOpen,
		isAuthenticated,
		pendingWalletAddress,
		walletAddress
	} from '$lib/stores/session';
	import { onMount } from 'svelte';
	import { get } from 'svelte/store';
	import './layout.css';

	let { children } = $props();

	onMount(() => {
		const handleAccountsChanged = (accounts: string[]) => {
			if (accounts.length === 0) {
				isAuthenticated.set(false);
				walletAddress.set(null);
				pendingWalletAddress.set(null);
				accountSwitchPromptOpen.set(false);
				return;
			}

			const newAccount = accounts[0];
			const acceptedAccount = get(walletAddress);

			if (!acceptedAccount) {
				walletAddress.set(newAccount);
				pendingWalletAddress.set(null);
				accountSwitchPromptOpen.set(false);
				return;
			}

			if (newAccount.toLowerCase() === acceptedAccount.toLowerCase()) {
				pendingWalletAddress.set(null);
				accountSwitchPromptOpen.set(false);
				return;
			}

			pendingWalletAddress.set(newAccount);
			accountSwitchPromptOpen.set(true);
		};

		const handleChainChanged = () => {
			isAuthenticated.set(false);
			walletAddress.set(null);
			pendingWalletAddress.set(null);
			accountSwitchPromptOpen.set(false);
		};

		setupMetaMaskListeners(handleAccountsChanged, handleChainChanged);

		return () => {
			removeMetaMaskListeners(handleAccountsChanged, handleChainChanged);
		};
	});
</script>

<svelte:head><link rel="icon" href={favicon} /></svelte:head>

{#if !$isAuthenticated}
	<ConnectWallet />
{:else}
	<div class="flex min-h-screen flex-col">
		<Header />
		<main class="flex-1">
			{@render children()}
		</main>
	</div>
{/if}
