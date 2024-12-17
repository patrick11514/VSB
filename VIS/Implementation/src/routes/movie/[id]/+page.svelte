<script lang="ts">
    import Button from '$/components/Button.svelte';
    import FormItem from '$/components/FormItem.svelte';
    import Icon from '$/components/Icon.svelte';
    import Textarea from '$/components/Textarea.svelte';
    import type { MovieDetailedDTO } from '$/lib/dto/movieDetailedDTO';
    import { SwalAlert } from '$/lib/functions';
    import { MovieDetailedService } from '$/lib/service/movieDetailedService';
    import type { DePromise, UserState } from '$/types/types';
    import { browser } from '$app/environment';
    import { page } from '$app/state';
    import { getContext } from 'svelte';
    import type { Writable } from 'svelte/store';

    type DeReturn<$Type extends (...args: any) => any> = DePromise<ReturnType<$Type>>;
    type DeArray<$Type> = $Type extends (infer $Inner)[] ? $Inner : $Type;

    const service = new MovieDetailedService();
    let movie = $state<DeReturn<typeof service.getMovie>>();
    let actors = $state<DeReturn<typeof service.getActors>>();
    let ratingUsers = $state<DeReturn<typeof service.resolveUsers<DeArray<MovieDetailedDTO['ratings']>>>>();
    let reviewUsers = $state<DeReturn<typeof service.resolveUsers<DeArray<MovieDetailedDTO['reviews']>>>>();

    let bookmarked = $state(false);

    const load = async () => {
        const data = await service.getMovie(parseInt(page.params.id));
        if (!data) {
            SwalAlert({
                icon: 'error',
                title: 'Nepovedlo se načíst data pro tento film'
            });
            return;
        }

        movie = data;
        actors = await service.getActors(data);
        ratingUsers = await service.resolveUsers(data.ratings);
        reviewUsers = await service.resolveUsers(data.reviews);

        const result = await service.isBookmarked(data);
        if (result.status) {
            bookmarked = result.data;
        }
    };
    if (browser) load();

    const userState = getContext<Writable<UserState>>('userState');

    let rating = $state(false);
    let hoveringStars = $state(0);

    const rate = async () => {
        if (!$userState.logged || !!movie?.ratings.find((r) => r.user === $userState.data.id)) {
            SwalAlert({
                icon: 'error',
                title: 'Pro tuto akci se musíš přihlásit'
            });
            return;
        }

        rating = true;
    };

    const handleStar = (
        ev: MouseEvent & {
            currentTarget: EventTarget & HTMLElement;
        },
        idx: number
    ) => {
        const currentPos = ev.clientX - ev.currentTarget.getBoundingClientRect().left;
        const width = ev.currentTarget.clientWidth;
        const star = idx + (currentPos > width / 2 ? 1 : 0.5);
        if (hoveringStars != star) {
            hoveringStars = star;
        }
    };

    const star = async () => {
        if (!$userState.logged || !!movie?.ratings.find((r) => r.user === $userState.data.id)) {
            SwalAlert({
                icon: 'error',
                title: 'Pro tuto akci se musíš přihlásit'
            });
            return;
        }

        const result = await service.setRating(movie!, hoveringStars);
        if (!result.status) {
            SwalAlert({
                icon: 'error',
                title: result.message
            });
            return;
        }

        SwalAlert({
            icon: 'success',
            title: 'Úspěšně jsi hlasoval'
        });

        load();

        rating = false;
        hoveringStars = 0;
    };

    const toggleBookmark = async () => {
        if (!$userState.logged) {
            SwalAlert({
                icon: 'error',
                title: 'Pro tuto akci se musíš přihlásit'
            });
            return;
        }

        const response = await service.toggleBookmarked(movie!);
        if (!response.status) {
            SwalAlert({
                icon: 'error',
                title: response.message
            });
            return;
        }

        SwalAlert({
            icon: 'success',
            title: bookmarked ? 'Film byl odebrán z uložených' : 'Film byl přidán do uložených'
        });
        load();
    };

    let reviewing = $state(false);
    let reviewData = $state<{
        value: string;
        error?: string;
    }>({
        value: '',
        error: undefined
    });

    const review = async () => {
        reviewData.error = undefined;

        if (!reviewData.value) {
            reviewData.error = 'Vyplň prosím pole';
            return;
        }

        const result = await service.setReview(movie!, reviewData.value);
        if (!result.status) {
            SwalAlert({
                icon: 'error',
                title: result.message
            });
            return;
        }

        SwalAlert({
            icon: 'success',
            title: 'Úspěšně jsi napsal hodnocení'
        });
        load();
        reviewing = false;
    };
