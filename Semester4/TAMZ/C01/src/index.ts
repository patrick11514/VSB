const butt = document.querySelector<HTMLButtonElement>('ion-button')

butt?.addEventListener('click', () => {
    const input = document.querySelector<HTMLInputElement>('ion-input')
    console.log(input?.value)
})
