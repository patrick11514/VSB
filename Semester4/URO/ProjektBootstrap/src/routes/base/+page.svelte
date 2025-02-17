<script lang="ts">
    import Table from '$/components/Table.svelte';
</script>

<div class="container bg-secondary">Hello , world !</div>
<div class="container-sm bg-info">100% wide until small breakpoint</div>
<div class="container-md bg-info">100% wide until medium breakpoint</div>
<div class="container-lg bg-info">100% wide until large breakpoint</div>
<div class="container-xl bg-info">100% wide until extra large breakpoint</div>
<div class="container-xxl bg-info">100% wide until extra extra large breakpoint</div>
<div class="container-fluid bg-warning">Hello , world !</div>

{#each ['', 'text-center'] as cls}
    <div class="container-xl">
        <Table {cls} />
    </div>
{/each}

<div class="container text-center">
    <div class="row">
        <div class="col"><Table /></div>
        <div class="col"><input type="text" class="form-control" /></div>
        <div class="col">
            <button class="btn btn-primary">Small button</button>
            <button class="btn btn-primary btn-lg">Large button</button>
        </div>
    </div>
    {#each ['primary', 'secondary', 'success', 'danger', 'warning', 'info', 'light'] as color}
        <div class="row bg-{color} p-3 mb-2" class:text-white={color != 'warning' && color != 'light'}>
            .bg-{color}
        </div>
    {/each}
</div>
