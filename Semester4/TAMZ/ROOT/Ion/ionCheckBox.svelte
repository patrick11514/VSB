<script lang="ts">
    export let labelPlacement: 'end' | 'fixed' | 'start' = 'end';
    export let checked = false;

    let element: HTMLIonCheckboxElement;

    $: element?.addEventListener('ionChange', (ev) => {
        const target = ev.currentTarget as HTMLIonCheckboxElement;
        checked = target.checked;
    });
</script>

<ion-checkbox bind:this={element} label-placement={labelPlacement} {checked}><slot /></ion-checkbox>
