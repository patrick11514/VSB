<script lang="ts">
    import { page } from '$app/state';
</script>

<section class="m-auto items-center justify-center rounded-md border-2 border-black p-4 text-center">
    <h1 class="font-poppins mb-2 w-max border-b-2 border-b-black text-2xl font-bold lg:text-3xl">Něco se nepovedlo</h1>
    <h2 class="text-xl font-bold lg:text-2xl">Nastala chyba {page.status}</h2>
    <h3>Popis: {page.error?.message ?? 'Neznámá chyba'}</h3>
</section>
