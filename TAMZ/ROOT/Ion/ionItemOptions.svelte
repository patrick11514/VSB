<script lang="ts">
    export let side: 'start' | 'end' | undefined = undefined;
    export let onSwipe: undefined | ((element: HTMLIonItemOptionsElement) => void) = undefined;

    let element: HTMLIonItemOptionsElement;
    $: element?.addEventListener('ionSwipe', (ev) => {
        if (onSwipe) {
            onSwipe(ev.currentTarget as HTMLIonItemOptionsElement);
        }
    });
</script>

<ion-item-options bind:this={element} {side}><slot /></ion-item-options>
