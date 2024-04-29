<script lang="ts">
    import { twMerge } from 'tailwind-merge';

    export let title: string;
    export let value: string | number | undefined = '';
    export let type: 'text' | 'number' | 'password' | 'email' = 'text';

    let cls = '';
    export { cls as class };

    const id = Math.random().toString(36).substring(7);
</script>

<div class={twMerge('font-ubuntu flex flex-col', cls)}>
    <label class="text-lg font-medium" for={id}>{title}</label>
    <input
        {id}
        class="text-ubuntu border-2 border-gray-400 px-2 py-1 text-lg text-black outline-none focus:border-blue-300"
        {value}
        on:input={(ev) => (value = ev.currentTarget.value)}
        {type}
    />
</div>
