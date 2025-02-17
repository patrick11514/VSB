<script lang="ts" context="module">
    import { writable } from 'svelte/store';
    import type { LocationData } from '../types/types';

    export const currentLocation = writable<LocationData>();
</script>
