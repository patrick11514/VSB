<script lang="ts">
    import { setupIonicBase } from "ionic-svelte"

    import Header from "../components/header.svelte"
    /* Theme variables */
    import "../app.css"
    /* load and register all components - you can also import separately to code split */
    import "ionic-svelte/components/all"
    import Footer from "../components/footer.svelte"

    /* run base configuration code from ionic/core */
    setupIonicBase()
</script>

<ion-app>
    <Header />
    <slot />
    <Footer />
</ion-app>
