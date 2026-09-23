<script lang="ts">
	import { Chat } from '$/lib/chat.svelte';
	import ChatMessage from '$/lib/components/ChatMessage.svelte';
	import * as Alert from '$/lib/components/ui/alert';
	import * as AlertDialog from '$/lib/components/ui/alert-dialog';
	import Button from '$/lib/components/ui/button/button.svelte';
	import { Field, FieldGroup, FieldLabel } from '$/lib/components/ui/field';
	import { Input } from '$/lib/components/ui/input';
	import * as Tabs from '$/lib/components/ui/tabs';
	import type { Message } from '$/types/message';
	import { onDestroy, tick } from 'svelte';
	import { SvelteMap } from 'svelte/reactivity';
	import { v4 } from 'uuid';

	type IdentifiedMessage = Message & { id: string };

	let logged = $state(false);
	let username = $state<string | undefined>(undefined);
	let address = $state('pcfeib425t.vsb.cz');
	let error = $state<null | 'username'>(null);

	let chat = $state<Chat | null>(null);
	let allMessages = new SvelteMap<string, IdentifiedMessage[]>();
	let messageTab = $state('global');
	let dmsOpen = $state<string[]>([]);
	let dmNotif = new SvelteMap<string, number>();
	let chats = $state<{
		[key: string]: HTMLElement | null;
	}>({});

	const getMessages = (tab: string) => {
		return allMessages.get(tab);
	};

	$effect(() => {
		const messages = allMessages.get(messageTab);
		const container = chats[messageTab];

		if (messages && messages.length && container) {
			tick().then(() => {
				if (container && container.parentElement) {
					container.parentElement.scrollTo({
						top: container.scrollHeight,
						behavior: 'smooth'
					});
				}
			});
		}
	});

	$effect(() => {
		if (messageTab !== 'global') {
			//AKA WE SWITCHED CHAT
			dmNotif.set(messageTab, 0);
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

	const putMessage = (message: IdentifiedMessage) => {
		const messages = getMessages('global') ?? [];

		allMessages.set('global', [...messages, message]);
	};

	const login = (anonymous: boolean) => {
		if (username == null && !anonymous) {
			error = 'username';
			return;
		}
		error = null;

		logged = true;

		chat = new Chat(anonymous ? undefined : username);

		chat.on('user-status', (user, status) => {
			putMessage({
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
			putMessage({
				id: v4(),
				type: 'chatMessage',
				timestamp,
				from,
				message,
				me: from === username
			});
		});

		chat.on('dmMessage', (chat, from, timestamp, message) => {
			if (dmsOpen.indexOf(chat) === -1) {
				dmsOpen.push(chat);
				chats[chat] = null;
			}

			const messages = getMessages(chat) ?? [];
			allMessages.set(chat, [
				...messages,
				{
					id: v4(),
					type: 'chatMessage',
					timestamp,
					from,
					message,
					me: from === username
				}
			]);

			if (messageTab !== chat) {
				dmNotif.set(chat, (dmNotif.get(chat) ?? 0) + 1);
			}
		});
	};

	onDestroy(() => {
		if (chat) chat.disconnect();
	});

	const logout = () => {
		if (chat) chat.disconnect();
		logged = false;
		username = undefined;
		messageTab = 'global';
		allMessages.clear();
		currentUsers = new Map();
	};

	let message = $state('');

	const send = () => {
		if (!message) return;
		if (!chat) return;

		if (messageTab === 'global') {
			chat.sendMessage(message);
		} else {
			chat.sendDM(messageTab, message);
		}

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
			<AlertDialog.Title class="w-full text-center text-2xl font-bold">Přihlášení</AlertDialog.Title
			>
			<AlertDialog.Description class="flex w-full flex-col gap-2">
				{#if error}
					<Alert.Root variant="destructive">
						<Alert.Description>Zadej platné jméno!</Alert.Description>
					</Alert.Root>
				{/if}
				<FieldGroup>
					<Field>
						<FieldLabel id="username">Adresa</FieldLabel>
						<Input
							id="address"
							type="text"
							placeholder="pcfeib425t.vsb.cz"
							required
							bind:value={address}
						/>
					</Field>
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
<main class="flex h-screen max-h-screen w-full gap-2 overflow-hidden p-2">
	<div class="flex h-full min-h-0 w-2/3 flex-col gap-2">
		<div class="flex min-h-0 w-full flex-1 flex-col rounded border border-primary p-2 shadow-md">
			<h1 class="mb-2 w-max shrink-0 border-b-2 border-primary text-lg font-bold">
				Historie Chatu
			</h1>

			<Tabs.Root bind:value={messageTab} class="flex h-full min-h-0 w-full flex-col gap-2">
				<Tabs.List>
					<Tabs.Trigger value="global">Globální</Tabs.Trigger>
					{#each dmsOpen as dm (dm)}
						<Tabs.Trigger
							value={dm}
							class={{
								'font-bold': (dmNotif.get(dm) ?? 0) > 0
							}}
						>
							{#if dmNotif.get(dm) ?? 0 > 0}
								🔔
							{/if}
							{dm} (PZ) {#if dmNotif.get(dm)}
								({dmNotif.get(dm)})
							{/if}</Tabs.Trigger
						>
					{/each}
				</Tabs.List>
				{#each ['global', ...dmsOpen] as tab (tab)}
					<Tabs.Content
						value={tab}
						class="flex h-full min-h-0 w-full flex-col gap-2 overflow-y-auto"
					>
						<div bind:this={chats[tab]}>
							{#each getMessages(tab) ?? [] as message (message.id)}
								<ChatMessage {message} />
							{/each}
						</div>
					</Tabs.Content>
				{/each}
			</Tabs.Root>
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

	<div class="flex h-full min-h-0 w-1/3 flex-col gap-2">
		<div
			class="flex min-h-0 w-full flex-1 flex-col overflow-y-auto rounded border border-primary p-2 shadow-md"
		>
			<h1 class="mb-2 w-max shrink-0 border-b-2 border-primary text-lg font-bold">
				Uživatelé chatu
			</h1>
			<div class="flex flex-col gap-1">
				<h2 class="text-lg font-bold text-green-500">Online ({onlineCount})</h2>
				<div class="flex flex-col gap-1">
					{#each Array.from(currentUsers) as [user, status] (user)}
						{#if status}
							<button
								class="w-full text-left text-green-500"
								onclick={() => {
									dmsOpen.push(user);
									tick().then(() => {
										messageTab = user;
									});
								}}>{user}</button
							>
						{/if}
					{/each}
				</div>
				<h2 class="text-lg font-bold text-red-500">Offline ({offlineCount})</h2>
				<div class="flex flex-col gap-1">
					{#each Array.from(currentUsers) as [user, status] (user)}
						{#if !status}
							<button
								class="w-full text-left text-red-500"
								onclick={() => {
									dmsOpen.push(user);
									tick().then(() => {
										messageTab = user;
									});
								}}>{user}</button
							>
						{/if}
					{/each}
				</div>
			</div>
		</div>
		<div class="flex w-full shrink-0 justify-between">
			<Button onclick={send}>Odeslat</Button>
			<Button variant="destructive" onclick={logout}>Odhlásit se</Button>
		</div>
	</div>
</main>
