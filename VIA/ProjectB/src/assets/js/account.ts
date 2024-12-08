const toDate = (date: Date | number) => {
    const d = new Date(date);
    const hours = d.getHours().toString().padStart(2, '0');
    const minutes = d.getMinutes().toString().padStart(2, '0');
    const seconds = d.getSeconds().toString().padStart(2, '0');
    const day = d.getDate();
    const month = d.getMonth() + 1;
    const year = d.getFullYear();

    return `${hours}:${minutes}:${seconds} ${day}.${month}.${year}`;
};

document.addEventListener('included', () => {
    //setup logout
    if (!localStorage.getItem('logged')) {
        window.location.replace('/login');
        return;
    }

    document.querySelector('#logout')?.addEventListener('click', () => {
        localStorage.removeItem('logged');
        window.location.replace('/login');
    });

    //setup menu selections
    const menus = Array.from(document.querySelectorAll<HTMLDivElement>('div[data-target]'));
    const containers = menus.map((menu) => document.querySelector<HTMLDivElement>('#' + menu.dataset.target));

    for (const id in menus) {
        const menu = menus[id];
        menu.addEventListener('click', () => {
            for (const containerId in containers) {
                const container = containers[containerId];
                if (!container) continue;

                if (containerId == id) {
                    container.classList.remove('hidden');
                    container.classList.add('flex');
                } else {
                    container.classList.add('hidden');
                    container.classList.remove('flex');
                }
            }

            for (const _menu of menus) {
                if (menu == _menu) {
                    _menu.classList.add('active');
                } else {
                    _menu.classList.remove('active');
                }
            }
        });
    }

    //counter
    const counter = document.querySelector<HTMLHeadingElement>('#counter')!;
    const to = new Date(counter.dataset.to!);
    document.querySelector('#to')!.textContent = toDate(to);

    const calculate = () => {
        const now = Date.now();
        const mills = to.getTime() - now;

        const secs = Math.floor(mills / 1000);
        const milliseconds = mills - secs * 1000;

        const mins = Math.floor(secs / 60);
        const seconds = secs - mins * 60;

        const hrs = Math.floor(mins / 60);
        const minutes = mins - hrs * 60;

        const ds = Math.floor(hrs / 24);
        const hours = hrs - ds * 24;

        const pd = (n: number, c = 2) => n.toString().padStart(c, '0');

        counter.textContent = `${ds}d ${pd(hours)}h ${pd(minutes)}m ${pd(seconds)}.${pd(milliseconds, 3)}s`;
    };

    setInterval(calculate, 10);
    calculate();

    //contact form
    const title = document.querySelector<HTMLInputElement>('input#title')!;
    const problem = document.querySelector<HTMLSelectElement>('select#problem')!;
    const text = document.querySelector<HTMLTextAreaElement>('textarea#text')!;

    document.querySelector('button#submit')?.addEventListener('click', () => {
        let ok = true;

        //title
        if (!title.value) {
            title.setCustomValidity('No input');
            ok = false;
        } else if (title.value.length < 3) {
            title.setCustomValidity('Please enter at least 3 characters');
            ok = false;
        } else if (title.value.length > 10) {
            title.setCustomValidity('Please enter maximum 10 characters');
            ok = false;
        } else {
            title.setCustomValidity('');
        }

        //problem
        console.log(problem.value);
        if (!problem.value) {
            problem.setCustomValidity('No input');
            ok = false;
        } else {
            problem.setCustomValidity('');
        }

        //problem
        if (!text.value) {
            text.setCustomValidity('No input');
            ok = false;
        } else if (text.value.length < 3) {
            text.setCustomValidity('Please enter at least 3 characters');
            ok = false;
        } else if (text.value.length > 255) {
            text.setCustomValidity('Please enter maximum 255 characters');
            ok = false;
        } else {
            text.setCustomValidity('');
        }

        if (!ok) return;

        //continue

        alert('Form sent');
    });
});
