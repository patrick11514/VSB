let current = 0;

const titles = ['Cinematic Experience', 'Yoke Steering', 'Perfect Environment', 'Redesigned Second Row', 'Tesla Arcade'];
const descs = [
    'A 17” touchscreen with left-right tilt offers 2200 x 1300 resolution, true colors and exceptional responsiveness for gaming, movies and more.',
    'A bold new approach gives you a true connection to Model S, offering better steering feel and unobstructed views of your dash and the road ahead. Tap the brake and Model S automatically selects the correct direction to start your trip.',
    'Air vents are hidden throughout the cabin, while tri-zone temperature controls, ventilated seats and HEPA filtration deliver the perfect environment.',
    'Seating for three adults, with extra legroom, headroom and a stowable armrest with integrated storage and wireless charging.',
    'Play games on your in-car touchscreens.'
];

let currentTimeout: number | null = null;

document.addEventListener('included', () => {
    const carousel = document.querySelector<HTMLDivElement>('#carousel')!;
    const controls = document.querySelector<HTMLDivElement>('#controls')!;
    const title = document.querySelector('#title')!;
    const desc = document.querySelector('#desc')!;

    const videos = Array.from(carousel.querySelectorAll('video'));
    const control = Array.from(controls.querySelectorAll('div'));

    const playVideo = () => {
        const video = videos[current];
        const len = video.duration * 1000 - 500;
        video.currentTime = 0;
        video.classList.remove('opacity-0');
        control[current].classList.add('active');
        title.textContent = titles[current];
        desc.textContent = descs[current];
        video.play();

        currentTimeout = window.setTimeout(() => {
            const video = videos[current];
            video.classList.add('opacity-0');
            control[current].classList.remove('active');
            current = (current + 1) % videos.length;
            playVideo();
        }, len);
    };
    playVideo();

    for (const id in control) {
        const single = control[id];

        single.addEventListener('click', () => {
            if (currentTimeout) {
                clearTimeout(currentTimeout);
                const video = videos[current];
                video.classList.add('opacity-0');
                control[current].classList.remove('active');
            }

            current = parseInt(id);
            playVideo();
        });
    }

    //gallery
    const gallery = document.querySelector<HTMLDivElement>('#gallery')!;
    const items = Array.from(gallery.querySelectorAll('img, video'));
    const overlay = document.querySelector<HTMLDivElement>('#overlay')!;
    const galleryItem = overlay.querySelector('#gallery-item')!;

    overlay.querySelector('i')?.addEventListener('click', () => {
        galleryItem.childNodes.forEach((child) => galleryItem.removeChild(child)); // clear childs
        overlay.classList.add('hidden');
        overlay.classList.remove('flex');
    });

    for (const item of items) {
        item.addEventListener('click', () => {
            //add item to overlay
            const clone = item.cloneNode() as HTMLElement;
            clone.classList.forEach((cls) => clone.classList.remove(cls));

            galleryItem.appendChild(clone);

            overlay.classList.remove('hidden');
            overlay.classList.add('flex');
        });
    }
});
