<script lang="ts">
    import type { Color } from '$/types/types';

    export let background: Color | undefined = undefined;

    let cls = '';
    export { cls as class };
</script>

<div class="container-fluid {background ? `bg-${background}` : ''} {cls}" style="height: 100%; min-heght:100%;">
    <slot />
</div>
