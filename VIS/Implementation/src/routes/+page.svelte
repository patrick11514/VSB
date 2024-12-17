<script lang="ts">
    import Movie from '$/components/Movie.svelte';
    import Title from '$/components/Title.svelte';
    import { browser } from '$app/environment';

    const movieList: { name: string; year: string; img: string }[] = [
        {
            name: 'Zvonilka',
            year: '2008',
            img: '/Zvonilka.jpg'
        },
        {
            name: 'Suzume',
            year: '2022',
            img: '/Suzume.jpg'
        },
        {
            name: 'Spy × Family Code: White',
            year: '2023',
            img: '/SpyXFamily.png'
        },
        {
            name: 'Avengers',
            year: '2012',
            img: '/Avengers.jpg'
        },
        {
            name: 'Iron Man',
            year: '2008',
            img: '/IronMan.jpg'
        },
        {
            name: 'Transformers',
            year: '2007',
            img: '/Transformers.jpg'
        },
        {
            name: 'Pokémon',
            year: '1997',
            img: '/Pokemon.jpg'
        }
    ];

    const popular = Math.floor(Math.random() * 2) + 3;
    const released = Math.floor(Math.random() * 3) + 2;

    const generate = (length: number) => {
        if (!browser) return [];
        return Array.from({ length }).map((_, i) => ({ id: i, ...movieList[Math.floor(Math.random() * movieList.length)] }));
    };
</script>

<div class="my-auto flex flex-col gap-2 p-4">
    <Title>Populární</Title>
    <div class="flex w-full flex-row items-center justify-center gap-2">
        {#each generate(popular) as movie (movie.id)}
            <Movie name={movie.name} year={movie.year} img={movie.img} />
        {/each}
    </div>
    <Title>Právě vyšlo</Title>

    <div class="flex w-full flex-row items-center justify-center gap-2">
        {#each generate(released) as movie (movie.id)}
            <Movie name={movie.name} year={movie.year} img={movie.img} />
        {/each}
    </div>
</div>
