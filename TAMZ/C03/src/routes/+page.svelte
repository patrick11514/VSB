<script lang="ts">
    import {
        IonButton,
        IonDateTime,
        IonInput,
        IonSelect,
        RadioGroup,
    } from "$/components/Ion"
    import { SwalAlert, formatDate, getISO } from "$/lib"
    import { onMount } from "svelte"
    import { z } from "zod"

    let entries: {
        date: string
        price: number
        type: "arctic" | "normal" | "ba95" | "ba100"
        gasStation: string
        volume: number
        tachometer: number
    }[] = []
    let selectedEntry: undefined | string = undefined

    let date: undefined | string = undefined
    let price = 0
    let type: "arctic" | "normal" | "ba95" | "ba100" | "" = ""
    let gasStation = ""
    let volume = 0
    let tachometer = 0
    let silenced = false

    const schema = z.object({
        date: z.string(),
        price: z.coerce.number().min(1),
        type: z.string().min(1),
        gasStation: z.string().min(1),
        volume: z.coerce.number().min(1),
        tachometer: z.coerce.number().min(1),
    })

    const saveData = () => {
        const obj = schema.safeParse({
            date: date,
            price,
            type,
            gasStation,
            volume,
            tachometer,
        })

        if (!obj.success) {
            SwalAlert({
                icon: "error",
                title: "Vyplň prosím všechny hodnoty.",
            })
            return
        }

        if (selectedEntry === undefined || selectedEntry.length == 0) {
            let items = localStorage.getItem("items")

            try {
                let id: number
                if (!items) {
                    id = 0
                } else {
                    id = parseInt(items) + 1
                }

                if (isNaN(id)) {
                    throw new Error("ID is not a number")
                }

                localStorage.setItem("items", id.toString())
                localStorage.setItem(
                    "item" + id.toString(),
                    JSON.stringify({ ...obj.data, tachometer: undefined })
                )

                sessionStorage.setItem(
                    "item" + id.toString(),
                    obj.data.tachometer.toString()
                )

                SwalAlert({
                    icon: "success",
                    title: "Data byla uložena!",
                })
            } catch (_) {
                SwalAlert({
                    icon: "error",
                    title: "Nepovedlo se uložit data :(",
                })
            }

            items = localStorage.getItem("items")

            if (items) {
                loadData(true)
                silenced = true
                selectedEntry = items
            }
        } else {
            try {
                localStorage.setItem(
                    "item" + selectedEntry.toString(),
                    JSON.stringify({ ...obj.data, tachometer: undefined })
                )

                sessionStorage.setItem(
                    "item" + selectedEntry.toString(),
                    obj.data.tachometer.toString()
                )

                SwalAlert({
                    icon: "success",
                    title: "Data byla uložena!",
                })
            } catch (_) {
                SwalAlert({
                    icon: "error",
                    title: "Nepovedlo se uložit data :(",
                })
            }
        }
    }

    const loadData = (silent = false) => {
        const items = localStorage.getItem("items")

        if (!items) {
            return
        }

        const itemsNumber = parseInt(items)

        entries = []

        for (let i = 0; i <= itemsNumber; i++) {
            const item = localStorage.getItem("item" + i.toString())

            if (item) {
                let data = JSON.parse(item)
                data.tachometer = parseInt(
                    sessionStorage.getItem("item" + i.toString()) || "0"
                )
                entries = [...entries, data]
            }
        }

        if (!silent || entries.length > 0) {
            selectedEntry = items
        }
    }

    onMount(loadData)

    const deleteData = async () => {
        if (!selectedEntry) {
            SwalAlert({
                icon: "error",
                title: "Není vybrán žádný záznam!",
            })
            return
        }

        const result = await SwalAlert({
            toast: false,
            timer: 0,
            showCancelButton: true,
            cancelButtonText: "Zrušit",
            showConfirmButton: true,
            confirmButtonText: "Smazat",
            position: "center",
            title: "Opravdu chcete smazat tuto položku?",
        })

        if (!result.isConfirmed) return

        entries = entries.filter((_, i) => i != parseInt(selectedEntry!))

        //resave all entries

        const items = localStorage.getItem("items")

        if (!items) return

        const itemsNumber = parseInt(items)

        for (let i = 0; i <= itemsNumber; i++) {
            localStorage.removeItem("item" + i.toString())
            sessionStorage.removeItem("item" + i.toString())
        }
        try {
            if (entries.length > 0) {
                localStorage.setItem("items", (entries.length - 1).toString())
            } else {
                localStorage.removeItem("items")
            }

            for (let i = 0; i < entries.length; i++) {
                localStorage.setItem(
                    "item" + i.toString(),
                    JSON.stringify({ ...entries[i], tachometer: undefined })
                )
                sessionStorage.setItem(
                    "item" + i.toString(),
                    entries[i].tachometer.toString()
                )
            }

            SwalAlert({
                icon: "success",
                title: "Položka byla úspěšně smazána!",
            })

            if (entries.length == 0) {
                newData()
            } else {
                selectedEntry = (entries.length - 1).toString()
                selectItem((entries.length - 1).toString())
            }
        } catch (_) {
            SwalAlert({
                icon: "error",
                title: "Nepovedlo se smazat položku :(",
            })
        }
    }

    const newData = () => {
        selectedEntry = undefined
        date = getISO(new Date())
        price = 0
        type = ""
        gasStation = ""
        volume = 0
        tachometer = 0
    }

    const selectItem = (index: string | undefined) => {
        if (!index) return

        const data = entries[parseInt(index)]

        if (!data) return

        date = data.date
        price = data.price
        type = data.type
        gasStation = data.gasStation
        volume = data.volume
        tachometer = data.tachometer

        if (!silenced) {
            SwalAlert({
                icon: "success",
                title: "Data byla načtena!",
            })
        } else {
            silenced = false
        }
    }

    $: selectItem(selectedEntry)
</script>

<h2>Historie tankování</h2>

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
<IonButton on:click={saveData} color="success">Uložit</IonButton>
