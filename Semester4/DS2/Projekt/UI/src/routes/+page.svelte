<script lang="ts">
    import Button from '$/components/Button.svelte';
    import Icon from '$/components/Icon.svelte';
    import Input from '$/components/Input.svelte';
    import Select from '$/components/Select.svelte';
    import { twMerge } from 'tailwind-merge';
</script>

<form class="font-ubuntu m-8 w-max max-w-[620px] border-2 border-black bg-slate-300 p-2" action="">
    <h2 class="text-xl font-medium">Nastavení oprávnění uživatele/ů</h2>
    <div>
        <div class="grid grid-cols-2 grid-rows-2 items-end gap-2">
            <Input class="col-start-1 row-span-2 row-start-1" title="Uživatel" value="Username..." />
            <Button class="col-start-2 row-start-2">Přidat</Button>
        </div>
        <div class="relative h-[100px] w-1/2 bg-white p-2">
            {#each ['Pepa', 'Karel', 'XxxHonza_PVP_CZYT'] as name}
                <h2 class="cursor-pointer text-lg font-medium hover:bg-gray-200">{name}</h2>
            {/each}
        </div>
        <div class="my-2 flex h-[120px] w-full flex-row flex-wrap gap-2 bg-white p-2">
            {#each ['patrick115', 'Skypad6000'] as permission}
                <div class="h-max rounded-lg bg-gray-400 p-2 text-white">{permission} <Icon class="cursor-pointer" name="bi-trash" /></div>
            {/each}
        </div>
    </div>

    <div>
        <div class="grid grid-cols-2 grid-rows-2 items-end gap-2">
            <Select class="col-start-1 row-span-2 row-start-1" title="Výběr serveru" value="">
                <option value="" disabled selected>Vyber server</option>
                <option value="global">Globálně</option>
                <option value="survial">Survival</option>
                <option value="skyblock">Skyblock</option>
            </Select>
        </div>
    </div>
    <div>
        <div class="grid grid-cols-2 grid-rows-2 items-end gap-2">
            <Input class="col-start-1 row-span-2 row-start-1" title="Skupina" value="SuperVIP..." />
            <Button class="col-start-2 row-start-2">Přidat</Button>
        </div>
        <div class="relative h-[100px] w-1/2 bg-white p-2">
            {#each ['Majitel', 'Technik', 'Admin'] as name}
                <h2 class="cursor-pointer text-lg font-medium hover:bg-gray-200">{name}</h2>
            {/each}
        </div>

        <div class="my-2 flex h-[120px] w-full flex-row flex-wrap gap-2 bg-white p-2">
            {#each [{ name: 'SupremeVip', color: 'bg-red-500' }, { name: 'Builder', color: 'bg-blue-500' }, { name: 'Hráč', color: 'bg-green-500' }] as group}
                <div class={twMerge('h-max rounded-lg p-2 text-white', group.color)}>{group.name} <Icon class="cursor-pointer" name="bi-trash" /></div>
            {/each}
        </div>
    </div>
    <div>
        <div class="grid grid-cols-2 grid-rows-2 items-end gap-2">
            <Input class="col-start-1 row-span-2 row-start-1" title="Dodatečné oprávnění" value="plugin.permisse" />
            <Button class="col-start-2 row-start-2">Přidat</Button>
        </div>
        <div class="my-2 flex h-[120px] w-full flex-row flex-wrap gap-2 bg-white p-2">
            {#each ['essentials.home.4', '-essentials.heal', 'luckperms.permission.add', '-worldedit.brush'] as permission}
                <div class="h-max rounded-lg bg-gray-400 p-2 text-white">{permission} <Icon class="cursor-pointer" name="bi-trash" /></div>
            {/each}
        </div>
    </div>

    <Button class="mt-4">Potvrdit</Button>
</form>
