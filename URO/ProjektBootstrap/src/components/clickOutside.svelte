<script lang="ts">
    /**
     * Source: https://svelte.dev/repl/0ace7a508bd843b798ae599940a91783?version=3.16.7
     */
    function clickOutside(node: HTMLElement) {
        const handleClick = (event: MouseEvent) => {
            if (node && !node.contains(event.target as Node | null) && !event.defaultPrevented) {
                if (ignore === undefined) {
                    clickoutside();
                    return;
                }

                if (Array.isArray(ignore)) {
                    for (const el of ignore) {
                        if (el === event.target || el.contains(event.target as Node)) {
                            return;
                        }
                    }
                    clickoutside();
                    return;
                }

                if (ignore == event.target || ignore.contains(event.target as Node)) {
                    return;
                }
                clickoutside();
            }
        };

        document.addEventListener('click', handleClick, true);

        return {
            destroy() {
                document.removeEventListener('click', handleClick, true);
            }
        };
    }

    export let clickoutside: () => void;
    let cls = '';
    export { cls as class };
    export let ignore: undefined | HTMLElement | HTMLElement[] = undefined;
</script>

<div class={cls} use:clickOutside><slot /></div>
