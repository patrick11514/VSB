<script lang="ts">
    import type { Color } from '$/types/types';

    export let type: 'button' | 'submit' | 'reset' = 'button';
    export let color: Color;

    let cls = '';
    export { cls as class };

    export let style = '';
</script>

<button {style} {type} class="fw-bold btn btn-{color} {cls}" on:click><slot /></button>
