<script lang="ts">
    import { lang } from "./store.svelte"
</script>

<ion-header>
    <ion-toolbar>
        <ion-title>
            {$lang === "cs" ? "Převod měn" : "Currency exchange"}
        </ion-title>
    </ion-toolbar>
</ion-header>
