<script lang="ts">
    export let value = ""
    let cls = ""
    export { cls as class }

    let element: HTMLIonRadioGroupElement

    $: element?.addEventListener("ionChange", (ev) => {
        value = (ev.currentTarget as HTMLIonRadioGroupElement).value
    })
</script>

<ion-radio-group class={cls} bind:this={element} {value}>
    <slot />
</ion-radio-group>
