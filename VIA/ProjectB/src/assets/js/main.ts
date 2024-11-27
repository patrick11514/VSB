/// SVG LOAD

const load = async (svg: SVGElement) => {
    const attr = svg.getAttribute('load');
    if (!attr) return;

    const response = await fetch(`/assets/svgs/${attr}`, {
        mode: "no-cors",
        credentials: "include"
    });
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

/// SCHEDULE DRIVE POPUP BUTTON

const showDriveTreshold = 800;
const scheduleButton = document.querySelector("#schedule-drive");

const checkScheduleDrive = () => {
    if (!scheduleButton) return;

    const classList = scheduleButton.classList;

    const toggle = () => {
        classList.toggle("translate-y-full");
        classList.toggle("translate-y-0");
    }

    const Y = window.scrollY;

    if (Y >= showDriveTreshold) {
        if (classList.contains("translate-y-full")) toggle();
    } else {
        if (classList.contains("translate-y-0")) toggle();
    }
}

window.addEventListener("scroll", checkScheduleDrive);
checkScheduleDrive();

/// NAV Scroll

const nav = document.querySelector("nav");
let navLastScroll: number | null = null;
let percentage = 0;

const checkNav = () => {
    if (!nav) return;

    const scroll = window.scrollY;

    if (scroll > 0) {
        nav.classList.add("bg-white", "text-black");
        nav.classList.remove("text-white");
    } else {
        nav.classList.remove("bg-white", "text-black");
        nav.classList.add("text-white");
    }

    const lastScroll = navLastScroll;
    navLastScroll = scroll;

    if (lastScroll === null) {
        return;
    }

    const diff = scroll - lastScroll;

    console.log(diff)

    if (diff > 0) { // scrolling downwards
        percentage = 0;
    } else if (diff < 0) {
        percentage = Math.min(percentage + 5, 100);
    }

    console.log(percentage, -100 + percentage)

    nav.style.transform = `translateY(${-100 + percentage}%)`;
}

window.addEventListener("scroll", checkNav);
checkNav();



