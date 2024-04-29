<script lang="ts">
    import type { BootstrapIcon } from '$/types/bootstrap_icons';
    let cls = '';
    export { cls as class };
    export let name: BootstrapIcon;
</script>

<i class="bi {name} {cls}"><slot /></i>
