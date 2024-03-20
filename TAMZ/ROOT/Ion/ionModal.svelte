<script lang="ts">
    export let opened = false

    let modal: HTMLIonModalElement
    $: modal?.addEventListener("ionModalWillDismiss", () => {
        opened = false
    })
</script>

<ion-modal bind:this={modal} is-open={opened}>
    <slot />
</ion-modal>
