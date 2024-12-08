const getByStep = (selects: HTMLElement[], step: number | string) => {
    return selects.find((sel) => sel.dataset.step === step.toString());
};

//very bad idea, but we need to somehow check password :)
const PASSWORD = 'test123';

document.addEventListener('included', () => {
    if (localStorage.getItem('logged')) {
        window.location.replace('/account');
        return;
    }

    const selects = Array.from(document.querySelectorAll<HTMLElement>('section[data-step]'));

    //CHECK EMAIL
    const emailSec = getByStep(selects, 1)!;
    const email = emailSec.querySelector<HTMLInputElement>('input')!;
    emailSec.querySelector('button')!.addEventListener('click', () => {
        const text = email.value;
        //just simple validation
        if (!new RegExp(/[a-zA-Z0-9.-]*@(([a-zA-Z0-9]*.)*)([a-zA-Z]*)/).test(text)) {
            email.setCustomValidity('Invalid email');
            return;
        }
        email.setCustomValidity('');

        //go to next step
        const isNext = getByStep(selects, 2);
        if (!isNext)
            //no step avaliable
            return;

        //put email
        isNext.querySelector('span')!.textContent = email.value;

        emailSec.classList.add('invisible');
        isNext.classList.remove('invisible');
    });

    //check password
    const passSec = getByStep(selects, 2)!;
    const password = passSec.querySelector<HTMLInputElement>('input')!;
    passSec.querySelector('button')!.addEventListener('click', () => {
        const text = password.value;
        if (text != PASSWORD) {
            password.setCustomValidity('Invalid password');
            return;
        }
        password.setCustomValidity('');

        localStorage.setItem('logged', 'true');

        window.location.replace('/account');
    });
});
