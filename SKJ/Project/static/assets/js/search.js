"use strict";
const searchBox = document.querySelector('input#search');
const result = document.querySelector('div#results');
let promise = undefined;
searchBox.addEventListener('input', async (e) => {
    const element = e.currentTarget;
    if (promise) {
        clearTimeout(promise);
    }
    promise = setTimeout(() => searchText(element.value), 500);
});
const userHtml = (user) => {
    return `<a href="/user/${user.id}" class="gap-2 flex flex-row min-h-24 w-[60%] min-w-64 border-primary border-2 rounded-lg mx-auto p-4 hover:bg-secondary duration-200 transition-colors cursor-pointer">
    <i class="bi bi-person-fill text-6xl my-auto"></i>
    <div class="flex flex-col my-auto">
        <h2 class="text-2xl font-bold">${user.name}</h2>
        <h3 class="text-xl font-bold">Účet založený ${user.date}</h3>
    </div>
    <h2 class="text-3xl ml-auto">${user.likes} <i class="bi bi-star-fill text-yellow-400" ></i></h2>
</a>`;
};
const figureHtml = (figure) => {
    return figure.image === null
        ? `<a href="/figure/${figure.id}" class="gap-2 flex flex-row min-h-24 w-[60%] min-w-64 border-primary border-2 rounded-lg mx-auto p-4 hover:bg-secondary duration-200 transition-colors cursor-pointer">
<i class="bi bi-person-standing text-6xl my-auto"></i>
<div class="flex flex-col my-auto">
    <h2 class="text-2xl font-bold">${figure.name}</h2>
    <h3 class="text-xl font-bold">Od ${figure.manufacturer}</h3>
</div>
<h2 class="text-3xl ml-auto">${figure.price}Kč <i class="bi bi-cash text-green-700" ></i></h2>
</a>`
        : `<a href="/figure/${figure.id}" class="gap-2 flex flex-row min-h-16 w-[60%] min-w-64 border-primary border-2 rounded-lg mx-auto p-4 hover:bg-secondary duration-200 transition-colors cursor-pointer">
<img class="max-h-24" src="${figure.image}" alt="figure preview" />
<div class="flex flex-col my-auto">
    <h2 class="text-2xl font-bold">${figure.name}</h2>
    <h3 class="text-xl font-bold">Od ${figure.manufacturer}</h3>
</div>
<h2 class="text-3xl ml-auto">${figure.price}Kč <i class="bi bi-cash text-green-700" ></i></h2>
</a>`;
};
const searchText = async (text) => {
    if (text.length === 0) {
        result.innerHTML = '';
        return;
    }
    const searchParams = new URLSearchParams();
    const csrf = document.querySelector("input[name='csrfmiddlewaretoken']");
    searchParams.append('csrfmiddlewaretoken', csrf.value);
    searchParams.append('text', text);
    const request = await fetch('/search', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded'
        },
        body: searchParams.toString()
    });
    const json = (await request.json());
    let finalHtml = '';
    for (const user of json.users) {
        finalHtml += userHtml(user);
    }
    for (const figure of json.figures) {
        finalHtml += figureHtml(figure);
    }
    result.innerHTML = finalHtml;
};
//# sourceMappingURL=search.js.map