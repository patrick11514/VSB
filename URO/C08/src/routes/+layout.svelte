<script lang="ts">
    import 'bootstrap/dist/css/bootstrap.min.css';
    import 'bootstrap/dist/js/bootstrap.bundle';
    import './main.css';

    import { API } from '$lib/api';
    import type { LayoutData } from './$types';

    export let data: LayoutData;

    API.hydrateFromServer(data.api);
</script>

<slot />
