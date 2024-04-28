import { browser } from '$app/environment';
import type { SweetAlertOptions } from 'sweetalert2';
import Swal from 'sweetalert2';

export const sleep = (ms: number) => {
    return new Promise((resolve) => setTimeout(resolve, ms));
};

export const SwalAlert = async (data: SweetAlertOptions) => {
    if (!browser) {
        return {
            isConfirmed: false
        };
    }

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

export const addItem = () => {
    /*TODO*/
};
export const editItem = (selected: number | null) => {
    /*TODO*/
};

export const removeItem = (selected: number | null) => {
    if (selected !== null) {
        //API CALL
    }
};
