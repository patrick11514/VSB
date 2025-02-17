import { creating, itemPopupCancel, itemPopupClose, itemPopupData, itemPopupOpen, items, modalAfterConfirm, modalIsOpen, selected } from '$/components/Store.svelte';
import { browser } from '$app/environment';
import { get } from 'svelte/store';
import type { SweetAlertOptions } from 'sweetalert2';
import Swal from 'sweetalert2';
import { API } from './api';

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

export const addItem = async () => {
    return new Promise<void>((resolve) => {
        creating.set(true);
        //reset data
        itemPopupData.set(undefined);

        //open dialog
        itemPopupOpen.set(true);

        //set functions
        itemPopupClose.set(async () => {
            const item = get(itemPopupData);
            if (!item) {
                return;
            }

            items.set(await API.data.POST(item));

            itemPopupOpen.set(false);
            resolve();
        });
        itemPopupCancel.set(() => {
            itemPopupOpen.set(false);
            resolve();
        });
    });
};
export const editItem = (selected: number | null) => {
    if (selected === null) {
        return;
    }
    return new Promise<void>((resolve) => {
        creating.set(false);
        //reset data
        itemPopupData.set(get(items)[selected]);

        //open dialog
        itemPopupOpen.set(true);

        //set functions
        itemPopupClose.set(async () => {
            const item = get(itemPopupData);
            if (!item) {
                return;
            }

            items.set(
                await API.data.PATCH({
                    index: selected,
                    newData: item
                })
            );

            itemPopupOpen.set(false);
            resolve();
        });
        itemPopupCancel.set(() => {
            itemPopupOpen.set(false);
            resolve();
        });
    });
};

export const removeItem = (sel: number | null) => {
    if (sel === null) {
        return;
    }

    modalAfterConfirm.set(async () => {
        const result = await API.data.DELETE(sel);
        items.set(result);
        modalIsOpen.set(false);
        selected.set(null);
    });
    modalIsOpen.set(true);
};

export const cleanFiles = (files: (File | null)[]) => {
    return files.filter((file) => file !== null) as File[];
};

//https://stackoverflow.com/a/9458996
export const arrayBufferToBase64 = (buffer: ArrayBuffer) => {
    var binary = '';
    var bytes = new Uint8Array(buffer);
    var len = bytes.byteLength;
    for (var i = 0; i < len; i++) {
        binary += String.fromCharCode(bytes[i]);
    }
    return window.btoa(binary);
};
