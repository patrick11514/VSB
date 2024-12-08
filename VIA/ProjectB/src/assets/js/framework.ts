/// Load nested HTMLS

const log = (...messages: string[]) => {
    console.log('[FrameWork] ' + messages.join(' '));
};

const loadHTML = async (el: HTMLElement) => {
    const attr = el.getAttribute('include');
    if (!attr) return;

    log('Fetching element', attr);

    const response = await fetch('/' + attr, {
        mode: 'no-cors',
        credentials: 'include'
    });

    log('Fetched element', attr);

    const html = await response.text();
    const temp = document.createElement('div');
    temp.innerHTML = html;
    const child = temp.querySelector('[root]') as HTMLElement | null;
    const scripts = temp.querySelectorAll('script');

    if (!child) return;

    //merge classes
    if (el.classList.length > 0) {
        el.classList.forEach((cls) => child.classList.add(cls));
    }

    el.replaceWith(child);

    log('Loading script tags for', attr);

    //append script elements after child was put
    if (scripts.length > 0) {
        const scriptLoad: Promise<undefined>[] = [];

        for (let i = 0; i < scripts.length; ++i) {
            const newScript = document.createElement('script');
            newScript.defer = true;
            const script = scripts.item(i);

            if (script.src) {
                newScript.src = script.src;
            } else {
                newScript.textContent = script.text;
            }

            scriptLoad.push(
                new Promise((resolve) => {
                    newScript.addEventListener('load', () => resolve(undefined), {
                        once: true
                    });
                })
            );

            document.head.appendChild(newScript);
        }

        //wait for all scripts to load
        await Promise.all(scriptLoad);
    }

    log('Tag loaded:', attr);
};

/// SVG LOAD

const load = async (svg: SVGElement) => {
    const attr = svg.getAttribute('load');
    if (!attr) return;

    log('Loading svg', attr);
    const response = await fetch(`/assets/svgs/${attr}`, {
        mode: 'no-cors',
        credentials: 'include'
    });
    log('Svg loaded', attr);

    const html = await response.text();
    const temp = document.createElement('div');
    temp.innerHTML = html;
    const child = temp.firstChild as SVGElement | null;

    if (!child) return;

    svg.classList.forEach((c) => child.classList.add(c));

    svg.replaceWith(child);
};

const INCLUDE_EVENT = new Event('included');

const loadIncludes = async () => {
    const start = Date.now();
    log('Started!');
    log('Loading elements!');
    const els = Array.from(document.querySelectorAll<HTMLElement>('[include]')).map(loadHTML);
    await Promise.all(els);

    log('Loading svgs!');
    const svgs = Array.from(document.querySelectorAll<SVGElement>('svg[load]')).map(load);
    await Promise.all(svgs);
    log('Work done in', (Date.now() - start).toString(), 'ms');

    log('Dispatching include event!');
    document.dispatchEvent(INCLUDE_EVENT);
};

loadIncludes();
