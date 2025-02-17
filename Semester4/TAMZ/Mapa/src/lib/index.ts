import { browser } from '$app/environment';
import Swal, { type SweetAlertOptions } from 'sweetalert2';

export const getISO = (date: Date) => {
    const tzoffset = date.getTimezoneOffset() * 60000; //offset in milliseconds
    const localISOTime = new Date(date.getTime() - tzoffset).toISOString().slice(0, -1);
    return localISOTime;
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

export const getCurrentLocation = async () => {
    if (!browser) {
        return null;
    }

    return new Promise<GeolocationPosition>((resolve) => {
        navigator.geolocation.getCurrentPosition(resolve);
    });
};

export const degToRad = (deg: number) => deg * (Math.PI / 180);
export const radToDeg = (rad: number) => rad * (180 / Math.PI);
