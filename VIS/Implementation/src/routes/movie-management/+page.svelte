<script lang="ts">
    import Button from '$/components/Button.svelte';
    import Title from '$/components/Title.svelte';

    let movies = $state<>;
</script>

<div class="flex w-full flex-1 flex-col gap-4 p-4">
    <div class="flex w-full items-center justify-between">
        <Title>Správa Filmů</Title>
        <Button>Přidat</Button>
    </div>
    <div class="flex flex-1 flex-col border-2 border-black p-2"></div>
</div>
