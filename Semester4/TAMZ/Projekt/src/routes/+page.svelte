<script lang="ts">
    import { IonButton, IonCheckBox, IonFabButton, IonInput, IonModal } from '$/components/Ion';
    import FileInput from '$/components/fileInput.svelte';
    import { LocalStorageManager, SwalAlert } from '$/lib';
    import { API } from '$/lib/api';
    import type { BankWithoutHash } from '$/types/types';
    import { add, eyeOffOutline, eyeOutline, walletOutline } from 'ionicons/icons';
    import { onMount } from 'svelte';
    import { writable } from 'svelte/store';
    import * as uuid from 'uuid';
    import { z } from 'zod';

    let localBanks: LocalStorageManager;
    let banks = writable<BankWithoutHash[]>([]);
    let onlineBanks = writable<BankWithoutHash[]>([]);

    const loadData = async () => {
        const response = await API.bank.GET();

        if (!response.status) {
            SwalAlert({
                icon: 'error',
                title: 'Nepovedlo se načíst banky na serveru.'
            });
            return;
        }

        onlineBanks.set(response.data);
    };

    onMount(() => {
        const change = () => {
            const bankList = localBanks.getBanks();
            banks.set(bankList.map((bank) => localBanks.getBankData(bank.uuid)!));
        };

        localBanks = new LocalStorageManager();
        localBanks.on('change', change);
        const bankList = localBanks.getBanks();
        banks.set(bankList.map((bank) => localBanks.getBankData(bank.uuid)!));

        loadData();

        return () => {
            localBanks.off('change', change);
        };
    });

    let createOpened = false;

    const openCreate = () => {
        confirmDisabled = false;
        name = '';
        password = '';
        passwordShow = false;
        createOpened = true;
    };

    const createBank = async () => {
        confirmDisabled = true;
        if (!saveOnServer) {
            localBanks.addBank({
                uuid: uuid.v4(),
                name: name,
                password: password
            });

            SwalAlert({
                icon: 'success',
                title: 'Banka byla úspěšně vytvořena'
            });
        } else {
            const response = await API.bank.PUT({
                name,
                password
            });

            if (!response.status) {
                SwalAlert({
                    icon: 'error',
                    title: 'Nepovedlo se vytvořit banku.'
                });
            } else {
                SwalAlert({
                    icon: 'success',
                    title: 'Banka byla úspěšně vytvořena.'
                });
                loadData();
            }
        }

        confirmDisabled = false;
        createOpened = false;
    };

    let confirmDisabled = false;
    let saveOnServer = false;
    let name = '';
    let password = '';
    let passwordShow = false;

    const importFile = async (iFiles: (File | null)[]) => {
        const files = iFiles.filter((file) => file !== null) as File[];

        if (files.length == 0) {
            SwalAlert({
                title: 'Vybel jeden soubor',
                icon: 'error'
            });
            return;
        }

        if (files.length > 1) {
            SwalAlert({
                title: 'Vybel pouze jeden soubor',
                icon: 'error'
            });
            return;
        }

        const file = files[0];
        const buffer = await file.arrayBuffer();

        const decoder = new TextDecoder('UTF-8');
        const string = decoder.decode(buffer);

        try {
            const json = JSON.parse(string) as object;

            if (!('local' in json)) {
                throw 'Invalid file structure';
            }

            if (typeof json.local !== 'boolean') {
                throw 'Invalid local type';
            }

            if (json.local) {
                const result = localBanks.importBank(json);

                if (result === -1) {
                    throw 'Invalid file structure';
                }

                if (result === 0) {
                    SwalAlert({
                        title: 'Tuto banku máš již importnutou',
                        icon: 'error'
                    });
                    return;
                }
            } else {
                const request = await API.import(json);

                if (!request.status) {
                    if (request.message == '-1') {
                        throw 'Invalid file structure';
                    }

                    if (request.message == '0') {
                        SwalAlert({
                            title: 'Tuto banku máš již importnutou',
                            icon: 'error'
                        });
                        return;
                    }
                    return;
                }

                await loadData();
            }

            SwalAlert({
                title: 'Banka byla úspěšně importována',
                icon: 'success'
            });

            createOpened = false;
        } catch (_) {
            SwalAlert({
                title: 'Vložil jsi neplatný soubor',
                icon: 'error'
            });
            return;
        }
    };

    const login = writable({
        uuid: '',
        password: '',
        opened: false,
        disabledButton: false
    });

    const doLogin = async () => {
        const parsedUuid = z.string().uuid().safeParse($login.uuid);
        if (!parsedUuid.success) {
            SwalAlert({
                title: 'Uuid banky je neplatné',
                icon: 'error'
            });
            return;
        }

        if ($login.password.length === 0) {
            SwalAlert({
                title: 'Heslo nesmí být prázdné',
                icon: 'error'
            });
            return;
        }

        const request = await API.addServer({
            uuid: $login.uuid,
            password: $login.password
        });

        if (!request.status) {
            SwalAlert({
                title: request.message,
                icon: 'error'
            });
            return;
        }

        await loadData();

        SwalAlert({
            title: 'Banka byla úspěšně přidána',
            icon: 'success'
        });

        $login.opened = false;
    };
