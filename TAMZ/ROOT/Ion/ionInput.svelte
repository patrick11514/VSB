<script lang="ts">
    type InputType = 'number' | 'search' | 'text' | 'tel' | 'url' | 'email' | 'date' | 'time' | 'datetime-local' | 'month' | 'password' | 'week';

    export let value: string | number | null | undefined = undefined;
    export let type: InputType = 'text';
    export let placeholder = '';

    let element: HTMLIonInputElement;

    $: element?.addEventListener('ionInput', (ev) => {
        value = (ev.currentTarget as HTMLIonInputElement).value;
    });
</script>

<ion-input {type} {placeholder} bind:this={element} {value} />
