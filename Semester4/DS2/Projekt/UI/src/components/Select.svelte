<script lang="ts">
    import { twMerge } from 'tailwind-merge';

    export let value = '';
    export let title: string;

    const id = Math.random().toString(36).substring(7);

    let cls = '';
    export { cls as class };
</script>

<div class={twMerge('font-ubuntu flex flex-col', cls)}>
    <label class="text-lg font-medium" for={id}>{title}</label>
    <select {id} {value} on:change={(ev) => (value = ev.currentTarget.value)} class="text-ubuntu border-2 border-gray-400 bg-white px-2 py-2 text-lg text-black"><slot /></select>
</div>
