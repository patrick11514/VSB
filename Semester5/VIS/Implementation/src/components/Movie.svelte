<script lang="ts">
    const { name, year, img }: { name: string; year: string; img: string } = $props();
</script>

<div class="flex flex-col gap-2 text-center">
    <div class="mx-auto w-max border-2 border-black">
        <img src={img} alt="movie poster" />
    </div>
    <h1 class="font-poppins text-xl font-bold lg:text-2xl">{name}</h1>
    <h2 class="font-poppins">{year}</h2>
</div>
