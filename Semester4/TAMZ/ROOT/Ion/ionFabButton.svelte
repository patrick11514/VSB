<script lang="ts">
    export let disabled = false
    export let activated = false
</script>

<!-- svelte-ignore a11y-click-events-have-key-events -->
<!-- svelte-ignore a11y-no-static-element-interactions -->
<ion-fab-button on:click {disabled} {activated}><slot /></ion-fab-button>