</script>

<svelte:head>
    <title>{movie?.name ?? ''} | FilmDB</title>
</svelte:head>

<div class="grid grid-cols-10 grid-rows-8 gap-4 p-8">
    {#if movie && actors && ratingUsers && reviewUsers}
        <div class="col-span-7 row-span-2 flex gap-2 border-2 border-black p-2">
            <div class="aspect-[6/8] h-full w-36 border-2 border-black">
                <!-- space for image !-->
            </div>
            <div class="flex flex-1 flex-col">
                <h1 class="flex w-full gap-2 font-poppins text-xl font-bold lg:text-2xl">
                    {movie.name} ({movie.year}) <Icon onclick={toggleBookmark} name={bookmarked ? 'bi-bookmark-fill' : 'bi-bookmark'} class="ml-auto" />
                </h1>
                <h3 class="mb-4">Drama, ...</h3>
                <h2 class="font-bold">Herci:</h2>
                <span>{actors.map((actor) => `${actor.firstName} ${actor.lastName}`).join(', ')}</span>
            </div>
        </div>

        <div class="col-span-2 col-start-9 row-span-4 flex flex-col items-center justify-start gap-4 border-2 border-black p-4">
            <h1 class="font-poppins text-2xl font-bold lg:text-3xl">{service.getAvgRating(movie).toFixed(1)}/5.0</h1>
            {#if !rating}
                <Button onclick={rate} class="px-1 py-0.5 text-base lg:text-lg" disabled={!$userState.logged || !!movie.ratings.find((r) => r.user === $userState.data.id)}>
                    Hodnotit
                </Button>
            {:else}
                <div class="mx-auto flex items-center justify-center gap-1">
                    {#each Array.from({ length: 5 }) as _, id}
                        <Icon
                            onmousemove={(ev) => handleStar(ev, id)}
                            onclick={star}
                            name={hoveringStars - id >= 1 ? 'bi-star-fill' : hoveringStars - id >= 0.5 ? 'bi-star-half' : 'bi-star'}
                            class="text-yellow-500"
                        />
                    {/each}
                </div>
            {/if}
            {#each ratingUsers as rating}
                <span>{rating.user?.username} {rating.rating.toFixed(1)}</span>
            {/each}
        </div>
        <div class="col-span-7 row-span-2 row-start-3 border-2 border-black p-4">
            <h1 class="font-poppins text-xl font-bold lg:text-2xl">Popis</h1>
            Lorem ipsum dolor sit, amet consectetur adipisicing elit. Omnis tempore expedita excepturi explicabo sapiente placeat ratione culpa suscipit magni, dolore aliquid officiis
            earum repellendus mollitia, iusto rerum dicta animi velit. Lorem ipsum dolor sit amet consectetur adipisicing elit. Nam molestiae sunt eaque illo expedita magnam? Eligendi
            ratione, ab labore recusandae necessitatibus ea quo, perspiciatis, facilis omnis eum quod tenetur odit!
        </div>
        <div class="col-span-full row-span-full row-start-5 border-2 border-black p-2">
            <h1 class="font-poppins text-xl font-bold lg:text-2xl">Recenze</h1>
            {#if $userState.logged === true && ratingUsers.find((r) => r.user?.id === $userState.data.id) && !reviewUsers.find((r) => r.user?.id === $userState.data.id)}
                {#if !reviewing}
                    <Button onclick={() => (reviewing = true)} class="px-1 py-0.5 text-base lg:text-lg">Napsat recenzi</Button>
                {:else}
                    <FormItem id="review" label="Text Recenze" error={reviewData.error}>
                        <Textarea id="review" bind:value={reviewData.value} invalid={reviewData.error}></Textarea>
                    </FormItem>
                    <Button onclick={review} class="px-1 py-0.5 text-base lg:text-lg">Odeslat</Button>
                {/if}
            {/if}
            <div class="flex flex-1 flex-col gap-2">
                {#if movie.reviews.length == 0}
                    <h1 class="font-poppins font-bold text-red-500">Žádné recenze nebyly nalezeny</h1>
                {:else}
                    {#each reviewUsers as review}
                        <div class="border-2 border-black p-2">
                            <h1>{review.user?.username} - {ratingUsers.find((rating) => rating.user?.id === review.user?.id)!.rating.toFixed(1)}/5.0</h1>
                            {review.text}
                        </div>
                    {/each}
                {/if}
            </div>
        </div>
    {/if}
</div>
