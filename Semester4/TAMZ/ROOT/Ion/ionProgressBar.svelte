<script lang="ts">
    import type { Color } from '@ionic/core';

    export let percentage: number;
    export let color: Color = 'primary';
</script>

<ion-progress-bar value={percentage / 100} {color} />
