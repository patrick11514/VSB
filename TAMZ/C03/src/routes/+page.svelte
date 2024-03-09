<script lang="ts">
    import { SwalAlert } from "$lib"
    import { z } from "zod"
    import { IonButton } from "../components/Ion"
    import IonDateTime from "../components/Ion/ionDateTime.svelte"
    import IonInput from "../components/Ion/ionInput.svelte"
    import RadioGroup from "../components/Ion/radioGroup.svelte"

    let date: undefined | string = undefined
    let price = 0
    let type = ""
    let gasStation = ""
    let volume = 0
    let tachometer = 0

    const schema = z.object({
        date: z.string().datetime(),
        price: z.number().min(1),
        type: z.string().min(1),
        gasStation: z.string().min(1),
        volume: z.number().min(1),
        tachometer: z.number().min(1),
    })

    const saveData = () => {
        const obj = schema.safeParse({
            date,
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

        for (const key in obj) {
            localStorage[key] = obj[key as keyof typeof obj]
        }
    }

    const loadData = () => {}
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
        <ion-radio value="grapes">BA 95</ion-radio>
        <ion-radio value="strawberries">BA 100</ion-radio>
    </div>
    <div class="group-row between">
        <ion-radio value="pineapple">Nafta</ion-radio>
        <ion-radio value="cherries">Arktická nafta</ion-radio>
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

<IonButton on:click={saveData} color="success">Uložit</IonButton>
<IonButton on:click={loadData} color="secondary">Načíst</IonButton>
