<script lang="ts">
    import '../app.css';
    import 'bootstrap-icons/font/bootstrap-icons.min.css';

    import { API } from '$/lib/api';
    import { setContext, type Snippet } from 'svelte';
    import type { LayoutData } from './$types';
    import Navigation from '$/components/Navigation.svelte';
    import { writable } from 'svelte/store';

    let { children, data }: { children: Snippet; data: LayoutData } = $props();

    API.hydrateFromServer(data.api);
    setContext('userState', writable(data.user));
</script>

<main class="flex h-full min-h-screen w-full min-w-full flex-col overflow-x-hidden bg-background p-4 font-roboto text-lg text-text lg:text-xl">
    <Navigation />
    <section class="flex h-full w-full flex-1 flex-col">
        {@render children()}
    </section>
</main>
