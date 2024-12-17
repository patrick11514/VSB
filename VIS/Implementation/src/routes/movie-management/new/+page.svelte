<script lang="ts">
    import Button from '$/components/Button.svelte';
    import FormItem from '$/components/FormItem.svelte';
    import Icon from '$/components/Icon.svelte';
    import Input from '$/components/Input.svelte';
    import Select from '$/components/Select.svelte';
    import { SwalAlert } from '$/lib/functions';
    import { ActorService } from '$/lib/service/actorService';
    import { MovieService } from '$/lib/service/movieService';
    import type { DePromise } from '$/types/types';
    import { browser } from '$app/environment';
    import { goto } from '$app/navigation';

    type Data = Record<
        'name' | 'studio' | 'year',
        {
            value: string;
            error?: string;
        }
    > &
        Record<
            'actors',
            {
                value: number[];
                error?: string;
            }
        >;

    const data = $state<Data>(
        Object.fromEntries(
            ['name', 'studio', 'year', 'actors'].map((item) => {
                return [item, { value: item === 'actors' ? [] : '', error: undefined }];
            })
        ) as unknown as Data //just do as unknown xD
    );

    const service = new ActorService();

    let actorList = $state<DePromise<ReturnType<typeof service.getActors>>>([]);
    let currentActor = $state('');

    $effect(() => {
        if (currentActor != '') {
            const id = parseInt(currentActor);
            data.actors.value.push(id);

            currentActor = ''; // and reset
        }
    });

    const load = async () => {
        actorList = await service.getActors();
    };

    if (browser) load();

    const add = async () => {
        Object.values(data).map((item) => (item.error = undefined)); // reset

        if (!data.name.value) {
            data.name.error = 'Zadej prosím název filmu';
        }

        if (!data.studio.value) {
            data.studio.error = 'Zadej prosím název studia';
        }
        if (parseInt(data.year.value) <= 1900 || parseInt(data.year.value) >= 3000) {
            data.year.error = 'Zadej prosím platný rok';
        }

        if (data.actors.value.length == 0) {
            data.actors.error = 'Vyber herce ve filmu';
        }

        if (Object.values(data).some((item) => item.error)) {
            return;
        }

        const service = new MovieService();
        const response = await service.addMovie(data.name.value, data.studio.value, parseInt(data.year.value), data.actors.value);
        if (!response.status) {
            SwalAlert({
                icon: 'error',
                title: response.message
            });
            return;
        }

        SwalAlert({
            icon: 'success',
            title: 'Film byl úspěšně přidán'
        });

        goto('/movie-management');
    };
</script>

<svelte:head>
    <title>Nový film | FilmDB</title>
</svelte:head>

<section class="m-auto flex max-w-[90%] flex-col items-center justify-center rounded-md border-2 border-black p-4 text-center md:max-w-[70%] lg:max-w-[50%]">
    <h1 class="mb-2 w-max border-b-2 border-b-black font-poppins text-2xl font-bold lg:text-3xl">Nový Film</h1>
    <FormItem id="name" label="Název filmu" error={data.name.error}>
        <Input id="name" bind:value={data.name.value} bind:invalid={data.name.error} />
    </FormItem>
    <FormItem id="studio" label="Studio" error={data.studio.error}>
        <Input id="studio" bind:value={data.studio.value} bind:invalid={data.studio.error} />
    </FormItem>
    <FormItem id="year" label="Rok vydání" error={data.year.error}>
        <Input id="year" type="number" min="1900" max="3000" bind:value={data.year.value} bind:invalid={data.year.error} />
    </FormItem>
    <FormItem id="actors" label="Herci" error={data.actors.error}>
        <div class="flex flex-row flex-wrap gap-2 p-1">
            {#each data.actors.value as actor}
                {@const actorData = actorList.find((a) => a.id === actor)!}
                <div class="rounded-md border-2 border-black p-1">
                    {actorData.firstName}
                    {actorData.lastName}
                    <Icon onclick={() => (data.actors.value = data.actors.value.filter((a) => a !== actor))} name="bi-trash-fill" class="text-xl text-red-500" />
                </div>
            {/each}
        </div>
        <Select id="actors" bind:value={currentActor} bind:invalid={data.actors.error}>
            <option value="" selected disabled>Vyber autora pro přidání</option>
            {#each actorList.filter((a) => !data.actors.value.includes(a.id)) as actor}
                <option value={actor.id}>{actor.firstName} {actor.lastName}</option>
            {/each}
        </Select>
    </FormItem>
    <Button onclick={add}>Přidat Film</Button>
</section>
