<script lang="ts">
    import type { HTMLTextareaAttributes } from 'svelte/elements';
    import { twMerge } from 'tailwind-merge';

    let { class: cls = '', value = $bindable(), invalid = $bindable(undefined), ...props }: HTMLTextareaAttributes & { invalid?: string } = $props();

    let el = $state<HTMLTextAreaElement>();

    $effect(() => {
        el?.setCustomValidity(invalid ?? '');
    });
</script>

<textarea
    bind:this={el}
    {...props}
    bind:value
    class={twMerge(
        'rounded-md border-2 border-black bg-transparent p-1 outline-none transition-colors duration-200 placeholder:text-center placeholder:text-black invalid:border-red-500 focus:border-sky-600',
        cls
    )}
></textarea>
