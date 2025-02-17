<script lang="ts">
    export let value: string | undefined = '';

    let element: HTMLIonSegmentElement;
    $: element?.addEventListener('ionChange', (ev) => {
        const target = ev.target as HTMLIonSegmentElement;
        value = target.value as string | undefined;
    });
</script>

<ion-segment bind:this={element} {value}>
    <slot />
</ion-segment>
