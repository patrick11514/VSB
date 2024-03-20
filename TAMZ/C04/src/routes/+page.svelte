<script lang="ts">
    import { IonFabButton, IonInput, RadioGroup } from "$/components/Ion"
    import IonActionSheet from "$/components/Ion/ionActionSheet.svelte"
    import IonButton from "$/components/Ion/ionButton.svelte"
    import IonDateTime from "$/components/Ion/ionDateTime.svelte"
    import IonItemOption from "$/components/Ion/ionItemOption.svelte"
    import IonItemOptions from "$/components/Ion/ionItemOptions.svelte"
    import IonModal from "$/components/Ion/ionModal.svelte"
    import { SwalAlert } from "$/lib"
    import { add } from "ionicons/icons"
    import { onMount } from "svelte"
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
        type: z
            .literal("arctic")
            .or(z.literal("normal"))
            .or(z.literal("ba95"))
            .or(z.literal("ba100")),
        gasStation: z.string().min(1),
        volume: z.coerce.number().min(1),
        tachometer: z.coerce.number().min(1),
    })

    let elements: z.infer<typeof itemSchema>[] = []

    let modalOpen = false
    let updating = false
    let updatingId: number | undefined = undefined

    const addRecord = () => {
        if (updating) {
            update()
            return
        }

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

        try {
            const items = localStorage.getItem("items")

            if (!items) {
                localStorage.setItem("items", Number(1).toString())
                localStorage.setItem("item0", JSON.stringify(item.data))
            } else {
                localStorage.setItem(
                    "items",
                    Number(Number(items) + 1).toString()
                )
                localStorage.setItem("item" + items, JSON.stringify(item.data))
            }

            SwalAlert({
                icon: "success",
                title: "Záznam byl úspěšně přidán",
            })

            modalOpen = false

            //clear inputs
            resetInputs()
            loadItems()
        } catch (_) {
            SwalAlert({
                icon: "error",
                title: "Něco se pokazilo, zkuste to prosím znovu",
            })
        }
    }

    const resetInputs = () => {
        date = undefined
        price = 0
        type = ""
        gasStation = ""
        volume = 0
        tachometer = 0
    }

    onMount(() => {
        loadItems()
    })

    let totalPrice = 0

    const loadItems = () => {
        const items = localStorage.getItem("items")

        if (!items) return

        const numberItems = Number(items)

        elements = []

        for (let i = 0; i < numberItems; i++) {
            const item = localStorage.getItem("item" + i)

            if (item) {
                const parsedJson = itemSchema.safeParse(JSON.parse(item))

                if (parsedJson.success) {
                    elements = [...elements, parsedJson.data]
                    totalPrice += parsedJson.data.price * parsedJson.data.volume
                } else {
                    console.log(item)
                    console.log(parsedJson.error)
                }
            }
        }
    }

    const deleteItem = async (item: number) => {
        const result = await SwalAlert({
            toast: false,
            position: "center",
            timer: 0,
            title: "Opravdu chceš smazat tento záznam?",
            showConfirmButton: true,
            confirmButtonText: "Ano",
            showCancelButton: true,
            cancelButtonText: "Ne",
        })

        if (!result.isConfirmed) return

        //delete
        elements = elements.filter((_, i) => i !== item)

        //clear storage
        const items = localStorage.getItem("items")

        if (!items) {
            return
        }

        const numberItems = Number(items)

        for (let i = 0; i < numberItems; i++) {
            localStorage.removeItem("item" + i)
        }

        //add back

        try {
            localStorage.setItem("items", elements.length.toString())

            for (let i = 0; i < elements.length; i++) {
                localStorage.setItem(
                    "item" + i.toString(),
                    JSON.stringify(elements[i])
                )
            }

            loadItems()
        } catch (_) {
            SwalAlert({
                icon: "error",
                title: "Nepovedlo se smazat záznam.",
            })
        }
    }

    const startEdit = (id: number) => {
        updating = true

        const data = elements[id]

        //fill inputs
        date = data.date
        price = data.price
        type = data.type
        gasStation = data.gasStation
        volume = data.volume
        tachometer = data.tachometer

        updatingId = id

        modalOpen = true
    }

    const update = () => {
        if (updatingId === undefined) return

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

        try {
            localStorage.setItem(
                "item" + updatingId.toString(),
                JSON.stringify(item.data)
            )

            modalOpen = false
            updatingId = undefined
            resetInputs()

            loadItems()
        } catch (_) {
            SwalAlert({
                icon: "error",
                title: "Nepovedlo se upravit záznam.",
            })
        }
    }

    const modalCancel = () => {
        modalOpen = false

        if (updating) {
            resetInputs()
            updating = false
            updatingId = undefined
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
        {#if elements.length == 0}
            <h2>Žádný záznam nenalezen, přidej nějaký.</h2>
        {:else}
            {#each elements as item, id}
                <ion-item-sliding id="{id}-{item.price}-{item.tachometer}}">
                    <ion-item>
                        <ion-label>
                            {item.volume}l
                        </ion-label>
                        <ion-label style="text-align:right;">
                            {item.price}Kč/l
                        </ion-label>
                    </ion-item>

                    <IonItemOptions onSwipe={() => deleteItem(id)} side="end">
                        <IonItemOption on:click={() => startEdit(id)}>
                            Upravit
                        </IonItemOption>
                        <IonItemOption
                            on:click={() => deleteItem(id)}
                            color="danger"
                            expandable
                        >
                            Smazat
                        </IonItemOption>
                    </IonItemOptions>
                </ion-item-sliding>
            {/each}
        {/if}
    </ion-list>

    <IonActionSheet header="Přidat nový záznam"></IonActionSheet>

    <IonModal bind:opened={modalOpen}>
        <ion-header>
            <ion-toolbar>
                <ion-buttons slot="start">
                    <IonButton on:click={modalCancel}>Cancel</IonButton>
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
