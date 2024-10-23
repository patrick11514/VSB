const days = {
    PO: 'Pondělí',
    UT: 'Úterý',
    ST: 'Středa',
    CT: 'Čtvrtek',
    PA: 'Pátek'
};

/**
 * @type {{
 *  day: keyof days,
 *  name: string,
 *  room: string,
 *  group: string,
 *  additionalInfo?: string,
 *  from: Date,
 *  to: Date,
 *  type: "Lecture" | "Excercise",
 *  teacher: string
 * }[]}
 */
const DATA = [
    {
        day: 'PO',
        name: 'ZPG',
        room: 'POREC1',
        group: 'P/01',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 1, 9, 0),
        to: new Date(2024, 1, 1, 10, 30),
        type: 'Lecture',
        teacher: 'M. Němec'
    },
    {
        day: 'PO',
        name: 'VIA',
        room: 'POREB213',
        group: 'C/01',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 1, 10, 45),
        to: new Date(2024, 1, 1, 12, 15),
        type: 'Excercise',
        teacher: 'M. Radecký'
    },
    {
        day: 'PO',
        name: 'VIA',
        room: 'POREC2',
        group: 'P/01',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 1, 12, 30),
        to: new Date(2024, 1, 1, 14, 0),
        type: 'Lecture',
        teacher: 'M. Radecký'
    },
    {
        day: 'UT',
        name: 'OSY',
        room: 'POREC2',
        group: 'P/01',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 2, 7, 15),
        to: new Date(2024, 1, 2, 8, 45),
        type: 'Lecture',
        teacher: 'P. Olivka'
    },
    {
        day: 'UT',
        name: 'SPR',
        room: 'PORJA401',
        group: 'P/01',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 2, 9, 0),
        to: new Date(2024, 1, 2, 10, 30),
        type: 'Lecture',
        teacher: 'Z. Sawa'
    },
    {
        day: 'UT',
        name: 'SPR',
        room: 'PORJA401',
        group: 'C/01',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 2, 10, 45),
        to: new Date(2024, 1, 2, 12, 15),
        type: 'Excercise',
        teacher: 'Z. Sawa'
    },
    {
        day: 'UT',
        name: 'ZPG',
        room: 'POREB408',
        group: 'C/07',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 2, 12, 30),
        to: new Date(2024, 1, 2, 14, 0),
        type: 'Excercise',
        teacher: 'M. Němec'
    },
    {
        day: 'UT',
        name: 'VIS',
        room: 'POREC3',
        group: 'P/01',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 3, 14, 15),
        to: new Date(2024, 1, 3, 15, 45),
        type: 'Lecture',
        teacher: 'M. Kudělka'
    },
    {
        day: 'ST',
        name: 'ZSU',
        room: 'POREC1',
        group: 'P/01',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 3, 9, 0),
        to: new Date(2024, 1, 3, 10, 30),
        type: 'Lecture',
        teacher: 'J. Platoš'
    },
    {
        day: 'ST',
        name: 'TAMZ II',
        room: 'POREC3',
        group: 'P/01',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 3, 10, 45),
        to: new Date(2024, 1, 3, 12, 15),
        type: 'Lecture',
        teacher: 'M. Krumnikl'
    },
    {
        day: 'ST',
        name: 'VIS',
        room: 'POREB406',
        group: 'C/05',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 3, 12, 30),
        to: new Date(2024, 1, 3, 14, 0),
        type: 'Excercise',
        teacher: 'T. Anlauf'
    },
    {
        day: 'CT',
        name: 'TAMZ II',
        room: 'POREB406',
        group: 'C/02',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 3, 9, 0),
        to: new Date(2024, 1, 3, 10, 30),
        type: 'Excercise',
        teacher: 'R. Fusek'
    },
    {
        day: 'CT',
        name: 'OSY',
        room: 'POREB430',
        group: 'C/10',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 3, 11, 30),
        to: new Date(2024, 1, 3, 14, 0),
        type: 'Excercise',
        teacher: 'P. Olivka'
    },
    {
        day: 'CT',
        name: 'ZSU',
        room: 'PORRV203',
        group: 'C/02',
        additionalInfo: 'Nepravidelně',
        from: new Date(2024, 1, 3, 14, 15),
        to: new Date(2024, 1, 3, 15, 45),
        type: 'Excercise',
        teacher: 'M. Hanuš'
    }
];

/**
 * @type {Record<keyof days, {
 *  name: string,
 *  room: string,
 *  group: string,
 *  additionalInfo?: string,
 *  from: Date,
 *  to: Date,
 *  type: "Lecture" | "Excercise"
 * }[]>}
 */
const agregated = {};

for (const day of Object.keys(days)) {
    const data = DATA.filter((item) => item.day === day);
    data.sort((a, b) => a.from - b.from);
    agregated[day] = data;
}

const tbody = document.querySelector('tbody');
const dayStart = new Date(2024, 1, 1, 7, 15);
const dayEnd = new Date(2024, 1, 1, 19, 15);
let currentDay = new Date(dayStart);

/**
 * Mapp times for example 8:45 -> 9:00 as start of next subject, or 9:45 -> 9:45
 * @type {Record<string, string>}
 */
const getStartFromEnd = {};

/**
 * Convert date to string in format HH:MM
 * @param {Date} date
 * @returns {string}
 */
const getTime = (date) => {
    const hours = date.getHours().toString().padStart(2, '0');
    const minutes = date.getMinutes().toString().padStart(2, '0');

    return `${hours}:${minutes}`;
};

