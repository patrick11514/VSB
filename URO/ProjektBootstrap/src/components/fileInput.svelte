<script lang="ts">
    //https://www.npmjs.com/package/@svelte-parts/drop-file
    export let onDrop: (files: Array<File | null>) => void;
    export let onEnter: (() => void) | null = null;
    export let onLeave: (() => void) | null = null;

    let isOver = false;
    let input: HTMLInputElement;

    const handleEnter = () => {
        isOver = true;
        if (onEnter) {
            onEnter();
        }
    };

    const handleLeave = () => {
        isOver = false;
        if (onLeave) {
            onLeave();
        }
    };

    const handleDrop = (
        e: DragEvent & {
            currentTarget: EventTarget & HTMLDivElement;
        }
    ) => {
        e.preventDefault();

        let items: Array<DataTransferItem> = [];
        if (e.dataTransfer?.items) {
            items = Array.from(e.dataTransfer.items);
        }
        onDrop(items.map((d) => d.getAsFile()));
        isOver = false;
    };

    const handleDragOver = (e: DragEvent) => {
        e.preventDefault();
    };

    const handleChange = (
        e: Event & {
            currentTarget: EventTarget & HTMLInputElement;
        }
    ) => {
        e.preventDefault();
        let target = e.target as HTMLInputElement;

        onDrop(target.files?.length ? Array.from(target.files) : []);
    };
</script>

<div
    on:drop={handleDrop}
    on:keydown={() => {
        return;
    }}
    on:dragover={handleDragOver}
    on:dragenter={handleEnter}
    on:dragleave={handleLeave}
    on:click={() => input.click()}
    role="button"
    tabindex="0"
>
    <slot />
</div>
<input style="display:none" type="file" on:change={handleChange} bind:this={input} multiple />
