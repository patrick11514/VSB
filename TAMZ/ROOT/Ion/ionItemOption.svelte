<script lang="ts">
    export let color:
        | "primary"
        | "secondary"
        | "tertiary"
        | "success"
        | "warning"
        | "danger"
        | "light"
        | "medium"
        | "dark"
        | undefined = undefined

    export let expandable = false
</script>

<!-- svelte-ignore a11y-click-events-have-key-events -->
<!-- svelte-ignore a11y-no-static-element-interactions -->
<ion-item-option on:click {color} {expandable}><slot /></ion-item-option>
