<script lang="ts">
    import { IonFabButton, IonInput, RadioGroup } from "$/components/Ion"
    import IonActionSheet from "$/components/Ion/ionActionSheet.svelte"
    import IonButton from "$/components/Ion/ionButton.svelte"
    import IonDateTime from "$/components/Ion/ionDateTime.svelte"
    import IonItemOption from "$/components/Ion/ionItemOption.svelte"
    import IonModal from "$/components/Ion/ionModal.svelte"
    import { SwalAlert } from "$/lib"
    import { add } from "ionicons/icons"
    import { z } from "zod"

    let date: undefined | string = undefined
    let price = 0
    let type: "arctic" | "normal" | "ba95" | "ba100" | "" = ""
    let gasStation = ""
    let volume = 0
    let tachometer = 0

    let itemSchema = z.object({
        date: z.string(),
        price: z.coerce.number().min(1),
        type: z.string().min(1),
        gasStation: z.string().min(1),
        volume: z.coerce.number().min(1),
        tachometer: z.coerce.number().min(1),
    })

    let items: z.infer<typeof itemSchema>[] = []

    let modalOpen = false
    let updating = false

    const addRecord = () => {
        const item = itemSchema.safeParse({
            date,
            price,
            type,
            gasStation,
            volume,
            tachometer,
        })

        if (!item.success) {
            SwalAlert({
                icon: "error",
                title: "Vyplň prosím všechny hodnoty",
            })
            return
        }
    }
</script>

<ion-menu content-id="main-content">
    <ion-header>
        <ion-toolbar>
            <ion-title>Menu</ion-title>
        </ion-toolbar>
    </ion-header>
    <ion-content class="ion-padding">This is the menu content.</ion-content>
</ion-menu>
<ion-content id="main-content" class="ion-padding">
    <ion-fab horizontal="end" vertical="bottom" slot="fixed">
        <IonFabButton
            activated={modalOpen}
            on:click={() => (modalOpen = !modalOpen)}
        >
            <ion-icon icon={add}></ion-icon>
        </IonFabButton>
    </ion-fab>

    <ion-list>
        {#if items.length == 0}
            <h2>Žádný záznam nenalezen, přidej nějaký.</h2>
        {:else}
            {#each items as item, id}
                <ion-item-sliding>
                    <ion-item>
                        <ion-label>
                            Sliding Item with Expandable Options
                        </ion-label>
                    </ion-item>

                    <ion-item-options side="end">
                        <IonItemOption>Favorite</IonItemOption>
                        <IonItemOption color="danger" expandable>
                            Delete
                        </IonItemOption>
                    </ion-item-options>
                </ion-item-sliding>
            {/each}
        {/if}
    </ion-list>

    <IonActionSheet header="Přidat nový záznam"></IonActionSheet>

    <IonModal bind:opened={modalOpen}>
        <ion-header>
            <ion-toolbar>
                <ion-buttons slot="start">
                    <IonButton on:click={() => (modalOpen = false)}>
                        Cancel
                    </IonButton>
                </ion-buttons>
                <ion-title>
                    {#if !updating}
                        Přidání nového záznamu
                    {:else}
                        Úprava záznamu
                    {/if}
                </ion-title>
                <ion-buttons slot="end">
                    <IonButton on:click={addRecord} strong={true}>
                        Confirm
                    </IonButton>
                </ion-buttons>
            </ion-toolbar>
        </ion-header>
        <ion-content class="ion-padding">
            <ion-item>
                <ion-label>Datum tankování</ion-label>
                <ion-datetime-button datetime="datetime" />

                <ion-modal>
                    <IonDateTime
                        id="datetime"
                        hourCycle="h23"
                        presentation="date"
                        bind:value={date}
                    />
                </ion-modal>
            </ion-item>
            <ion-item>
                <IonInput
                    label="Cena za litr"
                    labelPlacement="stacked"
                    type="number"
                    min={0}
                    bind:value={price}
                />
            </ion-item>
            <ion-item>
                <ion-label>Typ paliva</ion-label>
                <RadioGroup class="group" bind:value={type}>
                    <div class="group-row gap">
                        <div class="group gap">
                            <ion-radio value="ba95">BA 95</ion-radio>
                            <ion-radio value="ba100">BA 100</ion-radio>
                        </div>
                        <div class="group gap">
                            <ion-radio value="normal">Nafta</ion-radio>
                            <ion-radio value="arctic">Arktická nafta</ion-radio>
                        </div>
                    </div>
                </RadioGroup>
            </ion-item>
            <ion-item>
                <IonInput
                    label="Čerpací stanice"
                    labelPlacement="stacked"
                    type="text"
                    bind:value={gasStation}
                />
            </ion-item>
            <ion-item>
                <IonInput
                    label="Počet litrů"
                    labelPlacement="stacked"
                    type="number"
                    min={0}
                    bind:value={volume}
                />
            </ion-item>
            <ion-item>
                <IonInput
                    label="Stav tachometru"
                    labelPlacement="stacked"
                    type="number"
                    min={0}
                    bind:value={tachometer}
                />
            </ion-item>
        </ion-content>
    </IonModal>

    <!--<h2>Historie tankování</h2>

    <h4>Datum tankování</h4>
    <ion-datetime-button datetime="datetime" />

    <ion-modal>
        <IonDateTime
            id="datetime"
            hourCycle="h23"
            presentation="date"
            bind:value={date}
        />
    </ion-modal>
    <h4>Cena za litr</h4>
    <ion-item>
        <IonInput type="number" min={0} bind:value={price} />
    </ion-item>

    <h4>Typ paliva</h4>
    <RadioGroup class="group" bind:value={type}>
        <div class="group-row between">
            <ion-radio value="ba95">BA 95</ion-radio>
            <ion-radio value="ba100">BA 100</ion-radio>
        </div>
        <div class="group-row between">
            <ion-radio value="normal">Nafta</ion-radio>
            <ion-radio value="arctic">Arktická nafta</ion-radio>
        </div>
    </RadioGroup>

    <h4>Čerpací stanice</h4>
    <ion-item>
        <IonInput type="text" bind:value={gasStation} />
    </ion-item>

    <h4>Počet litrů</h4>
    <ion-item>
        <IonInput type="number" min={0} bind:value={volume} />
    </ion-item>

    <h4>Stav tachometru</h4>
    <ion-item>
        <IonInput type="number" min={0} bind:value={tachometer} />
    </ion-item>

    <IonSelect
        bind:value={selectedEntry}
        label="Vyber položku"
        placeholder="Vyber záznam"
    >
        {#if entries.length === 0}
            <ion-select-option value="" disabled>
                Nenalezeny žádne záznamy
            </ion-select-option>
        {:else}
            {#each entries as entry, i}
                <ion-select-option value={i.toString()}>
                    Záznam z {formatDate(new Date(entry.date))}
                </ion-select-option>
            {/each}
        {/if}
    </IonSelect>

    <IonButton on:click={deleteData} color="danger">Smazat</IonButton>
    <IonButton on:click={newData} color="primary">Nový</IonButton>
    <IonButton on:click={saveData} color="success">Uložit</IonButton>-->
</ion-content>
