<script lang="ts">
    import type { HTMLButtonAttributes } from 'svelte/elements';
    import { twMerge } from 'tailwind-merge';

    const { children, class: cls, ...props }: HTMLButtonAttributes = $props();
</script>

<button
    {...props}
    class={twMerge(
        'font-poppins rounded-md border-2 border-black px-4 py-2 text-xl font-bold transition-all hover:bg-secondary active:translate-y-1 disabled:cursor-not-allowed disabled:grayscale lg:text-2xl',
        cls
    )}
>
    {@render children?.()}
</button>
