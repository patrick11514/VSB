<script lang="ts">
    export let header: string
    export let isOpen = false
    let actionSheet: HTMLIonActionSheetElement
    //$: actionSheet?.addEventListener("ionActionSheetDidDismiss", () => {
    //    console.log("loil")
    //    isOpen = false
    //})

    $: actionSheet?.addEventListener("ionActionSheetWillDismiss", () => {
        isOpen = false
    })
</script>

<ion-action-sheet bind:this={actionSheet} is-open={isOpen} {header}
    ><slot /></ion-action-sheet
>
