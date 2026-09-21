<script lang="ts">
	import type { Message } from '$/types/message';

	const {
		message: data
	}: {
		message: Message;
	} = $props();

	const formatTimestamp = (timestamp: number): string => {
		const date = new Date(timestamp);
		return date.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
	};
</script>

<div class="flex w-full items-center gap-2">
	<span class="mb-auto whitespace-nowrap">{formatTimestamp(data.timestamp)}</span>
	{#if data.type === 'statusUpdate'}
		<span
			class={{
				'text-sm': true,
				'text-green-500': data.online,
				'text-red-500': !data.online
			}}
			>{data.username} se stal {data.online ? 'online' : 'offline'}
		</span>
	{:else if data.type === 'chatMessage'}
		<span class="mb-auto font-bold">{data.from}:</span>
		<span class="wrap-anywhere">{data.message}</span>
	{/if}
</div>
