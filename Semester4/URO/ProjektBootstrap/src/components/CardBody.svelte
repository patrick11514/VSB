<script lang="ts">
    export let style = '';
    let cls = '';
    export { cls as class };
</script>

<div class="card-body {cls}" {style}>
    <slot />
</div>
