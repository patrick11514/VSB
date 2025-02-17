<script lang="ts">
    import { setupIonicBase } from 'ionic-svelte';

    import Footer from '../components/footer.svelte';
    import Header from '../components/header.svelte';
    /* Theme variables */
    import 'ol/ol.css';
    import '../app.css';
    /* load and register all components - you can also import separately to code split */
    import 'ionic-svelte/components/all';

    /* run base configuration code from ionic/core */
    setupIonicBase();
</script>

<ion-app>
    <Header />
    <ion-content class="ion-padding">
        <slot />
    </ion-content>
    <Footer />
</ion-app>
