<script lang="ts">
    export let label = ""
    export let placeholder = ""
    export let value: string | undefined | number = ""

    let element: HTMLIonSelectElement

    $: element?.addEventListener("ionChange", (ev) => {
        value = (ev.currentTarget as HTMLIonSelectElement).value
    })
</script>

<ion-select {label} {placeholder} bind:this={element} {value}>
    <slot />
</ion-select>
