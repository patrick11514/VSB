<script lang="ts">
    import type { Color } from "@ionic/core"

    export let color: Color = "primary"
    export let strong = false
    export let disabled = false
</script>

<!-- svelte-ignore a11y-click-events-have-key-events -->
<!-- svelte-ignore a11y-no-static-element-interactions -->
<ion-button on:click {color} {strong} {disabled}><slot /></ion-button>
