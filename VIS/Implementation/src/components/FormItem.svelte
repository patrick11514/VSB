<script lang="ts">
    import type { Snippet } from 'svelte';

    const { children, id, label, error = undefined }: { children: Snippet; id: string; label: string; error?: string } = $props();
</script>

<div class="flex flex-col">
    <label class="font-poppins mb-1 w-max text-left font-bold" for={id}>{label}</label>
    {@render children()}
    <span class:opacity-0={!error} class="text-left text-base font-bold text-red-500">{error ?? '???'}</span>
</div>
