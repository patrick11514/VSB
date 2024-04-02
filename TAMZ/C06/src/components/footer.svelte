<script lang="ts">
    import { actionSheetController } from "ionic-svelte"
    import { document, layers } from "ionicons/icons"
    import IonButton from "./Ion/ionButton.svelte"
    import { lang, mode } from "./store.svelte"

    const showLanguageSelect = async () => {
        const options = {
            header: $lang == "cs" ? "Změnit jazyk" : "Change language",
            buttons: [
                {
                    text: "🇨🇿 Čeština",
                    handler: () => {
                        lang.set("cs")
                    },
                },
                {
                    text: "🇺🇲 English",
                    handler: () => {
                        lang.set("en")
                    },
                },
            ],
        }

        const actionSheet = await actionSheetController.create(options)
        actionSheet.present()
    }

    const showStreamSelect = async () => {
        const options = {
            header:
                $lang == "cs" ? "Změnit typ přenosu" : "Change transfer type",
            buttons: [
                {
                    text: "JSON",
                    handler: () => {
                        mode.set("json")
                    },
                },
                {
                    text: "SSE",
                    handler: () => {
                        mode.set("events")
                    },
                },
            ],
        }

        const actionSheet = await actionSheetController.create(options)
        actionSheet.present()
    }
</script>

<ion-footer>
    <ion-toolbar>
        <ion-title>
            <IonButton on:click={showLanguageSelect}>
                {$lang == "cs" ? "🇨🇿 Změnit jazyk" : "🇺🇲 Change language"}
            </IonButton>
            <IonButton on:click={showStreamSelect} color="secondary">
                {#if $mode == "json"}
                    <span>
                        <ion-icon icon={document} />
                        JSON
                    </span>
                {:else}
                    <span>
                        <ion-icon icon={layers} />
                        SSE
                    </span>
                {/if}
            </IonButton>
        </ion-title>
    </ion-toolbar>
</ion-footer>
