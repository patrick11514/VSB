<script lang="ts">
    import { setupIonicBase } from 'ionic-svelte';

    import Footer from '$/components/footer.svelte';
    import Header from '$/components/header.svelte';
    /* Theme variables */
    import '../app.css';

    /* load and register all components - you can also import separately to code split */
    import { userData } from '$/components/APP/store.svelte';
    import { API } from '$/lib/api';
    import 'ionic-svelte/components/all';
    import type { LayoutServerData } from './$types';

    /* run base configuration code from ionic/core */
    setupIonicBase();

    export let data: LayoutServerData;

    userData.set(data.userData);
    API.hydrateFromServer(data.hydrationData);
</script>

<ion-app>
    <Header />
    <slot />
    <Footer />
</ion-app>
