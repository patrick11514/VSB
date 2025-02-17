const input = document.querySelector('input');
const ul = document.querySelector('ul');

input?.addEventListener('keydown', (e) => {
    if (e.key !== 'Enter') {
        return;
    }

    const li = document.createElement('li');
    const span = document.createElement('span');
    span.innerText = input.value;
    li.appendChild(span);

    const button = document.createElement('button');
    button.innerText = 'X';
    button.addEventListener('click', removeTask);
    li.append(button);

    ul?.appendChild(li);

    input.value = '';
});

const removeTask = (ev) => {
    /**
     * @type {HTMLButtonElement}
     */
    const target = ev.currentTarget;
    const li = target.parentElement;
    li.remove();
};
