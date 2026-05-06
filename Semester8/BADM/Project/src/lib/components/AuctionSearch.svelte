<script lang="ts">
	import { Input } from '$lib/components/ui/input';
	import * as Select from '$lib/components/ui/select';
	import { Search } from 'lucide-svelte';

	let { onSearch = () => {}, onSort = () => {} } = $props<{
		onSearch?: (query: string, type: 'name' | 'address') => void;
		onSort?: (sortBy: 'newest' | 'highest-bid' | 'ending-soon') => void;
	}>();

	let searchQuery = $state('');
	let searchType = $state<'name' | 'address'>('name');
	let sortBy = $state<'newest' | 'highest-bid' | 'ending-soon'>('newest');

	function handleSearch() {
		onSearch(searchQuery, searchType);
	}

	function handleSort(value: 'newest' | 'highest-bid' | 'ending-soon') {
		sortBy = value;
		onSort(sortBy);
	}
</script>

<div class="mb-6 space-y-4">
	<div class="grid items-end gap-4 md:grid-cols-4">
		<div class="md:col-span-2">
			<label for="search" class="mb-2 block text-sm font-medium text-slate-700">Search</label>
			<form
				onsubmit={(e) => {
					e.preventDefault();
					handleSearch();
				}}
				class="flex gap-2"
			>
				<Input
					id="search"
					placeholder="Search by name or address..."
					bind:value={searchQuery}
					type="text"
					class="flex-1"
				/>
				<button
					type="submit"
					class="inline-flex h-9 items-center justify-center rounded-md border border-input bg-background px-2.5 text-sm font-medium whitespace-nowrap hover:bg-accent hover:text-accent-foreground focus-visible:outline-none disabled:pointer-events-none disabled:opacity-50"
				>
					<Search class="h-4 w-4" />
				</button>
			</form>
		</div>

		<div>
			<label for="search-type" class="mb-2 block text-sm font-medium text-slate-700"
				>Search Type</label
			>
			<Select.Root
				type="single"
				value={searchType}
				onValueChange={(v: string) => {
					searchType = v as 'name' | 'address';
				}}
			>
				<Select.Trigger id="search-type">
					<span>{searchType === 'name' ? 'By Name' : 'By Address'}</span>
				</Select.Trigger>
				<Select.Content>
					<Select.Item value="name">By Name</Select.Item>
					<Select.Item value="address">By Address</Select.Item>
				</Select.Content>
			</Select.Root>
		</div>

		<div>
			<label for="sort" class="mb-2 block text-sm font-medium text-slate-700">Sort</label>
			<Select.Root
				type="single"
				value={sortBy}
				onValueChange={(v: string) => {
					handleSort(v as 'newest' | 'highest-bid' | 'ending-soon');
				}}
			>
				<Select.Trigger id="sort">
					<span
						>{sortBy === 'newest'
							? 'Newest First'
							: sortBy === 'highest-bid'
								? 'Highest Bid'
								: 'Ending Soon'}</span
					>
				</Select.Trigger>
				<Select.Content>
					<Select.Item value="newest">Newest First</Select.Item>
					<Select.Item value="highest-bid">Highest Bid</Select.Item>
					<Select.Item value="ending-soon">Ending Soon</Select.Item>
				</Select.Content>
			</Select.Root>
		</div>
	</div>
</div>
