<script lang="ts">
    import { title } from '$/components/APP/store.svelte';
    import { IonButton, IonFabButton, IonInput, IonItemOption, IonItemOptions, IonModal, IonSegment } from '$/components/Ion';
    import { LocalStorageManager, SwalAlert } from '$/lib';
    import type { BankRecord, BankWithoutHash } from '$/types/types';
    import { goto } from '$app/navigation';
    import { page } from '$app/stores';
    import { add, sadOutline } from 'ionicons/icons';
    import { onMount } from 'svelte';

    let banks: LocalStorageManager;
    let bankData: BankWithoutHash;
    let records: BankRecord[];
    let uuid = $page.params.uuid;

    const getData = () => {
        banks = new LocalStorageManager();

        let data = banks.getBankData(uuid);

        if (!data) {
            goto('/');
            return;
        }

        bankData = data;

        title.set(`Prohlížení: ${data.name}`);

        records = banks.getRecords(uuid);
    };

    onMount(() => {
        getData();
    });

    let tab: 'log' | 'targets' = 'log';

    let refresher: HTMLIonRefresherElement;
    $: refresher?.addEventListener('ionRefresh', () => {
        getData();
        refresher.complete();
    });

    const addItem = () => {
        if (tab === 'log') {
            addLogOpened = true;
        } else {
            addTargetOpened = true;
        }
    };

    let addLogOpened = false;
    let logButtonDisabled = false;
    let value = 0;
    let description = '';
    const addLog = () => {
        if (value == 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej platnou částku'
            });
            return;
        }

        logButtonDisabled = true;

        banks.addRecord(uuid, value, description);
        getData();

        value = 0;
        description = '';

        addLogOpened = false;
        logButtonDisabled = false;
    };

    let addTargetOpened = false;
    let targetButtonDisabled = false;
    let targetValue = 0;
    let name = '';
    let targetDescription = '';
    const addTarget = () => {};
</script>

<ion-content class="ion-padding">
    <ion-refresher bind:this={refresher} slot="fixed">
        <ion-refresher-content></ion-refresher-content>
    </ion-refresher>

    {#if bankData}
        <div class="flex flex-col">
            <div class="flex flex-row justify-between">
                <ion-text>
                    <h3>Informace o účtu</h3>
                </ion-text>
            </div>
            <div class="flex flex-row justify-between">
                <ion-text>
                    <h4 class="m-0">Název: {bankData.name}</h4>
                </ion-text>
                <ion-text color={bankData.balance > 0 ? 'success' : bankData.balance < 0 ? 'danger' : ''}>
                    <h4 class="m-0">{bankData.balance}Kč</h4>
                </ion-text>
            </div>
            <div class="flex justify-between">
                <ion-text>Typ účtu: Lokální</ion-text>
                <ion-text color="success">Příjmy tento měsíc: {bankData.revenues}Kč</ion-text>
            </div>
            <div class="ml-auto">
                <ion-text color="danger">Výdaje tento měsíc: {bankData.expenses}Kč</ion-text>
            </div>
        </div>

        <IonSegment bind:value={tab}>
            <ion-segment-button value="log">
                <ion-label>Pohyby na účtě</ion-label>
            </ion-segment-button>
            <ion-segment-button value="targets">
                <ion-label>Cíle</ion-label>
            </ion-segment-button>
        </IonSegment>

        {#if tab === 'log'}
            {#if records.length == 0}
                <ion-text color="danger">
                    <h2 class="text-center flex justify-center">Žádný pohyb na účtě <ion-icon class="my-auto" icon={sadOutline} /></h2>
                </ion-text>
            {/if}
            <ion-list inset={true} lines="full">
                {#each records as record}
                    <ion-item-sliding>
                        <ion-item>
                            <div class="flex flex-col">
                                <ion-label>{record.date}</ion-label>
                                <ion-label>10</ion-label>
                            </div>
                        </ion-item>

                        <IonItemOptions onSwipe={console.log} side="end">
                            <IonItemOption on:click={console.log}>Upravit</IonItemOption>
                            <IonItemOption on:click={console.log} color="danger" expandable>Smazat</IonItemOption>
                        </IonItemOptions>
                    </ion-item-sliding>
                {/each}
            </ion-list>
        {:else}
            <ion-text color="danger">
                <h2 class="text-center flex justify-center">Nemáš dané žádné cíle <ion-icon class="my-auto" icon={sadOutline} /></h2>
            </ion-text>
        {/if}
        <ion-fab horizontal="end" vertical="bottom">
            <IonFabButton on:click={addItem}>
                <ion-icon icon={add} />
            </IonFabButton>
        </ion-fab>

        <IonModal bind:opened={addLogOpened}>
            <ion-header>
                <ion-toolbar>
                    <ion-buttons slot="start">
                        <IonButton color="danger" on:click={() => (addLogOpened = false)}>Zrušít</IonButton>
                    </ion-buttons>
                    <ion-title>Nový příjem/výdaj</ion-title>
                    <ion-buttons slot="end">
                        <IonButton bind:disabled={logButtonDisabled} color="success" on:click={addLog}>Přidat</IonButton>
                    </ion-buttons>
                </ion-toolbar>
            </ion-header>
            <ion-content class="ion-padding">
                <ion-item>
                    <IonInput label="Částka" type="number" bind:value />
                </ion-item>
                <ion-item>
                    <IonInput label="Popisek" type="text" bind:value={description} />
                </ion-item>
            </ion-content>
        </IonModal>
    {/if}
</ion-content>
