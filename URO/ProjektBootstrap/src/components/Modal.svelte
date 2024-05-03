<script lang="ts">
    import Portal from 'svelte-portal';
    import ClickOutside from './clickOutside.svelte';

    export let show = false;
</script>

<Portal target={document.body}>
    {#if show}
        <div class="modal-backdrop fade show"></div>
    {/if}
</Portal>

<div class="modal" tabindex="-1" style="display: {show ? 'block' : 'none'};">
    <div class="modal-dialog">
        <ClickOutside class="modal-content" clickoutside={() => (show = false)}>
            <slot />
        </ClickOutside>
    </div>
</div>
