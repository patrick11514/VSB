<script lang="ts" context="module">
    import { writable } from "svelte/store"

    export let lang = writable<"cs" | "en">("cs")
    export let mode = writable<"json" | "events">("json")
</script>
