<script lang="ts">
    import { title } from '$/components/APP/store.svelte';
    import { IonButton, IonFabButton, IonInput, IonItemOption, IonItemOptions, IonModal, IonProgressBar, IonSegment } from '$/components/Ion';
    import { LocalStorageManager, SwalAlert, formatDate } from '$/lib';
    import type { BankRecord, BankTarget, BankWithoutHash } from '$/types/types';
    import { goto } from '$app/navigation';
    import { page } from '$app/stores';
    import FileSaver from 'file-saver';
    import { add, checkmarkCircleOutline, cogOutline, downloadOutline, sadOutline, trashOutline } from 'ionicons/icons';
    import { onMount } from 'svelte';
    import { writable } from 'svelte/store';

    let banks: LocalStorageManager;
    let bankData: BankWithoutHash;
    let records: BankRecord[];
    let targets: BankTarget[];
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

        //reverse array to be sorted as descending by id
        records.reverse();

        //

        targets = banks.getTargets(uuid);

        //reverse to be sorted as descending by id
        targets.reverse();
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
            $log.opened = true;
        } else {
            $target.opened = true;
        }
    };

    const log = writable({
        value: '0',
        description: '',
        buttonDisabled: false,
        opened: false
    });

    const addLog = () => {
        if (!$log.value) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej částku'
            });
            return;
        }

        const parsedValue = parseFloat($log.value);

        if (parsedValue == 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej platnou částku'
            });
            return;
        }

        $log.buttonDisabled = true;

        banks.addRecord(uuid, parsedValue, $log.description);
        getData();

        SwalAlert({
            icon: 'success',
            title: `${parsedValue > 0 ? 'Příjem' : 'Výdaj'} byl úspěšně přidán`
        });

        log.set({
            opened: false,
            buttonDisabled: false,
            value: '0',
            description: ''
        });
    };

    const target = writable({
        name: '',
        description: '',
        value: '0',
        buttonDisabled: false,
        opened: false
    });
    const addTarget = () => {
        if ($target.name.length == 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej název cíle'
            });
            return;
        }

        if ($target.description.length == 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej popisek cíle'
            });
            return;
        }

        if (!$target.value) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej částku'
            });
            return;
        }

        const parsedValue = parseFloat($target.value);

        if (parsedValue <= 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej platnou částku (>0)'
            });
            return;
        }

        $target.buttonDisabled = true;

        banks.addTarget(uuid, parsedValue, $target.name, $target.description);

        getData();

        SwalAlert({
            icon: 'success',
            title: 'Cíl byl úspěšně přidán'
        });

        target.set({
            name: '',
            description: '',
            value: '0',
            buttonDisabled: false,
            opened: false
        });
    };

    const logEdit = writable({
        value: '0',
        description: '',
        buttonDisabled: false,
        opened: false,
        sliding: null as HTMLIonItemSlidingElement | null,
        recordId: -1
    });
    const openEditLog = (id: number, ev: MouseEvent) => {
        const record = records.find((record) => record.id === id);

        if (!record) {
            return;
        }

        logEdit.set({
            buttonDisabled: false,
            value: record.value.toString(),
            description: record.description,
            opened: true,
            sliding: getSliding(ev),
            recordId: id
        });
    };

    const editLog = () => {
        if ($logEdit.recordId == -1) {
            return;
        }

        if (!$logEdit.value) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej částku'
            });
            return;
        }

        const parsedValue = parseFloat($logEdit.value);

        if (parsedValue == 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej platnou částku'
            });
            return;
        }

        $logEdit.buttonDisabled = true;

        banks.editRecord(uuid, $logEdit.recordId, parsedValue, $logEdit.description);
        getData();

        SwalAlert({
            icon: 'success',
            title: 'Záznam byl úspěšně upraven'
        });

        //close sliding when done
        $logEdit.sliding?.close();

        logEdit.set({
            opened: false,
            buttonDisabled: false,
            value: '0',
            description: '',
            sliding: null,
            recordId: -1
        });
    };

    const deleteLog = async (id: number, el: HTMLIonItemSlidingElement | null) => {
        const confirmation = await SwalAlert({
            toast: false,
            position: 'center',
            timer: 0,
            title: 'Opravdu cheš smazat tento záznam?',
            showConfirmButton: true,
            confirmButtonText: 'Ano',
            showCancelButton: true,
            cancelButtonText: 'Ne'
        });

        if (!confirmation.isConfirmed) {
            el?.close();
            return;
        }

        banks.removeRecord(uuid, id);
        getData();

        (el as HTMLIonItemSlidingElement | null)?.close();

        SwalAlert({
            icon: 'success',
            title: 'Záznam byl úspěšně smazán'
        });
    };

    const targetEdit = writable({
        name: '',
        description: '',
        value: '0',
        buttonDisabled: false,
        opened: false,
        sliding: null as HTMLIonItemSlidingElement | null,
        targetId: -1
    });
    const openEditTarget = (id: number, ev: MouseEvent) => {
        const target = targets.find((target) => target.id === id);

        if (!target) {
            return;
        }

        targetEdit.set({
            name: target.name,
            description: target.description,
            value: target.targetValue.toString(),
            buttonDisabled: false,
            opened: true,
            sliding: getSliding(ev),
            targetId: id
        });
    };

    const editTarget = () => {
        if ($targetEdit.targetId == -1) {
            return;
        }

        if ($targetEdit.name.length == 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej název cíle'
            });
            return;
        }

        if ($targetEdit.description.length == 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej popisek cíle'
            });
            return;
        }

        if (!$targetEdit.value) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej částku'
            });
            return;
        }

        const parsedValue = parseFloat($targetEdit.value);

        if (parsedValue == 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej platnou částku'
            });
            return;
        }

        $targetEdit.buttonDisabled = true;

        banks.editTarget(uuid, $targetEdit.targetId, parsedValue, $targetEdit.name, $targetEdit.description);
        getData();

        SwalAlert({
            icon: 'success',
            title: 'Cíl byl úspěšně upraven'
        });

        //close sliding when done
        $targetEdit.sliding?.close();

        targetEdit.set({
            opened: false,
            buttonDisabled: false,
            name: '',
            description: '',
            value: '0',
            sliding: null,
            targetId: -1
        });
    };

    const deleteTarget = async (id: number, el: HTMLIonItemSlidingElement | null) => {
        const confirmation = await SwalAlert({
            toast: false,
            position: 'center',
            timer: 0,
            title: 'Opravdu cheš smazat tento cíl?',
            showConfirmButton: true,
            confirmButtonText: 'Ano',
            showCancelButton: true,
            cancelButtonText: 'Ne'
        });

        if (!confirmation.isConfirmed) {
            el?.close();
            return;
        }

        banks.removeTarget(uuid, id);
        getData();

        (el as HTMLIonItemSlidingElement | null)?.close();

        SwalAlert({
            icon: 'success',
            title: 'Cíl byl úspěšně smazán'
        });
    };

    const getSliding = (ev: { currentTarget: EventTarget | null }) => {
        let target = ev.currentTarget as HTMLElement | null;
        if (!target) {
            return null;
        }

        if (target.localName == 'ion-item-sliding') {
            return target as HTMLIonItemSlidingElement;
        }

        while (true) {
            const parent = target.parentElement as HTMLElement | null;

            if (!parent) {
                return null;
            }

            if (target.localName == 'ion-item-sliding') {
                return target as HTMLIonItemSlidingElement;
            }

            target = parent;
        }
    };

    const completeTarget = (id: number) => {
        const target = targets.find((target) => target.id === id);
        if (!target) {
            return;
        }

        if (bankData.balance < target.targetValue) {
            SwalAlert({
                icon: 'error',
                title: 'Nelze splnit cíl, protože jsi nedosáhnul cílové částky.'
            });
            return;
        }

        banks.removeTarget(uuid, id);
        banks.addRecord(uuid, -1 * target.targetValue, `Cíl: ${target.name}`);

        SwalAlert({
            icon: 'success',
            title: 'Cíl byl úspěšně splněn'
        });

        getData();
    };

    const bankSettings = writable({
        opened: false,
        buttonDisabled: false,
        name: ''
    });
    const openSettings = () => {
        bankSettings.set({
            opened: true,
            buttonDisabled: false,
            name: bankData.name
        });
    };

    const exportBank = () => {
        const result = banks.exportBank(uuid);
        if (result === null) {
            SwalAlert({
                icon: 'error',
                title: 'Export se nepovedl :/'
            });
            return;
        }

        const string = JSON.stringify(result);

        const blob = new Blob([string], { type: 'text/plain;charset=utf-8' });

        FileSaver.saveAs(blob, `export-${uuid}.json`);

        SwalAlert({
            icon: 'success',
            title: 'Export byl stažen'
        });
    };

    const deleteBank = async () => {
        const confirmation = await SwalAlert({
            toast: false,
            position: 'center',
            timer: 0,
            title: 'Opravdu cheš smazat tuto banku?',
            showConfirmButton: true,
            confirmButtonText: 'Ano',
            showCancelButton: true,
            cancelButtonText: 'Ne'
        });

        if (!confirmation.isConfirmed) {
            return;
        }

        $bankSettings.opened = false;

        banks.removeBank(uuid);

        SwalAlert({
            icon: 'success',
            title: 'Bank byla smazána'
        });

        goto('/');
    };

    const editBank = () => {
        if ($bankSettings.name.length == 0) {
            SwalAlert({
                icon: 'error',
                title: 'Zadej platné jméno'
            });
            return;
        }

        $bankSettings.buttonDisabled = true;

        banks.editBank(uuid, $bankSettings.name);
        getData();

        bankSettings.set({
            buttonDisabled: false,
            name: '',
            opened: false
        });

        SwalAlert({
            icon: 'success',
            title: 'Banka úspěšně upravena.'
        });
    };
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
                <!-- svelte-ignore a11y-click-events-have-key-events -->
                <!-- svelte-ignore a11y-no-static-element-interactions -->
                <ion-text style="cursor:pointer;" color="primary" on:click={openSettings}>
                    <h1><ion-icon icon={cogOutline} /></h1>
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
                            <div class="flex flex-row justify-between w-full">
                                <div class="flex flex-col my-auto">
                                    <ion-label>{formatDate(record.date)}</ion-label>
                                    <ion-label>{record.description}</ion-label>
                                </div>

                                <div class="flex flex-col">
                                    <ion-text color={record.value > 0 ? 'success' : 'danger'}><h3>{record.value}Kč</h3></ion-text>
                                </div>
                            </div>
                        </ion-item>

                        <IonItemOptions onSwipe={(el) => deleteLog(record.id, getSliding({ currentTarget: el }))} side="end">
                            <IonItemOption on:click={(ev) => openEditLog(record.id, ev)}>Upravit</IonItemOption>
                            <IonItemOption on:click={(ev) => deleteLog(record.id, getSliding(ev))} color="danger" expandable>Smazat</IonItemOption>
                        </IonItemOptions>
                    </ion-item-sliding>
                {/each}
            </ion-list>
        {:else}
            {#if targets.length == 0}
                <ion-text color="danger">
                    <h2 class="text-center flex justify-center">Nemáš dané žádné cíle <ion-icon class="my-auto" icon={sadOutline} /></h2>
                </ion-text>
            {/if}
            <ion-list inset={true} lines="full">
                {#each targets as target}
                    <ion-item-sliding>
                        <ion-item>
                            <div class="flex flex-col w-full">
                                <div class="flex flex-row w-full justify-between">
                                    <div class="flex flex-col my-auto">
                                        <ion-label>{target.name}</ion-label>
                                        <ion-label>{target.description}</ion-label>
                                    </div>

                                    <div class="flex flex-row" style="gap:8px">
                                        <ion-text><h3>{target.targetValue}Kč</h3></ion-text>
                                        <!-- svelte-ignore a11y-click-events-have-key-events -->
                                        <!-- svelte-ignore a11y-no-static-element-interactions -->
                                        <ion-text
                                            style="cursor:{bankData.balance >= target.targetValue ? 'pointer' : 'not-allowed'}"
                                            color={bankData.balance >= target.targetValue ? 'success' : 'danger'}
                                            on:click={() => completeTarget(target.id)}
                                        >
                                            <h3><ion-icon icon={checkmarkCircleOutline} /></h3>
                                        </ion-text>
                                    </div>
                                </div>
                                <IonProgressBar percentage={100 * (bankData.balance / target.targetValue)} />
                            </div>
                        </ion-item>

                        <IonItemOptions onSwipe={(el) => deleteTarget(target.id, getSliding({ currentTarget: el }))} side="end">
                            <IonItemOption on:click={(ev) => openEditTarget(target.id, ev)}>Upravit</IonItemOption>
                            <IonItemOption on:click={(ev) => deleteTarget(target.id, getSliding(ev))} color="danger" expandable>Smazat</IonItemOption>
                        </IonItemOptions>
                    </ion-item-sliding>
                {/each}
            </ion-list>
        {/if}
        <ion-fab horizontal="end" vertical="bottom">
            <IonFabButton on:click={addItem}>
                <ion-icon icon={add} />
            </IonFabButton>
        </ion-fab>

        <IonModal bind:opened={$log.opened}>
            <ion-header>
                <ion-toolbar>
                    <ion-buttons slot="start">
                        <IonButton color="danger" on:click={() => ($log.opened = false)}>Zrušít</IonButton>
                    </ion-buttons>
                    <ion-title>Nový příjem/výdaj</ion-title>
                    <ion-buttons slot="end">
                        <IonButton bind:disabled={$log.buttonDisabled} color="success" on:click={addLog}>Přidat</IonButton>
                    </ion-buttons>
                </ion-toolbar>
            </ion-header>
            <ion-content class="ion-padding">
                <ion-item>
                    <IonInput label="Částka" placeholder="10" type="number" bind:value={$log.value} />
                </ion-item>
                <ion-item>
                    <IonInput label="Popisek" placeholder="Kapesné" type="text" bind:value={$log.description} />
                </ion-item>
            </ion-content>
        </IonModal>

        <IonModal bind:opened={$logEdit.opened}>
            <ion-header>
                <ion-toolbar>
                    <ion-buttons slot="start">
                        <IonButton color="danger" on:click={() => ($logEdit.opened = false)}>Zrušít</IonButton>
                    </ion-buttons>
                    <ion-title>Úprava příjmu/výdaje</ion-title>
                    <ion-buttons slot="end">
                        <IonButton bind:disabled={$logEdit.buttonDisabled} color="success" on:click={editLog}>Upravit</IonButton>
                    </ion-buttons>
                </ion-toolbar>
            </ion-header>
            <ion-content class="ion-padding">
                <ion-item>
                    <IonInput label="Částka" placeholder="10" type="number" bind:value={$logEdit.value} />
                </ion-item>
                <ion-item>
                    <IonInput label="Popisek" placeholder="Kapesné" type="text" bind:value={$logEdit.description} />
                </ion-item>
            </ion-content>
        </IonModal>

        <IonModal bind:opened={$target.opened}>
            <ion-header>
                <ion-toolbar>
                    <ion-buttons slot="start">
                        <IonButton color="danger" on:click={() => ($target.opened = false)}>Zrušít</IonButton>
                    </ion-buttons>
                    <ion-title>Nový cíl</ion-title>
                    <ion-buttons slot="end">
                        <IonButton bind:disabled={$target.buttonDisabled} color="success" on:click={addTarget}>Přidat</IonButton>
                    </ion-buttons>
                </ion-toolbar>
            </ion-header>
            <ion-content class="ion-padding">
                <ion-item>
                    <IonInput label="Jméno" type="text" placeholder="Auto" bind:value={$target.name} />
                </ion-item>
                <ion-item>
                    <IonInput label="Popisek" type="text" placeholder="Hyundai i30 fastback mild hybrid" bind:value={$target.description} />
                </ion-item>
                <ion-item>
                    <IonInput label="Cíl" type="number" placeholder="450467" bind:value={$target.value} />
                </ion-item>
            </ion-content>
        </IonModal>

        <IonModal bind:opened={$targetEdit.opened}>
            <ion-header>
                <ion-toolbar>
                    <ion-buttons slot="start">
                        <IonButton color="danger" on:click={() => ($targetEdit.opened = false)}>Zrušít</IonButton>
                    </ion-buttons>
                    <ion-title>Úprava cíle</ion-title>
                    <ion-buttons slot="end">
                        <IonButton bind:disabled={$targetEdit.buttonDisabled} color="success" on:click={editTarget}>Upravit</IonButton>
                    </ion-buttons>
                </ion-toolbar>
            </ion-header>
            <ion-content class="ion-padding">
                <ion-item>
                    <IonInput label="Jméno" type="text" placeholder="Auto" bind:value={$targetEdit.name} />
                </ion-item>
                <ion-item>
                    <IonInput label="Popisek" type="text" placeholder="Hyundai i30 fastback mild hybrid" bind:value={$targetEdit.description} />
                </ion-item>
                <ion-item>
                    <IonInput label="Cíl" type="number" placeholder="450467" bind:value={$targetEdit.value} />
                </ion-item>
            </ion-content>
        </IonModal>

        <IonModal bind:opened={$bankSettings.opened}>
            <ion-header>
                <ion-toolbar>
                    <ion-buttons slot="start">
                        <IonButton color="danger" on:click={() => ($bankSettings.opened = false)}>Zrušít</IonButton>
                    </ion-buttons>
                    <ion-title>Úprava banky</ion-title>
                    <ion-buttons slot="end">
                        <IonButton bind:disabled={$bankSettings.buttonDisabled} color="success" on:click={editBank}>Upravit</IonButton>
                    </ion-buttons>
                </ion-toolbar>
            </ion-header>
            <ion-content class="ion-padding">
                <ion-item>
                    <IonInput label="Jméno" type="text" placeholder="Super banka" bind:value={$bankSettings.name} />
                </ion-item>
                <div class="flex flex-row justify-around">
                    <IonButton on:click={exportBank}><ion-icon icon={downloadOutline} /> Export účtu</IonButton>
                    <IonButton color="danger" on:click={deleteBank}><ion-icon icon={trashOutline} /> Smazání účtu</IonButton>
                </div>
            </ion-content>
        </IonModal>
    {/if}
</ion-content>
