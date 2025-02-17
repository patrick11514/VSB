import type { Response, ResponseWithData } from '$/types/response';
import { browser } from '$app/environment';
import type { ErrorApiResponse } from '@patrick115/sveltekitapi';
import Swal, { type SweetAlertOptions } from 'sweetalert2';

export const sleep = (ms: number) => {
    return new Promise((resolve) => setTimeout(resolve, ms));
};

export const SwalAlert = (data: SweetAlertOptions) => {
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

export const isValid = (response: unknown): response is Response | ResponseWithData<unknown> => {
    if (typeof response === 'object' && response && 'status' in response && response.status === true) return true;
    return false;
};
