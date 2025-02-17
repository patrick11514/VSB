<script lang="ts">
    import { twMerge } from 'tailwind-merge';

    export let type: 'button' | 'submit' | 'reset' = 'button';

    let cls = '';
    export { cls as class };
</script>

<button {type} on:click class={twMerge('text-ubuntu border-2 border-gray-400 bg-white px-2 py-1 text-lg text-black hover:border-blue-300 hover:bg-gray-200', cls)}><slot /></button>
