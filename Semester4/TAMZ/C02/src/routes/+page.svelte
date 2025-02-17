<script lang="ts">
    import IonDateTime from '../components/Ion/ionDateTime.svelte';
    import IonInput from '../components/Ion/ionInput.svelte';

    let name = '';

    let date: string;
    let birthday: string | undefined = undefined;
    let had: 0 | 1 | 2 = 0;
    let difference = 0;

    const calculateBirthday = (inputDate: string) => {
        const date = new Date(inputDate);
        const now = new Date();
        date.setFullYear(now.getFullYear());

        if (now.getMonth() == date.getMonth() && now.getDate() == date.getDate()) {
            birthday = date.toLocaleDateString();
            had = 1;
        } else if (now.getTime() < date.getTime()) {
            birthday = date.toLocaleDateString();
            had = 0;
            difference = (date.getTime() - now.getTime()) / (1000 * 60 * 60 * 24);
        } else {
            date.setFullYear(now.getFullYear() + 1);
            birthday = date.toLocaleDateString();
            had = 2;
        }
    };

    $: calculateBirthday(date);
</script>

{#if name.length > 0}
    <h2>Vítej {name}!</h2>
{/if}

<IonInput type="text" bind:value={name} placeholder="Zadej jméno" />

<h1>Zadej svoje datum narození</h1>

<ion-datetime-button datetime="datetime" />

<ion-modal>
    <IonDateTime id="datetime" hourCycle="h23" presentation="date" bind:value={date} />
</ion-modal>

{#if birthday !== undefined}
    {#if had === 2}
        <h1>Tento rok jsi již měl narozeniny :(, další máš: {birthday}</h1>
    {:else if had === 0}
        <h1>Narozeniny jsi ještě neměl, budeš je mít: {birthday} což je za {Math.round(difference)} dní</h1>
    {:else}
        <h1>Všechno nejlepší {name}!! 🎂</h1>
    {/if}
{/if}
