"use strict";
const butt = document.querySelector('ion-button');
butt?.addEventListener('click', () => {
    const input = document.querySelector('ion-input');
    console.log(input?.value);
});
