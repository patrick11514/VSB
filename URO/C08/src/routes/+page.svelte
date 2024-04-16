<script lang="ts">
    import { SwalAlert } from '$/lib/functions';
    import { writable } from 'svelte/store';

    const exit = () => {
        window.location.assign('about:blank');
    };

    const newItem = () => {};

    const removeItem = () => {};

    const showAbout = () => {
        SwalAlert({
            toast: false,
            title: 'Evidence anime figurek',
            text: 'Vytvořil Patrik Mintěl (MIN0150)',
            timer: 0,
            position: 'center'
        });
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
    };

    const doSearch = () => {
        //TODO
    };
</script>

<svelte:head>
    <title>Evidence Anime Figurek</title>
</svelte:head>

<nav class="navbar navbar-expand-lg bg-body-tertiary">
    <div class="container-fluid">
        <div class="collapse navbar-collapse">
            <ul class="navbar-nav">
                <li class="nav-item">
                    <a class="nav-link dropdown-toggle" href="#" role="button" data-bs-toggle="dropdown" aria-expanded="false">File</a>
                    <ul class="dropdown-menu">
                        <li><a on:click={newItem} class="dropdown-item" href="#">New item</a></li>
                        <li><a on:click={removeItem} class="dropdown-item" href="#">Delete item</a></li>
                        <li><a on:click={exit} class="dropdown-item" href="#">Exit</a></li>
                    </ul>
                </li>
                <li class="nav-item">
                    <a class="nav-link dropdown-toggle" href="#" role="button" data-bs-toggle="dropdown" aria-expanded="false">Other</a>
                    <ul class="dropdown-menu">
                        <li><a on:click={showAbout} class="dropdown-item" href="#">About program</a></li>
                    </ul>
                </li>
            </ul>
        </div>
    </div>
</nav>

<div class="container-fluid mt-4">
    <div class="row">
        <div class="bg-dark-subtle p-4">
            <h4><b>Vyhledávání</b></h4>
            <div class="container">
                <div class="row">
                    <div class="col">
                        <div class="form-group">
                            <label for="name">Jméno</label>
                            <input bind:value={$search.name} id="name" type="text" class="form-control" />
                        </div>
                    </div>
                    <div class="col">
                        <div class="form-group">
                            <label for="name">Výrobce</label>
                            <input bind:value={$search.manufacturer} id="name" type="text" class="form-control" />
                        </div>
                    </div>
                    <div class="col">
                        <div class="form-group">
                            <label for="name">Velikost</label>
                            <input bind:value={$search.size} id="name" type="number" class="form-control" />
                        </div>
                    </div>
                    <div class="col">
                        <div class="form-group">
                            <label for="name">Cena</label>
                            <input bind:value={$search.price} id="name" type="number" class="form-control" />
                        </div>
                    </div>
                    <div class="d-flex gap-2 col align-items-end">
                        <button on:click={resetSearch} class="btn btn-secondary">Resetovat</button>
                        <button on:click={doSearch} class="btn btn-secondary">Vyhledat</button>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
