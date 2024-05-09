<script lang="ts">
    import { SwalAlert, arrayBufferToBase64, cleanFiles } from '$/lib/functions';
    import type { Item } from '$/types/types';
    import Path from 'path-browserify';
    import Button from './Button.svelte';
    import Modal from './Modal.svelte';
    import FileInput from './fileInput.svelte';

    export let data: Item | undefined = undefined;
    export let creating: boolean;

    $: {
        if (data === undefined) {
            data = {
                name: '',
                manufacturer: '',
                price: 0,
                size: 0,
                weight: 0,
                images: {
                    main: '',
                    other: []
                }
            };
        }
    }

    export let opened = false;
    export let closeFunction: () => void;
    export let cancelFunction: () => void;

    let names = ['Jméno', 'Výrobce', 'Velikost', 'Váha', 'Cena'];
    let values = ['name', 'manufacturer', 'size', 'weight', 'price'] as (keyof Item)[];
    let types = ['text', 'text', 'number', 'number', 'number'];

    const selectSingleImage = async (dirtyFiles: (File | null)[]) => {
        if (data === undefined) {
            return;
        }

        const files = cleanFiles(dirtyFiles);
        if (files.length == 0) {
            return;
        }

        if (files.length > 1) {
            SwalAlert({
                icon: 'error',
                title: 'Vyber pouze jeden obrázek!'
            });
            return;
        }

        const file = files[0];

        const imageData = await file.arrayBuffer();
        const fullString = `data:image/${Path.parse(file.name).ext};base64,${arrayBufferToBase64(imageData)}`;

        data.images.main = fullString;
    };

    const selectImages = async (dirtyFiles: (File | null)[]) => {
        if (data === undefined) {
            return;
        }

        const files = cleanFiles(dirtyFiles);
        if (files.length == 0) {
            return;
        }

        if (files.length > 4) {
            SwalAlert({
                icon: 'error',
                title: 'Vyber maximálně 4 obrázky!'
            });
            return;
        }

        data.images.other = [];

        for (const file of files) {
            const imageData = await file.arrayBuffer();
            const fullString = `data:image/${Path.parse(file.name).ext};base64,${arrayBufferToBase64(imageData)}`;

            data.images.other.push(fullString);
        }
    };
</script>

{#if data !== undefined}
    <Modal bind:show={opened} closeFunction={cancelFunction}>
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title">{creating ? 'Přidání nové položky' : 'Úprava položky'}</h5>
                <button on:click={cancelFunction} type="button" class="btn-close"></button>
            </div>
            <div class="modal-body">
                <table class="table">
                    <tbody>
                        {#each names as name, i}
                            <tr>
                                <th>{name}</th>
                                <td class="text-end"
                                    ><input
                                        class="form-control"
                                        type={types[i]}
                                        value={data[values[i]]}
                                        on:input={(ev) => {
                                            if (data !== undefined) {
                                                //@ts-ignore
                                                data[values[i]] = ev.currentTarget.value;
                                            }
                                            return;
                                        }}
                                    /></td
                                >
                            </tr>
                        {/each}
                    </tbody>
                </table>
            </div>
            <div class="modal-footer">
                <div class="d-flex gap-2">
                    <Button color="primary" on:click={closeFunction}>{creating ? 'Přidat' : 'Upravit'}</Button>
                    <Button color="secondary" on:click={cancelFunction}>Zrušit</Button>
                </div>
                <div class="d-flex gap-2">
                    <FileInput onDrop={selectSingleImage}>
                        <Button color="info">Nastavit náhled</Button>
                    </FileInput>
                    <FileInput onDrop={selectImages}>
                        <Button color="info">Nastavit obrázky (max 4)</Button>
                    </FileInput>
                </div>
            </div>
        </div>
    </Modal>
{/if}
