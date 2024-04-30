<script lang="ts">
    import type { Item } from '$/types/types';

    export let data: Item[];
    export let selected: number | null = null;

    const select = (id: number) => {
        if (selected == id) {
            selected = null;
            return;
        }

        selected = id;
    };
</script>

<div class="table-responsive">
    <table class="table table-striped table-hover">
        <thead>
            <tr>
                <th>Id</th>
                <th>Jméno figurky</th>
                <th>Výrobce</th>
                <th>Váha</th>
                <th>Velikost</th>
                <th>Cena</th>
            </tr>
        </thead>
        <tbody>
            {#each data as item, id}
                <tr on:click={() => select(id)} class:table-secondary={id == selected} style="cursor:pointer;">
                    <th>{id + 1}</th>
                    <td>{item.name}</td>
                    <td>{item.manufacturer}</td>
                    <td>{item.weight}g</td>
                    <td>{item.size}mm</td>
                    <td>{item.price}$</td>
                </tr>
            {/each}
        </tbody>
    </table>
</div>
