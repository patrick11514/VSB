document.addEventListener('included', () => {
    //nav open
    const nav = document.querySelector('nav');
    const menuItems = nav?.querySelector('div');
    const navContent = nav?.querySelector<HTMLDivElement>('div#navContent');

    let opened = false;
    const divList = nav?.querySelector('div#divlist');
    const navBackdrop = nav?.querySelector('div#backdrop');

    type NavItems = 'vehicles' | 'energy' | 'charging' | 'discover' | 'shop' | 'close';
    let currentNav: NavItems | undefined;
    let currentNavElement: HTMLElement | undefined;

    const setNavContentHeight = (element: HTMLElement) => {
        if (!navContent) return;
        navContent.style.removeProperty('--nav-height');
        const computed = getComputedStyle(navContent);
        const elComputed = getComputedStyle(element);

        const sum = parseInt(computed.paddingTop) + parseInt(computed.paddingBottom) + parseInt(elComputed.height);
        navContent.style.height = `${sum}px`;
    };

    navContent?.querySelectorAll<HTMLDivElement>('div[href]')?.forEach((link) => link.addEventListener('click', () => window.location.replace(link.getAttribute('href')!)));

    const handleNavOpen = () => {
        if (!nav || !menuItems || !navContent || !navBackdrop) return;

        if (currentNav) {
            const elements = Array.from(nav.querySelectorAll('div#navContent > div')) as HTMLElement[];
            for (const element of elements) {
                if (element.id === currentNav) {
                    element.classList.remove('opacity-0');
                    element.classList.add('opacity-100');
                    element.classList.add('z-50');

                    setNavContentHeight(element);
                } else {
                    element.classList.add('opacity-0');
                    element.classList.remove('opacity-100');
                    element.classList.remove('z-50');
                }
            }
        }

        if (opened) {
            //add bg white
            menuItems.classList.add('text-black');
            menuItems.classList.remove('text-white');

            //show content
            navContent.classList.remove('-translate-y-8', 'invisible');
            navContent.classList.add('bg-white');

            //show backdrop
            navBackdrop.classList.add('backdrop-blur');
            setTimeout(() => {
                navBackdrop.classList.remove('invisible');
                navBackdrop.classList.remove('opacity-0');
            }, 100);

            //disable scroll
            document.querySelector('html')!.style.overflow = 'hidden';
        } else {
            if ((window.navLastScroll ?? 0) <= 0) {
                //remove bg white
                menuItems.classList.remove('text-black');
                menuItems.classList.add('text-white');
            }

            //hide content
            navContent.classList.add('-translate-y-8', 'invisible');
            navContent.classList.remove('bg-white');

            //hide
            navBackdrop.classList.add('opacity-0');
            setTimeout(() => {
                navBackdrop.classList.add('invisible');
                navBackdrop.classList.remove('backdrop-blur');
            }, 500);

            //hide currently opened nav data
            const opened = document.querySelector(`div#` + currentNav);
            if (opened) {
                opened.classList.add('opacity-0');
                opened.classList.remove('opacity-100');
            }

            document.querySelector('html')!.style.overflow = 'auto';
        }
    };

    window.addEventListener('resize', () => {
        if (!nav) return;
        if (!currentNav) return;

        const element = nav.querySelector(`div#navContent > div#${currentNav}`);
        if (!element) return;

        setNavContentHeight(element as HTMLElement);
    });

    nav?.addEventListener('mouseover', (ev) => {
        const target = ev.target as HTMLElement | null;
        if (!target || !divList) return;

        if (!divList.contains(target) || target === divList) {
            return;
        }

        const dataset = target.dataset;

        opened = true;

        if ('nav' in dataset) {
            const dataNav = dataset.nav as NavItems;

            const preCurrent = currentNavElement;
            if (dataNav === 'close') {
                opened = false;
                currentNavElement = undefined;
            } else {
                currentNav = dataNav;

                currentNavElement = target;

                currentNavElement.classList.add('hover');
            }

            if (preCurrent) {
                // remove from previous
                preCurrent.classList.remove('hover');
            }
        }

        handleNavOpen();
    });

    document.addEventListener('mouseover', (ev) => {
        if (!opened) return;
        if (!ev.target) return;

        if (ev.target != navBackdrop) return;
        if (currentNavElement) {
            currentNavElement.classList.remove('hover');
        }
        currentNavElement = undefined;
        opened = false;

        handleNavOpen();
    });

    //dynamically assign animations
    Array.from(document.querySelectorAll<HTMLElement>('[data-dynamic]')).forEach((element) => {
        const data = element.dataset;
        if (!('offset' in data) || !('attr' in data)) return;
        if (!data.offset || !data.attr) return;

        const offset = parseFloat(data.offset);
        const attr = data.attr;

        const childs = element.children;

        let begin = offset;
        for (const child of Array.from(childs) as HTMLElement[]) {
            child.style.setProperty(attr, `${begin.toFixed(2)}s`);

            begin += offset;
        }
    });

    //MOBILE NAV SECTION
    const mobileButton = nav?.querySelector('button#mobileButton');
    const navNav = nav?.querySelector('div#mobileNav');

    let navOpened = false;

    mobileButton?.addEventListener('click', () => {
        if (!navNav) return;

        if (navOpened) {
            navNav.classList.add('invisible');
            document.documentElement.classList.remove('overflow-hidden');
            if (announcement) announcement.classList.add('z-30');
        } else {
            navNav.classList.remove('invisible');
            document.documentElement.classList.add('overflow-hidden');
            if (announcement) announcement.classList.remove('z-30');
        }

        navOpened = !navOpened;
    });

    const announcement = document.querySelector('#announcement');
    if (announcement) {
        navNav?.classList.add('-top-[52px]');
    }

    nav?.querySelector('#closeMobileNav')?.addEventListener('click', () => {
        navNav?.classList.add('invisible');
        document.documentElement.classList.remove('overflow-hidden');
        navOpened = false;
        if (announcement) announcement.classList.add('z-30');
    });

    document.querySelectorAll<HTMLDivElement>('[data-subnav]').forEach((div) => {
        const subnav = div.querySelector('#subnav');
        if (!subnav) return;

        div.addEventListener('mouseover', () => {
            subnav.classList.remove('hidden');
            subnav.classList.add('flex');
        });

        div.addEventListener('mouseleave', () => {
            subnav.classList.remove('flex');
            subnav.classList.add('hidden');
        });
    });
});
