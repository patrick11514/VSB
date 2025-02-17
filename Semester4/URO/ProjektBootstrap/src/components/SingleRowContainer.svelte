<script lang="ts">
    import type { Color } from '$/types/types';

    export let color: Color;

    let cls = '';
    export { cls as class };
</script>

<div class="container-fluid w-auto m-4 {cls}">
    <div class="row">
        <div class="bg-{color} p-4">
            <slot />
        </div>
    </div>
</div>
