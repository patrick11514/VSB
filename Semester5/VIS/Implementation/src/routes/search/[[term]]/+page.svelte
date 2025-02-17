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
                <div class="mx-auto grid w-[90%] grid-cols-4 border-[1px] border-black text-center md:w-[80%] lg:w-[70%]">
                    {#each result.toReversed() as movie}
                        <!--<div class="flex w-full justify-around border-2 border-black">!-->
                        <a href="/movie/{movie.id}" class="border-y-[1px] border-l-[1px] border-black">{movie.name}</a>
                        <a href="/movie/{movie.id}" class="border-y-[1px] border-black">{movie.year}</a>
                        <a href="/movie/{movie.id}" class="border-y-[1px] border-black">Drama</a>
                        <a href="/movie/{movie.id}" class="border-y-[1px] border-r-[1px] border-black">{movie.rating.toFixed(1)}/5.0</a>
                        <!--</div>!-->
                    {/each}
                </div>
            {/if}
        {/await}
    {/if}
</div>
