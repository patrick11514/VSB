/// Load nested HTMLS

const loadHTML = async (el: HTMLElement) => {
    const attr = el.getAttribute('include');
    if (!attr) return;

    const response = await fetch(attr, {
        mode: 'no-cors',
        credentials: 'include'
    });

    const html = await response.text();
    const temp = document.createElement('div');
    temp.innerHTML = html;
    const child = temp.firstChild as HTMLElement | null;

    if (!child) return;

    el.replaceWith(child);
};

/// SVG LOAD

const load = async (svg: SVGElement) => {
    const attr = svg.getAttribute('load');
    if (!attr) return;

    const response = await fetch(`/assets/svgs/${attr}`, {
        mode: 'no-cors',
        credentials: 'include'
    });

    const html = await response.text();
    const temp = document.createElement('div');
    temp.innerHTML = html;
    const child = temp.firstChild as SVGElement | null;

    if (!child) return;

    svg.classList.forEach((c) => child.classList.add(c));

    svg.replaceWith(child);
};

const main = async () => {
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

    let navLastScroll: number | null = null;
    let percentage = 0;

    const checkNav = () => {
        if (!nav || !menuItems || !navContent) return;

        const scroll = window.scrollY;

        if (scroll > 0) {
            menuItems.classList.add('bg-white', 'text-black');
            menuItems.classList.remove('text-white', 'bg-transparent');
        } else {
            menuItems.classList.remove('bg-white', 'text-black');
            menuItems.classList.add('text-white', 'bg-trasparent');
        }

        const lastScroll = navLastScroll;
        navLastScroll = scroll;

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

        console.log(percentage, -100 + percentage);

        nav.style.transform = `translateY(${-100 + percentage}%)`;
    };

    window.addEventListener('scroll', checkNav);
    checkNav();

    //nav open

    type Collections = [HTMLCollection, HTMLElement];

    const inCollection = <$CollectionType extends Collections>(collection: $CollectionType[0], item: $CollectionType[1]) => {
        for (let i = 0; i < collection.length; ++i) {
            if (collection.item(i) == item) return true;
        }
        return false;
    };

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

    const handleNavOpen = () => {
        if (!nav || !menuItems || !navContent || !navBackdrop) return;

        if (currentNav) {
            const elements = Array.from(nav.querySelectorAll('div#navContent > div')) as HTMLElement[];
            for (const element of elements) {
                if (element.id === currentNav) {
                    element.classList.remove('opacity-0');
                    element.classList.add('opacity-100');

                    setNavContentHeight(element);
                } else {
                    element.classList.add('opacity-0');
                    element.classList.remove('opacity-100');
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
            navBackdrop.classList.remove('invisible');
        } else {
            if ((navLastScroll ?? 0) <= 0) {
                //remove bg white
                menuItems.classList.remove('text-black');
                menuItems.classList.add('text-white');
            }

            //hide content
            navContent.classList.add('-translate-y-8', 'invisible');
            navContent.classList.remove('bg-white');

            //hide
            navBackdrop.classList.remove('backdrop-blur');
            navBackdrop.classList.add('invisible');

            //hide currently opened nav data
            const opened = document.querySelector(`div#` + currentNav);
            if (opened) {
                opened.classList.add('opacity-0');
                opened.classList.remove('opacity-100');
            }
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
};

const loadIncludes = async () => {
    const els = Array.from(document.querySelectorAll<HTMLElement>('[include]')).map(loadHTML);
    await Promise.all(els);

    const svgs = Array.from(document.querySelectorAll<SVGElement>('svg[load]')).map(load);
    await Promise.all(svgs);

    main();
};

loadIncludes();
