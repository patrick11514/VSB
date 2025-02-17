<script lang="ts">
    import type { BootstrapIcon } from '$/types/bootstrap_icons';
    import type { SvelteHTMLElements } from 'svelte/elements';

    const { children, class: cls = '', name, ...rest }: SvelteHTMLElements['i'] & { class?: string; name: BootstrapIcon } = $props();
</script>

<i {...rest} role="button" tabindex="-1" class="bi {name} {cls}">
    {@render children?.()}
</i>
