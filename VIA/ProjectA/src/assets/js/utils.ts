const SwalAlert = async (data: import('sweetalert2').SweetAlertOptions) => {
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

const toDate = (date: Date | string) => {
    const d = new Date(date);
    const hours = d.getHours();
    const minutes = d.getMinutes().toString().padStart(2, '0');
    const seconds = d.getSeconds().toString().padStart(2, '0');
    const day = d.getDate();
    const month = d.getMonth() + 1;
    const year = d.getFullYear();

    return `${hours}:${minutes}:${seconds} ${day}.${month}.${year}`;
};

const firstUpper = (str: string) => {
    return str
        .split('')
        .map((ch, i) => (i == 0 ? ch.toUpperCase() : ch.toLowerCase()))
        .join('');
};

const sleep = (ms: number) => {
    return new Promise((resolve) => setTimeout(resolve, ms));
};
