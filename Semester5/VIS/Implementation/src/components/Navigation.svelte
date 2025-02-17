<script lang="ts">
    import Icon from '$/components/Icon.svelte';
    import type { UserState } from '$/types/types';
    import { page } from '$app/state';
    import { getContext } from 'svelte';
    import Input from './Input.svelte';
    import type { Writable } from 'svelte/store';
    import { goto } from '$app/navigation';

    const pages: {
        name: string;
        title: string;
        path: string;
        show: boolean;
    }[] = [
        {
            name: 'Domů',
            path: '/',
            title: 'Domů',
            show: false
        },
        {
            name: 'Populární',
            path: '/popular',
            title: 'Populární',
            show: true
        },
        {
            name: 'Nové',
            path: '/new',
            title: 'Nové',
            show: true
        },
        {
            name: 'Registrace',
            path: '/register',
            title: 'Registrace',
            show: false
        },
        {
            name: 'Přihlášení',
            path: '/login',
            title: 'Přihlášení',
            show: false
        },
        {
            name: 'Movie Management',
            path: '/movie-management',
            title: 'Správa Filmů',
            show: false
        }
    ];

    const userState = getContext<Writable<UserState>>('userState');

    let invalid = $state<string | undefined>(undefined);
    let search = $state('');

    const doSearch = () => {
        if (!search) {
            invalid = 'Enter something to search';
            return;
        }
        invalid = undefined;
        goto(`/search/${search}`);
    };
</script>

<svelte:head>
    <title>{pages.find((p) => p.path === page.url.pathname)?.title ?? ''} | FilmDB</title>
</svelte:head>

<nav class="flex w-full flex-row items-center gap-4 rounded-md border-2 border-black p-4">
    <a href="/" class="mr-8 font-poppins text-4xl font-bold lg:text-5xl">FilmDB</a>
    {#each pages.filter((p) => p.show) as page}
        <a class="text-xl font-bold lg:text-2xl" href={page.path}>{page.name}</a>
    {/each}
    <Input bind:value={search} {invalid} type="search" placeholder="Zadej jméno filmu" class="ml-auto text-center" />
    <Icon onclick={doSearch} name="bi-search" class="mr-auto rounded-md border-2 border-black px-2 py-1 transition-colors duration-200 hover:bg-black hover:bg-opacity-10" />

    {#if !$userState.logged}
        <a class="text-xl font-bold lg:text-2xl" href="/register">Registrovat se</a>
        <a class="text-xl font-bold lg:text-2xl" href="/login">Přihlásit se</a>
    {:else}
        <h2 class="text-xl font-bold lg:text-2xl">{$userState.data.username}</h2>
        <a class="text-xl font-bold lg:text-2xl" href="/movie-management">Správa filmů</a>
    {/if}
</nav>
