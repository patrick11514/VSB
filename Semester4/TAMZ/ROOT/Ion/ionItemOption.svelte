<script lang="ts">
    import type { Color } from "@ionic/core"

    export let color: Color = "primary"

    export let expandable = false
</script>

<!-- svelte-ignore a11y-click-events-have-key-events -->
<!-- svelte-ignore a11y-no-static-element-interactions -->
<ion-item-option on:click {color} {expandable}><slot /></ion-item-option>
