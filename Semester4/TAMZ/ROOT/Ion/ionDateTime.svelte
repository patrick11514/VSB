<script lang="ts">
    import { getISO } from '$lib';

    export let id = '';
    export let hourCycle: 'h12' | 'h23' = 'h12';
    export let value: string | string[] | undefined | null = getISO(new Date());
    export let presentation: 'date' | 'date-time' | 'month' | 'month-year' | 'time' | 'time-date' | 'year' = 'date-time';

    let element: HTMLIonDatetimeElement;

    $: element?.addEventListener('ionChange', (ev) => {
        value = (ev.currentTarget as HTMLIonDatetimeElement).value;
    });
</script>

<ion-datetime bind:this={element} {value} {id} {presentation} hour-cycle={hourCycle} />
