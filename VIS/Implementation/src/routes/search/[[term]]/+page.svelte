<script lang="ts">
    import Title from '$/components/Title.svelte';
    import { MovieService } from '$/lib/service/movieService';
    import { page } from '$app/state';

    let searchTerm = $state(page.params.term);

    $effect(() => {
        if (searchTerm != page.params.term) {
            searchTerm = page.params.term;
        }
    });

    const service = new MovieService();

    let promise = $derived<ReturnType<typeof service.getMovies>>(service.getMovies(searchTerm));
</script>

<svelte:head>
    <title>Vyhledávání: {searchTerm} | FilmDB</title>
</svelte:head>

{#snippet Info(text: string)}
    <h1 class="m-auto font-poppins text-2xl font-bold lg:text-3xl">{text}</h1>
{/snippet}

<div class="flex flex-1 flex-col gap-2 p-4">
    <Title>Vyhledávání</Title>
    <h2>pro dotaz: <span class="font-bold">{searchTerm}</span></h2>
    {#if promise}
        {#await promise}
            {@render Info('Načítání...')}
        {:then result}
            {#if result === undefined}
                {@render Info('Při hledání nastala chyba, zkus to prosím později')}
            {:else if result === null}
                {@render Info('Zadanému výrazu neodpovídá žádný film')}
            {:else}
                {JSON.stringify(result)}
            {/if}
        {/await}
    {/if}
</div>
