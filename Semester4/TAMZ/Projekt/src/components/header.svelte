<script lang="ts">
    import { goto } from '$app/navigation';
    import { page } from '$app/stores';
    import { home } from 'ionicons/icons';
    import { title } from './APP/store.svelte';
    import IonButton from './Ion/ionButton.svelte';

    let rootPage: boolean;
    $: rootPage = $page.url.pathname === '/';
    $: if (rootPage) {
        title.set('Správa bankovních účtů');
    }
</script>

<ion-header>
    <ion-toolbar>
        {#if !rootPage}
            <ion-buttons slot="start">
                <IonButton on:click={() => goto('/')}>
                    <ion-icon icon={home} />
                </IonButton>
            </ion-buttons>
        {/if}
        <ion-title>{$title}</ion-title>
    </ion-toolbar>
</ion-header>