</script>

<ion-content class="ion-padding">
    <ion-fab horizontal="end" vertical="bottom">
        <IonFabButton on:click={openCreate}>
            <ion-icon icon={add} />
        </IonFabButton>
    </ion-fab>

    <IonModal bind:opened={createOpened}>
        <ion-header>
            <ion-toolbar>
                <ion-buttons slot="start">
                    <IonButton color="danger" on:click={() => (createOpened = false)}>Zrušit</IonButton>
                </ion-buttons>
                <ion-title>Nový účet</ion-title>
                <ion-buttons slot="end">
                    <IonButton bind:disabled={confirmDisabled} color="success" on:click={createBank}>Vytvořit</IonButton>
                </ion-buttons>
            </ion-toolbar>
        </ion-header>
        <ion-content class="ion-padding">
            <ion-item>
                <IonCheckBox bind:checked={saveOnServer} labelPlacement="start">Uložit na serveru?</IonCheckBox>
            </ion-item>
            <ion-item>
                <IonInput bind:value={name} label="Jméno" placeholder="Nějaký účet" type="text" />
            </ion-item>
            <ion-item>
                <IonInput bind:value={password} label="Heslo" placeholder="Heslo" type={passwordShow ? 'text' : 'password'} />
                <IonButton on:click={() => (passwordShow = !passwordShow)}><ion-icon icon={passwordShow ? eyeOutline : eyeOffOutline} /></IonButton>
            </ion-item>
            <div class="flex flex-row justify-around">
                <FileInput onDrop={importFile}>
                    <IonButton>Import účtu</IonButton>
                </FileInput>

                <IonButton
                    on:click={() => {
                        createOpened = false;
                        $login.opened = true;
                    }}>Přihlášení do účtu</IonButton
                >
            </div>
        </ion-content>
    </IonModal>

    <ion-list lines="full">
        {#if $banks.length > 0}
            <h3>Lokální banky</h3>
            {#each $banks as bank}
                <a href="/localBank/{bank.uuid}" style="text-decoration: none;">
                    <ion-item>
                        <ion-label>
                            <h1>
                                <ion-icon icon={walletOutline} />
                                {bank.name}
                            </h1>
                            <ion-text color="success">Příjmy: {bank.revenues}Kč</ion-text>
                            <ion-text color="danger">Výdaje: {bank.expenses}Kč</ion-text>
                        </ion-label>
                        <ion-note slot="end"><h5>{bank.balance}Kč</h5></ion-note>
                    </ion-item>
                </a>
            {/each}
        {/if}
        {#if $onlineBanks.length > 0}
            <h3>Online banky</h3>
            {#each $onlineBanks as bank}
                <a href="/bank/{bank.uuid}" style="text-decoration: none;">
                    <ion-item>
                        <ion-label>
                            <h1>
                                <ion-icon icon={walletOutline} />
                                {bank.name}
                            </h1>
                            <ion-text color="success">Příjmy: {bank.revenues}Kč</ion-text>
                            <ion-text color="danger">Výdaje: {-1 * bank.expenses}Kč</ion-text>
                        </ion-label>
                        <ion-note slot="end"><h6>{bank.balance}Kč</h6></ion-note>
                    </ion-item>
                </a>
            {/each}
        {/if}
    </ion-list>

    <IonModal bind:opened={$login.opened}>
        <ion-header>
            <ion-toolbar>
                <ion-buttons slot="start">
                    <IonButton color="danger" on:click={() => ($login.opened = false)}>Zrušit</IonButton>
                </ion-buttons>
                <ion-title>Přihlášení do účtu</ion-title>
                <ion-buttons slot="end">
                    <IonButton bind:disabled={$login.disabledButton} color="success" on:click={doLogin}>Přihlásit se</IonButton>
                </ion-buttons>
            </ion-toolbar>
        </ion-header>
        <ion-content class="ion-padding">
            <ion-item>
                <IonInput label="Uuid banky" placeholder="6e4c1c30-c737-4aa1-8597-412222c75221" bind:value={$login.uuid} type="text" />
            </ion-item>
            <ion-item>
                <IonInput label="Heslo" placeholder="Tajné heslo" bind:value={$login.password} type="password" />
            </ion-item>
        </ion-content>
    </IonModal>
</ion-content>
