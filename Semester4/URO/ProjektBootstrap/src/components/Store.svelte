<script lang="ts" context="module">
    import type { Item } from '$/types/types';
    import { writable } from 'svelte/store';

    export const itemPopupData = writable<Item | undefined>(undefined);
    export const itemPopupOpen = writable<boolean>(false);
    export const itemPopupClose = writable<() => void>(() => {});
    export const itemPopupCancel = writable<() => void>(() => {});
    export const items = writable<Item[]>([]);
    export const modalAfterConfirm = writable<() => void>(() => {
        modalIsOpen.set(false);
    });
    export const modalIsOpen = writable<boolean>(false);
    export const creating = writable<boolean>(false);
    export const selected = writable<number | null>(null);
</script>
