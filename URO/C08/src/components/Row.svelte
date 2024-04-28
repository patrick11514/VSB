<script lang="ts">
    let cls = '';
    export { cls as class };
    export let wrap = false;
</script>

<div class="row {cls}" class:flex-wrap={wrap}>
    <slot />
</div>
