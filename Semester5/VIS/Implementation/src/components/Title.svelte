<script lang="ts">
    import type { Snippet } from 'svelte';

    const { children }: { children: Snippet } = $props();
</script>

<h1 class="mb-2 font-poppins text-2xl font-bold lg:text-3xl">{@render children()}</h1>
