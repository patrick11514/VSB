"use strict";
const regions = {
    cs: [
        'Hlavní město praha',
        'Středočeský kraj',
        'Jihočeský kraj',
        'Plzeňský kraj',
        'Karlovarský kraj',
        'Ústecký kraj',
        'Liberecký kraj',
        'Královéhradecký kraj',
        'Pardubický kraj',
        'Kraj Vysočina',
        'Jihomoravský kraj',
        'Olomoucký kraj',
        'Moravskoslezský kraj',
        'Zlínský kraj'
    ],
    sk: ['Trnavský', 'Bratislavský', 'Trenčínský', 'Nitranský', 'Žilinský', 'Banskobystrický', 'Prešovský', 'Košický']
};
const putRegions = (value) => {
    const region = document.querySelector('select#region');
    let html = '<option disabled selected>Vyberte kraj</option>';
    const list = regions[value == 'Česká Republika' ? 'cs' : 'sk'];
    for (const region of list) {
        html += `<option value="${region}">${region}</option>`;
    }
    region.innerHTML = html;
};
document.querySelector('select#state')?.addEventListener('change', (ev) => {
    const select = ev.currentTarget;
    const value = select.value;
    putRegions(value);
});
document.addEventListener('DOMContentLoaded', () => {
    const state = document.querySelector('select#state');
    if (['Česká Republika', 'Slovensko'].includes(state.value)) {
        state.dispatchEvent(new Event('change'));
    }
});
let state2 = false;
document.querySelector('form#form1')?.addEventListener('submit', async (ev) => {
    ev.preventDefault();
    state2 = true;
    const select = document.querySelector('form#form1');
    select.classList.remove('flex');
    select.classList.add('hidden');
    const select2 = document.querySelector('form#form2');
    select2.classList.remove('hidden');
    select2.classList.add('flex');
    return;
});
document.querySelector('form#form2')?.addEventListener('submit', async (ev) => {
    ev.preventDefault();
    const form1 = new FormData(document.querySelector('form#form1'));
    const form2 = new FormData(document.querySelector('form#form2'));
    const mainForm = new FormData();
    for (const [key, value] of form1.entries()) {
        mainForm.append(key, value);
    }
    for (const [key, value] of form2.entries()) {
        mainForm.append(key, value);
    }
    fetch('/api', {
        method: 'POST',
        body: mainForm
    });
    window.location.replace('/success');
});
document.querySelector('button#back')?.addEventListener('click', (ev) => {
    ev.preventDefault();
    const select = document.querySelector('form#form1');
    select.classList.add('flex');
    select.classList.remove('hidden');
    const select2 = document.querySelector('form#form2');
    select2.classList.add('hidden');
    select2.classList.remove('flex');
});
const search = async (value) => {
    const list = [
        { id: 0, name: 'Střední Škola akademika Heyrovského příspěvková orgranizase ORG ostrava zábře bnla bla fgsdg sdg sdgsdgs' },
        { id: 1, name: 'Střední Škola Stavební Praha' },
        { id: 2, name: 'Střední škola jánského olomouc' },
        { id: 3, name: 'Střední škola IDK LOOL' },
        { id: 4, name: 'Nevim něc' }
    ];
    return list.filter((i) => i.name.includes(value));
};
const addListener = (inputId, dataId, fakeId, programs) => {
    const input = document.querySelector(`input#${inputId}`);
    const data = document.querySelector(`div#${dataId}`);
    const output = document.querySelector(`input#${fakeId}`);
    input.addEventListener('input', async (ev) => {
        const value = ev.currentTarget.value;
        const result = await search(value);
        if (result.length == 0) {
            data.classList.remove('flex');
            data.classList.add('hidden');
        }
        else {
            data.classList.remove('hidden');
            data.classList.add('flex');
        }
        let html = '';
        for (const item of result) {
            html += `<div data-id="${item.id}" class="hover:bg-primary cursor-pointer rounded-md border-2 border-white p-1 text-lg text-white transition-colors duration-200">${item.name}</div>`;
        }
        data.innerHTML = html;
        data.querySelectorAll('div').forEach((el) => {
            const id = el.attributes.getNamedItem('data-id').value;
            el.addEventListener('click', () => {
                data.classList.remove('flex');
                data.classList.add('hidden');
                output.value = id;
                input.value = el.innerText;
                loadPrograms(id, programs);
            });
        });
    });
};
const loadPrograms = async (id, programsId) => {
    const programs = ['Program1' + id, 'Program2' + id, 'Program3' + id, 'Program4' + id, 'Program5' + id];
    const select = document.querySelector(`select#${programsId}`);
    let html = '<option disabled selected>Vyber obor</option>';
    for (const programId in programs) {
        const program = programs[programId];
        html += `<option value="${programId}">${program}</option>`;
    }
    select.innerHTML = html;
};
addListener('hs1', 'h1List', 'hs1Real', 'program1');
addListener('hs2', 'h2List', 'hs2Real', 'program2');
addListener('hs3', 'h3List', 'hs3Real', 'program3');
//# sourceMappingURL=main.js.map