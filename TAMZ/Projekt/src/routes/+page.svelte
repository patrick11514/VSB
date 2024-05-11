<script lang="ts">
    import { IonButton, IonCheckBox, IonFabButton, IonInput, IonModal } from '$/components/Ion';
    import { LocalStorageManager, SwalAlert } from '$/lib';
    import { API } from '$/lib/api';
    import type { BankWithoutHash } from '$/types/types';
    import { add, eyeOffOutline, eyeOutline, walletOutline } from 'ionicons/icons';
    import { onMount } from 'svelte';
    import { writable } from 'svelte/store';
    import * as uuid from 'uuid';

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
        localBanks = new LocalStorageManager();
        localBanks.on('change', () => {
            const bankList = localBanks.getBanks();
            banks.set(bankList.map((bank) => localBanks.getBankData(bank.uuid)!));
        });
        const bankList = localBanks.getBanks();
        banks.set(bankList.map((bank) => localBanks.getBankData(bank.uuid)!));

        loadData();
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
</ion-content>
