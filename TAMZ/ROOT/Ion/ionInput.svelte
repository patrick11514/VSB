<script lang="ts">
    type InputType = 'number' | 'search' | 'text' | 'tel' | 'url' | 'email' | 'date' | 'time' | 'datetime-local' | 'month' | 'password' | 'week';

    export let value: string | number | null | undefined = undefined;
    export let type: InputType = 'text';
    export let placeholder = '';
    export let min: number | undefined = undefined;
    export let max: number | undefined = undefined;
    export let label: string | undefined = undefined;
    export let labelPlacement: undefined | 'stacked' = undefined;
    export let readonly = false;
    export let onChange: ((value: string | null | undefined | number) => void) | undefined = undefined;

    let element: HTMLIonInputElement;

    $: element?.addEventListener('ionInput', (ev) => {
        const currentValue = (ev.currentTarget as HTMLIonInputElement).value;

        if (type === 'number') {
            value = currentValue === undefined || currentValue === null ? currentValue : parseFloat(currentValue.toString());
        } else {
            value = currentValue;
        }

        if (onChange) onChange(value);
    });
</script>

<ion-input {type} {placeholder} {label} label-placement={labelPlacement} bind:this={element} {value} {min} {max} {readonly} />
