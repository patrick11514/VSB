<script lang="ts">
    import type { Color } from '$/types/types';

    export let background: Color | undefined = undefined;

    let cls = '';
    export { cls as class };
</script>

<div class="container {background ? `bg-${background}` : ''} {cls}">
    <slot />
</div>
