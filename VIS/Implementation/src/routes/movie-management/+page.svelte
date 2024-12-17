<script lang="ts">
    import Button from '$/components/Button.svelte';
    import Title from '$/components/Title.svelte';
    import { MovieDetailedService } from '$/lib/service/movieDetailedService';
    import type { DePromise } from '$/types/types';
    import { browser } from '$app/environment';
    import { goto } from '$app/navigation';
    import type { Snippet } from 'svelte';

    const service = new MovieDetailedService();
    const loading = Symbol('loading');
    let movies = $state<DePromise<ReturnType<typeof service.getMovies>> | typeof loading>(loading);

    const getData = async () => {
        movies = await service.getMovies();
    };
    if (browser) getData();
</script>

{#snippet Info(text: string)}
    <h1 class="m-auto font-poppins text-2xl font-bold lg:text-3xl">{text}</h1>
{/snippet}

{#snippet keyValue(key: string, value: string)}
    <div class="flex flex-col justify-between">
        <h2 class="font-poppins font-bold">{key}</h2>
        <h3 class="text-base lg:text-lg">{value}</h3>
    </div>
{/snippet}

<div class="flex w-full flex-1 flex-col gap-4 p-4">
    <div class="flex w-full items-center justify-between">
        <Title>Správa Filmů</Title>
        <Button onclick={() => goto('/movie-management/new')}>Přidat</Button>
    </div>
    <div class="flex flex-1 flex-col border-2 border-black p-2">
        {#if movies === loading}
            {@render Info('Načítání...')}
        {:else if movies === undefined}
            {@render Info('Nepovedlo se načíst seznam')}
        {:else if movies === null}
            {@render Info('Žádný film nebyl nalezen')}
        {:else}
            {#each movies.toReversed() as movie}
                <div class="flex items-center justify-between gap-2 border-2 border-black p-2 text-center">
                    <div class="w-16 self-stretch border-2 border-black">
                        <!-- IMAGE !-->
                    </div>

                    <div class="flex flex-col justify-between">
                        <h2 class="font-poppins font-bold">{movie.name}</h2>
                        <h3 class="text-base lg:text-lg">{service.getAvgRating(movie).toFixed(1)}/5.0</h3>
                        <h3 class="text-base lg:text-lg">Drama,...</h3>
                    </div>

                    {@render keyValue('Vydáno', movie.year.toString())}
                    {@render keyValue('Recenzí', movie.reviews.length.toString())}
                    {@render keyValue('Hodnocení', movie.ratings.length.toString())}

                    <Button class="text-base lg:text-lg">Editovat</Button>
                </div>
            {/each}
        {/if}
    </div>
</div>
