<script lang="ts" context="module">
    import type { UserData } from '$/types/types';
    import { writable } from 'svelte/store';

    export const userData = writable<UserData>();
    export const title = writable<string>('Správa bankovních účtů');
</script>
