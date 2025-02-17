type Collections = [HTMLCollection, HTMLElement];

const inCollection = <$CollectionType extends Collections>(collection: $CollectionType[0], item: $CollectionType[1]) => {
    for (let i = 0; i < collection.length; ++i) {
        if (collection.item(i) == item) return true;
    }
    return false;
};

document.addEventListener('included', () => {
    /// SCHEDULE DRIVE POPUP BUTTON

    const showDriveTreshold = 800;
    const scheduleButton = document.querySelector('#schedule-drive');

    const checkScheduleDrive = () => {
        if (!scheduleButton) return;

        const classList = scheduleButton.classList;

        const toggle = () => {
            classList.toggle('translate-y-full');
            classList.toggle('translate-y-0');
        };

        const Y = window.scrollY;

        if (Y >= showDriveTreshold) {
            if (classList.contains('translate-y-full')) toggle();
        } else {
            if (classList.contains('translate-y-0')) toggle();
        }
    };

    window.addEventListener('scroll', checkScheduleDrive);
    checkScheduleDrive();

    /// NAV Scroll

    const nav = document.querySelector('nav');
    const menuItems = nav?.querySelector('div');
    const navContent = nav?.querySelector<HTMLDivElement>('div#navContent');
    const mobileButton = nav?.querySelector<HTMLButtonElement>('button#mobileButton');

    window.navLastScroll = null;
    let percentage = 0;

    const checkNav = () => {
        if (!nav || !menuItems || !navContent || !mobileButton) return;

        const scroll = window.scrollY;

        if (scroll > 0) {
            menuItems.classList.add('bg-white', 'text-black');
            menuItems.classList.remove('text-white', 'bg-transparent');

            //mobile button
            mobileButton.classList.add('text-black');
            mobileButton.classList.remove('text-white');
        } else {
            menuItems.classList.remove('bg-white', 'text-black');
            menuItems.classList.add('text-white', 'bg-trasparent');

            //mobile button
            mobileButton.classList.remove('text-black');
            mobileButton.classList.add('text-white');
        }

        const lastScroll = window.navLastScroll;
        window.navLastScroll = scroll;

        if (lastScroll === null) {
            return;
        }

        const diff = scroll - lastScroll;

        if (diff > 0) {
            // scrolling downwards
            percentage = 0;
        } else if (diff < 0) {
            percentage = Math.min(percentage + 3, 100);
        }

        if (scroll == 0) {
            percentage = 100;
        }

        if (nav.classList.contains('no-semi') && scroll > 0 && percentage != 100 && diff < 0) {
            return;
        }

        nav.style.transform = `translateY(${-100 + percentage}%)`;
    };

    window.addEventListener('scroll', checkNav);
    checkNav();
});
