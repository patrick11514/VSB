<script lang="ts">
    export let side: "start" | "end" | undefined = undefined
    export let onSwipe: undefined | (() => void) = undefined

    let element: HTMLIonItemOptionsElement
    $: element?.addEventListener("ionSwipe", (ev) => {
        if (onSwipe) {
            onSwipe()
        }
    })
</script>

<ion-item-options bind:this={element} {side}><slot /></ion-item-options>
