//Regions

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

const SwalAlert = async (data: any) => {
    //@ts-ignore
    return Swal.fire({
        toast: true,
        position: 'top-end',
        timer: 2000,
        timerProgressBar: true,
        showCancelButton: false,
        showConfirmButton: false,
        ...data
    });
};

const putRegions = (value: string) => {
    const region = document.querySelector('select#region') as HTMLSelectElement;

    let html = '<option disabled selected>Vyberte kraj</option>';

    const list = regions[value == 'Česká Republika' ? 'cs' : 'sk'];

    for (const region of list) {
        html += `<option value="${region}">${region}</option>`;
    }

    region.innerHTML = html;
};

document.querySelector('select#state')?.addEventListener('change', (ev) => {
    const select = ev.currentTarget as HTMLSelectElement;
    const value = select.value;
    putRegions(value);
});

//if state is selected, then fire change event
document.addEventListener('DOMContentLoaded', () => {
    const state = document.querySelector('select#state') as HTMLSelectElement;
    if (['Česká Republika', 'Slovensko'].includes(state.value)) {
        state.dispatchEvent(new Event('change'));
    }
});

let state2 = false;

document.querySelector('form#form1')?.addEventListener('submit', async (ev) => {
    ev.preventDefault();

    const formData = new FormData();
    formData.append('Phone', (document.querySelector('input#phone') as HTMLInputElement).value);
    formData.append('Email', (document.querySelector('input#email') as HTMLInputElement).value);

    //check if user is not in db by email/phone ig
    const response = await fetch('/Api/CheckStudent', {
        method: 'POST',
        body: formData
    });

    const json = await response.json();

    if (!json.status) {
        SwalAlert({
            icon: 'error',
            title: json.message
        });
        return;
    }

    const count = json.count;

    if (count != 0) {
        SwalAlert({
            icon: 'error',
            title: 'Email, nebo telefonní číslo bylo již použito'
        });
        return;
    }

    state2 = true;

    const select = document.querySelector('form#form1') as HTMLSelectElement;
    select.classList.remove('flex');
    select.classList.add('hidden');

    const select2 = document.querySelector('form#form2') as HTMLSelectElement;
    select2.classList.remove('hidden');
    select2.classList.add('flex');
    return;
});

document.querySelector('form#form2')?.addEventListener('submit', async (ev) => {
    ev.preventDefault();
    const form1 = new FormData(document.querySelector('form#form1') as HTMLFormElement);
    const form2 = new FormData(document.querySelector('form#form2') as HTMLFormElement);

    const mainForm = new FormData();
    for (const [key, value] of form1.entries()) {
        mainForm.append(key, value);
    }

    for (const [key, value] of form2.entries()) {
        mainForm.append(key, value);
    }

    //send final form
    const response = await fetch('/Api/Form', {
        method: 'POST',
        body: mainForm
    });

    const json = await response.json();

    if (!json.status) {
        SwalAlert({
            icon: 'error',
            title: json.message
        });
        return;
    }

    window.location.replace('/Home/Success');
});

document.querySelector('button#back')?.addEventListener('click', (ev) => {
    ev.preventDefault();

    const select = document.querySelector('form#form1') as HTMLSelectElement;
    select.classList.add('flex');
    select.classList.remove('hidden');

    const select2 = document.querySelector('form#form2') as HTMLSelectElement;
    select2.classList.add('hidden');
    select2.classList.remove('flex');
});

const search = async (value: string) => {
    //FETCH API
    const formData = new FormData();
    formData.append('Text', value);
    const response = await fetch('/Api/GetSchools', {
        method: 'POST',
        body: formData
    });

    const json = await response.json();

    if (!json.status) {
        return [];
    }

    const list = json.items as { id: number; name: string }[];

    return list.filter((i) => i.name.includes(value));
};

const addListener = (inputId: string, dataId: string, fakeId: string, programs: string) => {
    const input = document.querySelector(`input#${inputId}`) as HTMLInputElement;
    const data = document.querySelector(`div#${dataId}`) as HTMLDataListElement;
    const output = document.querySelector(`input#${fakeId}`) as HTMLInputElement;

    input.addEventListener('input', async (ev) => {
        const value = (ev.currentTarget as HTMLInputElement).value;

        const result = await search(value);

        if (result.length == 0) {
            data.classList.remove('flex');
            data.classList.add('hidden');
        } else {
            data.classList.remove('hidden');
            data.classList.add('flex');
        }

        let html = '';
        for (const item of result) {
            html += `<div data-id="${item.id}" class="hover:bg-primary cursor-pointer rounded-md border-2 border-white p-1 text-lg text-white transition-colors duration-200">${item.name}</div>`;
        }

        data.innerHTML = html;

        data.querySelectorAll('div').forEach((el) => {
            const id = (el.attributes.getNamedItem('data-id') as Attr).value;
            el.addEventListener('click', () => {
                data.classList.remove('flex');
                data.classList.add('hidden');

                output.value = id;
                input.value = (el as HTMLDivElement).innerText;

                //load programs
                loadPrograms(id, programs);
            });
        });
    });
};

const loadPrograms = async (id: string, programsId: string) => {
    //FETCH API

    const formData = new FormData();
    formData.append('Id', id);
    const response = await fetch('/Api/GetPrograms', {
        method: 'POST',
        body: formData
    });

    const json = await response.json();

    if (!json.status) {
        return [];
    }

    const programs = json.items as { id: number; name: string }[];

    const select = document.querySelector(`select#${programsId}`) as HTMLSelectElement;
    let html = '<option disabled selected>Vyber obor</option>';

    for (const program of programs) {
        html += `<option value="${program.id}">${program.name}</option>`;
    }

    select.innerHTML = html;
};

addListener('hs1', 'h1List', 'hs1Real', 'program1');
addListener('hs2', 'h2List', 'hs2Real', 'program2');
addListener('hs3', 'h3List', 'hs3Real', 'program3');
