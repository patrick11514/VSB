<script lang="ts">
    import type { Color } from '$/types/types';

    export let background: Color | undefined = undefined;

    let cls = '';
    export { cls as class };

    export let style = '';
</script>

<div class="col {cls} {background ? `bg-${background}` : ''}" {style}>
    <slot />
</div>
