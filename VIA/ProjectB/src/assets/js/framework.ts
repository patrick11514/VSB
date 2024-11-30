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
    const child = temp.querySelector('[root]') as HTMLElement | null;
    const scripts = temp.querySelectorAll('script');

    if (!child) return;

    el.replaceWith(child);

    //append script elements after child was put
    if (scripts.length > 0) {
        for (let i = 0; i < scripts.length; ++i) {
            const newScript = document.createElement('script');
            newScript.defer = true;
            const script = scripts.item(i);

            if (script.src) {
                newScript.src = script.src;
            } else {
                newScript.textContent = script.text;
            }

            document.head.appendChild(newScript);
        }
    }
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

const INCLUDE_EVENT = new Event('included');

const loadIncludes = async () => {
    const els = Array.from(document.querySelectorAll<HTMLElement>('[include]')).map(loadHTML);
    await Promise.all(els);

    const svgs = Array.from(document.querySelectorAll<SVGElement>('svg[load]')).map(load);
    await Promise.all(svgs);

    document.dispatchEvent(INCLUDE_EVENT);
    console.log('fired');
};

loadIncludes();
