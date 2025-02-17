<script lang="ts">
    import Button from '$/components/Button.svelte';
    import Col from '$/components/Col.svelte';
    import Container from '$/components/Container.svelte';
    import DropDownItem from '$/components/DropDownItem.svelte';
    import FluidContainer from '$/components/FluidContainer.svelte';
    import FormGroup from '$/components/FormGroup.svelte';
    import ItemPopup from '$/components/ItemPopup.svelte';
    import ItemShow from '$/components/ItemShow.svelte';
    import ItemsTable from '$/components/ItemsTable.svelte';
    import Modal from '$/components/Modal.svelte';
    import NavItem from '$/components/NavItem.svelte';
    import Navigation from '$/components/Navigation.svelte';
    import Row from '$/components/Row.svelte';
    import SingleRowContainer from '$/components/SingleRowContainer.svelte';
    import { creating, itemPopupCancel, itemPopupClose, itemPopupData, itemPopupOpen, items, modalAfterConfirm, modalIsOpen, selected } from '$/components/Store.svelte';
    import { API } from '$/lib/api';
    import { addItem, removeItem } from '$/lib/functions';
    import type { Item } from '$/types/types';
    import { onMount } from 'svelte';
    import { writable } from 'svelte/store';
    import type { PageServerData } from './$types';

    export let data: PageServerData;

    let filteredData = writable<Item[]>([]);

    const handleData = (response: (typeof data)['data']) => {
        items.set(response);
    };
    handleData(data.data);

    onMount(async () => {
        const response = await API.data.GET();
        handleData(response);
    });

    const exit = () => {
        window.location.assign('about:blank');
    };

    let modalOpen = false;

    const showAbout = () => {
        modalOpen = true;
    };

    let search = writable<{
        name: string;
        manufacturer: string;
        size: number;
        price: number;
    }>({
        name: '',
        manufacturer: '',
        size: 0,
        price: 0
    });

    const resetSearch = () => {
        search.set({
            name: '',
            manufacturer: '',
            size: 0,
            price: 0
        });

        filteredData.set($items);
    };

    const doSearch = () => {
        console.log($search);
        const { name, manufacturer, price, size } = $search;
        filteredData.set(
            $items.filter((item) => {
                return (
                    (name === '' || item.name.includes(name)) &&
                    (manufacturer === '' || item.manufacturer.includes(manufacturer)) &&
                    (price !== undefined || item.price >= price) &&
                    (size !== undefined || item.size >= size)
                );
            })
        );
    };

    items.subscribe(() => {
        doSearch();
    });
</script>

<svelte:head>
    <title>Evidence Anime Figurek</title>
</svelte:head>

<Navigation>
    <NavItem name="File">
        <DropDownItem on:click={addItem}>New Item</DropDownItem>
        <DropDownItem on:click={() => removeItem($selected)}>Delete Item</DropDownItem>
        <DropDownItem on:click={exit}>Exit Item</DropDownItem>
    </NavItem>
    <NavItem name="Other">
        <DropDownItem on:click={showAbout}>About program</DropDownItem>
    </NavItem>
</Navigation>

<SingleRowContainer color="dark-subtle">
    <h4><b>Vyhledávání</b></h4>
    <Container>
        <Row>
            <Col>
                <FormGroup title="Jméno" bind:value={$search.name} />
            </Col>
            <Col>
                <FormGroup title="Výrobce" bind:value={$search.manufacturer} />
            </Col>
            <Col>
                <FormGroup title="Velikost" bind:value={$search.size} type="number" />
            </Col>
            <Col>
                <FormGroup title="Cena" bind:value={$search.price} type="number" />
            </Col>
            <Col class="d-flex gap-2 align-items-end">
                <Button on:click={resetSearch} color="secondary">Resetovat</Button>
                <Button on:click={doSearch} color="secondary">Vyhledat</Button>
            </Col>
        </Row>
    </Container>
</SingleRowContainer>

<FluidContainer>
    <Row class="p-4 h-100">
        <Col class="m-2">
            <Container class="h-100 p-3" background="body-secondary">
                <ItemsTable data={$filteredData} bind:selected={$selected} />
            </Container>
        </Col>
        <Col class="m-2">
            <Container class="h-100" background="body-secondary">
                <ItemShow selectedId={$selected} selected={$selected !== null ? $filteredData[$selected] : null} />
            </Container>
        </Col>
    </Row>
</FluidContainer>

<Modal bind:show={modalOpen}>
    <div class="modal-header">
        <h5 class="modal-title">Evidence anime figurek</h5>
        <button on:click={() => (modalOpen = false)} type="button" class="btn-close"></button>
    </div>
    <div class="modal-body">
        <h6>Vytvořil <a href="https://patrick115.eu" target="_blank">Patrik Mintěl</a></h6>
    </div>
</Modal>

<ItemPopup bind:creating={$creating} bind:data={$itemPopupData} bind:opened={$itemPopupOpen} bind:closeFunction={$itemPopupClose} bind:cancelFunction={$itemPopupCancel} />

<Modal bind:show={$modalIsOpen}>
    <div class="modal-header">
        <h5 class="modal-title">Opravdu chceš smazat tuto položku?</h5>
        <button on:click={() => ($modalIsOpen = false)} type="button" class="btn-close"></button>
    </div>
    <div class="modal-footer">
        <Button color="danger" on:click={$modalAfterConfirm}>Ano</Button>
        <Button color="secondary" on:click={() => ($modalIsOpen = false)}>Ne</Button>
    </div>
</Modal>
