<script lang="ts">
	import { Chat } from '$/lib/chat.svelte';
	import ChatMessage from '$/lib/components/ChatMessage.svelte';
	import * as Alert from '$/lib/components/ui/alert';
	import * as AlertDialog from '$/lib/components/ui/alert-dialog';
	import Button from '$/lib/components/ui/button/button.svelte';
	import { Field, FieldGroup, FieldLabel } from '$/lib/components/ui/field';
	import { Input } from '$/lib/components/ui/input';
	import type { Message } from '$/types/message';
	import { onDestroy, tick } from 'svelte';
	import { v4 } from 'uuid';

	type IdentifiedMessage = Message & { id: string };

	let logged = $state(false);
	let username = $state<string | undefined>(undefined);
	let error = $state<null | 'username'>(null);

	let chat = $state<Chat | null>(null);
	let messages = $state<IdentifiedMessage[]>([]);
	let messagesContainer = $state<HTMLDivElement | null>(null);

	$effect(() => {
		if (messages.length && messagesContainer) {
			tick().then(() => {
				if (messagesContainer) {
					messagesContainer.scrollTo({
						top: messagesContainer.scrollHeight,
						behavior: 'smooth'
					});
				}
			});
		}
	});
	let currentUsers = $state<Map<string, boolean>>(new Map());
	let onlineCount = $derived.by(() => {
		let count = 0;
		for (const status of currentUsers) {
			if (status[1]) count++;
		}
		return count;
	});

	let offlineCount = $derived.by(() => {
		return currentUsers.size - onlineCount;
	});

	const login = (anonymous: boolean) => {
		if (username == null && !anonymous) {
			error = 'username';
			return;
		}
		error = null;

		logged = true;

		chat = new Chat(anonymous ? undefined : username);

		chat.on('user-status', (user, status) => {
			messages.push({
				id: v4(),
				type: 'statusUpdate',
				timestamp: Date.now(),
				username: user,
				online: status
			});
		});

		chat.on('list-update', (list) => {
			currentUsers = list;
		});

		chat.on('message', (from, timestamp, message) => {
			messages.push({
				id: v4(),
				type: 'chatMessage',
				timestamp,
				from,
				message
			});
		});
	};

	onDestroy(() => {
		if (chat) chat.disconnect();
	});

	const logout = () => {
		if (chat) chat.disconnect();
		logged = false;
		username = undefined;
		messages = [];
		currentUsers = new Map();
	};

	let message = $state('');

	const send = () => {
		if (!message) return;
		if (!chat) return;

		chat.sendMessage(message);
		message = '';
	};
</script>

<AlertDialog.Root
	bind:open={() => !logged, (val) => (logged = !val)}
	onOpenChange={(change) => {
		if (!change) {
			//Aka user closed popup
			login(true);
		}
	}}
>
	<AlertDialog.Content class="flex min-h-1/2 flex-col justify-center">
		<AlertDialog.Header>
			<AlertDialog.Title class="w-full text-center text-2xl font-bold">Přihlášení</AlertDialog.Title>
			<AlertDialog.Description class="flex w-full flex-col gap-2">
				{#if error}
					<Alert.Root variant="destructive">
						<Alert.Description>Zadej platné jméno!</Alert.Description>
					</Alert.Root>
				{/if}
				<FieldGroup>
					<Field>
						<FieldLabel id="username">Jméno</FieldLabel>
						<Input id="username" type="text" placeholder="xxx0123" required bind:value={username} />
					</Field>
				</FieldGroup>
			</AlertDialog.Description>
		</AlertDialog.Header>
		<AlertDialog.Footer>
			<AlertDialog.Action variant="secondary" class="mx-auto w-1/2" onclick={() => login(true)}>
				Anonymní přihlášení
			</AlertDialog.Action>

			<AlertDialog.Action class="mx-auto w-1/2" onclick={() => login(false)}>
				Přihlásit
			</AlertDialog.Action>
		</AlertDialog.Footer>
	</AlertDialog.Content>
</AlertDialog.Root>

<!-- chat window aka 4 parts where topleft = chat history, top right = user list + their status bottom left the chat window and bottom right = send button-->
<main class="flex h-screen max-h-screen w-full gap-2 p-2 overflow-hidden">
	<div class="flex h-full w-2/3 flex-col gap-2 min-h-0">
		<div class="flex flex-1 flex-col min-h-0 w-full rounded border border-primary p-2 shadow-md">
			<h1 class="w-max shrink-0 border-b-2 border-primary text-lg font-bold mb-2">Historie Chatu</h1>
			<div bind:this={messagesContainer} class="flex flex-1 flex-col gap-1 overflow-y-auto min-h-0">
				{#each messages as message (message.id)}
					<ChatMessage {message} />
				{/each}
			</div>
		</div>
		<div class="w-full shrink-0">
			<Input
				type="text"
				class="border-primary"
				placeholder="Zadej zprávu..."
				required
				bind:value={message}
				onkeydown={(e) => {
					if (e.key === 'Enter') send();
				}}
			/>
		</div>
	</div>

	<div class="flex h-full w-1/3 flex-col gap-2 min-h-0">
		<div class="flex flex-1 flex-col min-h-0 w-full rounded border border-primary p-2 shadow-md overflow-y-auto">
			<h1 class="w-max shrink-0 border-b-2 border-primary text-lg font-bold mb-2">Uživatelé chatu</h1>
			<div class="flex flex-col gap-1">
				<h2 class="text-lg font-bold text-green-500">Online ({onlineCount})</h2>
				<div class="flex flex-col gap-1">
					{#each Array.from(currentUsers) as [user, status] (user)}
						{#if status}
							<span class="text-green-500">{user}</span>
						{/if}
					{/each}
				</div>
				<h2 class="text-lg font-bold text-red-500">Offline ({offlineCount})</h2>
				<div class="flex flex-col gap-1">
					{#each Array.from(currentUsers) as [user, status] (user)}
						{#if !status}
							<span class="text-red-500">{user}</span>
						{/if}
					{/each}
				</div>
			</div>
		</div>
		<div class="flex w-full justify-between shrink-0">
			<Button onclick={send}>Odeslat</Button>
			<Button variant="destructive" onclick={logout}>Odhlásit se</Button>
		</div>
	</div>
</main>
