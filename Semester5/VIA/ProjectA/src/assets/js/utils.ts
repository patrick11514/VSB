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

const firstUpper = (str: string) => {
    return str
        .split('')
        .map((ch, i) => (i == 0 ? ch.toUpperCase() : ch.toLowerCase()))
        .join('');
};

const sleep = (ms: number) => {
    return new Promise((resolve) => setTimeout(resolve, ms));
};

const timeDiff = (date: Date | number) => {
    const d = new Date(date);
    const diff = Date.now() - d.getTime();

    //check seconds
    const seconds = diff / 1000;
    if (seconds / 60 < 1) {
        return `${seconds.toFixed(0)} seconds ago`;
    }

    const minutes = seconds / 60;
    //check minutes
    if (minutes / 60 < 1) {
        return `${minutes.toFixed(0)} minutes ago`;
    }

    const hours = minutes / 60;
    if (hours / 24 < 1) {
        return `${hours.toFixed(0)} hours ago`;
    }

    const days = hours / 24;
    if (days / 7 < 1) {
        return `${days.toFixed(0)} days ago`;
    }

    const weeks = days / 7;
    if (days / 30 < 1) {
        return `${weeks.toFixed(0)} weeks ago`;
    }

    return `${(days / 30).toFixed(0)} months ago`;
};
