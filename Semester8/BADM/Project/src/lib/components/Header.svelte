<script lang="ts">
	import { goto } from '$app/navigation';
	import { Badge } from '$lib/components/ui/badge';
	import { Button } from '$lib/components/ui/button';
	import { resetAuth, walletAddress } from '$lib/stores/session';
	import { LogOut } from 'lucide-svelte';

	function formatAddress(addr: string) {
		return `${addr.slice(0, 6)}...${addr.slice(-4)}`;
	}

	async function handleDisconnect() {
		resetAuth();
	}

	async function handleLogoClick() {
		await goto('/');
	}
</script>

<header class="sticky top-0 z-40 border-b bg-white shadow-sm">
	<div class="container mx-auto flex items-center justify-between px-4 py-4">
		<button
			type="button"
			onclick={handleLogoClick}
			class="flex flex-col gap-0 transition-opacity hover:opacity-80"
		>
			<h1 class="text-2xl font-bold text-slate-900">EtherZar</h1>
			<p class="text-xs text-slate-500">Ethereum Bazaar for Encrypted Digital Assets</p>
		</button>

		<div class="flex items-center gap-4">
			{#if $walletAddress}
				<div class="flex items-center gap-2">
					<Badge variant="secondary">{formatAddress($walletAddress)}</Badge>
				</div>
				<Button variant="outline" onclick={handleDisconnect}>
					<LogOut class="mr-2 h-4 w-4" />
					Disconnect
				</Button>
			{/if}
		</div>
	</div>
</header>