/**
 * Convert time string to Date object
 * @param {string} time
 * @returns {Date}
 */
const getDate = (time) => {
    const [hours, minutes] = time.split(':');
    const date = new Date(currentDay);
    date.setHours(parseInt(hours));
    date.setMinutes(parseInt(minutes));

    return date;
};

let spaces = 0;
let add = false;

currentDay.setMinutes(currentDay.getMinutes() + 45);

while (true) {
    const from = getTime(currentDay);

    if (spaces == 1) {
        currentDay.setMinutes(currentDay.getMinutes() + 15);
    }

    const to = getTime(currentDay);

    currentDay.setMinutes(currentDay.getMinutes() + 45);
    spaces++;

    if (spaces == 2) {
        spaces = 0;
    }

    getStartFromEnd[from] = to;

    if (currentDay.getTime() >= dayEnd.getTime()) {
        break;
    }
}

currentDay = new Date(dayStart);

for (const [day, subjects] of Object.entries(agregated)) {
    currentDay = new Date(dayStart);
    const tr = document.createElement('tr');
    const dayName = document.createElement('td');
    dayName.innerText = days[day];
    tr.appendChild(dayName);

    /**
     * @type {typeof DATA[number] | undefined}
     */
    let popped = undefined;
    let spaces = 0;

    while (true) {
        if (!popped) {
            popped = subjects.shift();
        }

        if (popped && popped.from.getHours() === currentDay.getHours() && popped.from.getMinutes() === currentDay.getMinutes()) {
            const td = document.createElement('td');
            td.classList.add(popped.type === 'Excercise' ? 'excercises' : 'lecture');
            const blocks = Math.floor((popped.to.getTime() - popped.from.getTime()) / 1000 / 60 / 45);
            td.colSpan = blocks;

            const Id = `${popped.type}.${popped.name}`;

            td.addEventListener('click', () => showNote(Id));

            const subjectInfo = document.createElement('div');
            subjectInfo.classList.add('row');

            const link = document.createElement('a');
            link.href = '#';
            link.innerText = popped.name;
            subjectInfo.appendChild(link);

            if (popped.additionalInfo) {
                const note = document.createElement('span');
                note.innerText = popped.additionalInfo;
                note.classList.add('note');
                subjectInfo.appendChild(note);
            }

            const icon = document.createElement('div');
            icon.classList.add('icon');
            subjectInfo.appendChild(icon);
            td.appendChild(subjectInfo);

            const teacher = document.createElement('div');
            teacher.innerText = popped.teacher;
            td.appendChild(teacher);

            const room = document.createElement('div');
            room.classList.add('row');

            const roomName = document.createElement('div');
            roomName.innerText = popped.room;
            room.appendChild(roomName);

            const group = document.createElement('div');
            group.innerText = popped.group;
            group.classList.add('end');
            room.appendChild(group);

            td.appendChild(room);

            tr.appendChild(td);

            //set current time to end of subject
            currentDay = getDate(getStartFromEnd[getTime(popped.to)]);

            popped = undefined;

            //skip loop to check if there is another subject
            continue;
        } else {
            //fill empty cell
            const td = document.createElement('td');
            tr.appendChild(td);
        }

        ++spaces;

        currentDay.setMinutes(currentDay.getMinutes() + 45);

        if (spaces == 2) {
            //add another 15 minutes, if we want to skip break;
            currentDay.setMinutes(currentDay.getMinutes() + 15);
            spaces = 0;
        }

        if (currentDay.getTime() >= dayEnd.getTime()) {
            break;
        }
    }

    tbody.appendChild(tr);
}

const time = document.querySelector('#time');

const printTime = () => {
    if (!time) return;

    const now = new Date();
    const day = now.getDate().toString();
    const month = (now.getMonth() + 1).toString();
    const year = now.getFullYear();
    const hour = now.getHours().toString().padStart(2, '0');
    const minutes = now.getMinutes().toString().padStart(2, '0');
    const seconds = now.getSeconds().toString().padStart(2, '0');

    time.textContent = `${day}.${month}.${year} ${hour}:${minutes}:${seconds}`;
};

printTime();

setInterval(printTime, 1 * 1000);

const resetHighlight = () => {
    document.querySelectorAll('.excercises, .lecture').forEach((item) => {
        item.classList.remove('hidden');
    });
};

document.querySelector('#highlightLectures')?.addEventListener('click', () => {
    resetHighlight();
    document.querySelectorAll('.excercises').forEach((item) => {
        item.classList.add('hidden');
    });
});

document.querySelector('#highlightExcercises')?.addEventListener('click', () => {
    resetHighlight();
    document.querySelectorAll('.lecture').forEach((item) => {
        item.classList.add('hidden');
    });
});

document.querySelector('#resetHighlight')?.addEventListener('click', resetHighlight);

const dialog = document.querySelector('dialog');

const textArea = dialog.querySelector('textarea');
const btn = dialog.querySelector('button');

const showNote = (id) => {
    const data = localStorage.getItem(id);

    if (data) {
        textArea.value = data;
    } else {
        textArea.value = '';
    }

    dialog.setAttribute("data-id", id)

    dialog.show();
};

btn.addEventListener('click', () => {
    const id = dialog.getAttribute("data-id")
    dialog.removeAttribute("data-id")

    localStorage.setItem(id, textArea.value);

    dialog.close();
});
