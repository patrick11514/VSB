<script lang="ts">
    export let placeholder = ""
    export let disabled = false
    export let value: string | null | undefined = ""

    let searchbar: HTMLIonSearchbarElement

    $: searchbar?.addEventListener("ionInput", (e) => {
        value = (e.target as HTMLIonSearchbarElement).value
    })
</script>

<ion-searchbar bind:this={searchbar} {placeholder} {disabled} {value} />
