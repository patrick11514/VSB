const load = async (svg: SVGElement) => {
    const attr = svg.getAttribute('load');
    if (!attr) return;

    const response = await fetch(`/assets/svgs/${attr}`);
    const html = await response.text();
    const temp = document.createElement('div');
    temp.innerHTML = html;
    const child = temp.firstChild as SVGElement | null;

    if (!child) return;

    svg.classList.forEach((c) => child.classList.add(c));

    svg.replaceWith(child);
};

const svgs = Array.from(document.querySelectorAll('svg'))
    .filter((el) => el.hasAttribute('load'))
    .map(load);
Promise.all(svgs);
