<script lang="ts">
    import { addItem, editItem, removeItem } from '$/lib/functions';
    import type { Item } from '$/types/types';
    import Button from './Button.svelte';
    import Card from './Card.svelte';
    import CardBody from './CardBody.svelte';
    import CardHeader from './CardHeader.svelte';
    import CardNav from './CardNav.svelte';
    import Col from './Col.svelte';
    import Container from './Container.svelte';
    import Row from './Row.svelte';

    export let selectedId: number | null = null;
    export let selected: Item | null = null;

    let category: 'info' | 'images' = 'info';

    let names = ['Id', 'Jméno', 'Výrobce', 'Velikost', 'Váha', 'Cena'];
    let values: (string | number)[] = [];
    $: {
        if (selected !== null) {
            values = [(selectedId ?? 0) + 1, selected?.name ?? '', selected?.manufacturer ?? '', selected?.size ?? '', selected?.weight ?? '', selected?.price ?? ''];
        } else {
            values = Array.from({ length: names.length }).fill('') as string[];
        }
    }
</script>

<Container class="pe-0 h-100 d-flex flex-column">
    <Row>
        <div class="col-8">
            <Card class="m-4">
                <CardHeader>
                    <CardNav>
                        <li class="nav-item">
                            <a class="nav-link" on:click={() => (category = 'info')} class:active={category == 'info'} aria-current="true" href="#">Informace o produktu</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" on:click={() => (category = 'images')} class:active={category == 'images'} href="#">Více obrázků</a>
                        </li>
                    </CardNav>
                </CardHeader>
                <CardBody class="d-flex flex-column" style="min-height:240px">
                    {#if category == 'info'}
                        {#if selected == null}
                            <h4 class="m-auto">Vyber produkt pro zobrazení informací</h4>
                        {:else}
                            <table class="table">
                                <tbody>
                                    {#each names as name, i}
                                        <tr>
                                            <th>{name}</th>
                                            <td class="text-end">{values[i]}</td>
                                        </tr>
                                    {/each}
                                </tbody>
                            </table>
                        {/if}
                    {:else}
                        <Container class="w-100 h-100 flex-1" background="secondary">
                            {#if selected !== null}
                                <Row class="row-cols-2 m-2 h-100">
                                    {#each selected.images.other as image}
                                        <Col class="flex">
                                            <img class="w-100 m-auto h-auto mh-100" src={image} alt="Product" />
                                        </Col>
                                    {/each}
                                </Row>
                            {/if}
                        </Container>
                    {/if}
                </CardBody>
            </Card>
        </div>
        <Col class="pe-0">
            <div class="bg-secondary ratio ratio-1x1">
                {#if selected !== null}
                    <img class="object-fit-contain" src={selected.images.main} alt="Product preview" />
                {/if}
            </div>
        </Col>
    </Row>
    <Row class="mt-auto p-4">
        <div class="d-flex flex-row gap-4">
            <Button on:click={addItem} style="min-width: 120px;" color="success">Přidat</Button>
            <Button on:click={() => editItem(selectedId)} style="min-width: 120px;" color="secondary">Upravit</Button>
            <Button on:click={() => removeItem(selectedId)} style="min-width: 120px;" color="danger">Odebrat</Button>
        </div>
    </Row>
</Container>
