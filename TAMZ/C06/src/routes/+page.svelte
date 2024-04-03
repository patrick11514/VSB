<script lang="ts">
    import { z } from "zod"
    import { SearchBar } from "../components/Ion"
    import IonInput from "../components/Ion/ionInput.svelte"
    import { lang, mode } from "../components/store.svelte"

    const BASE_URL = "http://linedu.vsb.cz/~mor03/TAMZ/cnb_json.php"

    const getDate = (date: Date) => {
        const day = date.getDate() > 9 ? date.getDate() : `0${date.getDate()}`
        const month =
            date.getMonth() + 1 > 9
                ? date.getMonth() + 1
                : `0${date.getMonth() + 1}`
        const year = date.getFullYear()

        return `${year}-${month}-${day}`
    }

    let sStream = false
    let date: string = getDate(new Date())
    let searchTerm = ""

    const currencySchema = z.object({
        country_label: z.string(),
        curr_label: z.string(),
        unit: z.coerce.number(),
        code: z.string(),
        rate: z.coerce.number(),
    })

    const schema = z.object({
        date: z.string(),
        order: z.string(),
        data: z.array(currencySchema),
    })

    let data: z.infer<typeof schema> | null = null
    let selected: z.infer<typeof currencySchema> | null = null
    let currency = "CUR"

    const getEventSourceData = async (params: URLSearchParams) => {
        return new Promise<object>((resolve, reject) => {
            const source = new EventSource(BASE_URL + `?${params.toString()}`)
            source.onmessage = (event) => {
                const json = JSON.parse(event.data)
                resolve(json)
            }
            source.onerror = () => {
                reject()
            }
        })
    }

    const getData = async () => {
        const params = new URLSearchParams()
        params.append("lang", $lang)

        if (date != undefined) {
            params.append("date", date)
        }

        let json
        if ($mode == "json") {
            const request = await fetch(BASE_URL + `?${params.toString()}`)
            json = await request.json()
        } else {
            params.append("sse", "y")
            json = await getEventSourceData(params)
        }

        const parsed = schema.safeParse(json)
        if (parsed.success) {
            data = parsed.data
        }
    }

    lang.subscribe(() => {
        getData()
    })

    mode.subscribe(() => {
        getData()
    })

    const updateItem = (item: z.infer<typeof currencySchema> | null) => {
        if (item == null) {
            currency = "CUR"
            return
        }

        currency = item.code
    }

    $: updateItem(selected)

    const applySearch = (
        list: z.infer<typeof currencySchema>[],
        search: string
    ) => {
        if (list == null) {
            return []
        }

        if (search == "") {
            return list
        }

        return list.filter((item) => {
            return (
                item.code
                    .toLocaleLowerCase()
                    .includes(search.toLocaleLowerCase()) ||
                item.country_label
                    .toLocaleLowerCase()
                    .includes(search.toLocaleLowerCase()) ||
                item.curr_label
                    .toLocaleLowerCase()
                    .includes(search.toLocaleLowerCase())
            )
        })
    }

    const selectItem = (item: z.infer<typeof currencySchema>) => () => {
        selected = item

        updateCZK(czk)
    }

    let czk = "0"
    let cur = "0"

    const updateCZK = (value: number | string | null | undefined) => {
        if (typeof value === "object" || typeof value === "undefined") {
            return
        }

        if (typeof value === "number") {
            value = value.toString()
        }

        if (selected == null) {
            cur = value
            return
        }

        cur = ((parseFloat(value) / selected.rate) * selected.unit).toFixed(2)
    }

    const updateCUR = (value: number | string | null | undefined) => {
        if (typeof value === "object" || typeof value === "undefined") {
            return
        }

        if (typeof value === "number") {
            value = value.toString()
        }

        if (selected == null) {
            czk = value
            return
        }

        czk = ((parseFloat(value) * selected.rate) / selected.unit).toFixed(2)
    }

    const updateDate = async (date: string) => {
        await getData()

        //update selected

        selected = data?.data.find((item) => item.code === currency) ?? null

        updateCZK(czk)
    }

    $: updateDate(date)
</script>

<ion-item>
    <IonInput
        bind:value={czk}
        onChange={updateCZK}
        label="CZK: "
        type="number"
    />
</ion-item>
<ion-item>
    <IonInput
        bind:value={cur}
        onChange={updateCUR}
        label="{currency}: "
        type="number"
    />
</ion-item>
<SearchBar
    placeholder={$lang === "cs" ? "Vyhledat měnu" : "Search currency"}
    bind:value={searchTerm}
/>

<ion-content>
    {#if data != null}
        <ion-list inset={true}>
            {#each applySearch(data.data, searchTerm) as item}
                <ion-item
                    style="cursor:pointer;"
                    on:click={selectItem(item)}
                    on:keypress={selectItem(item)}
                    role="button"
                    tabindex="-1"
                >
                    <ion-label>
                        {item.code} - {item.country_label} ({item.curr_label})
                    </ion-label>
                </ion-item>
            {/each}
        </ion-list>
    {/if}
</ion-content>

<ion-item>
    <IonInput
        label={$lang === "cs" ? "Vyber datum" : "Select date"}
        type="date"
        bind:value={date}
    />
</ion-item>
