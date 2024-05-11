import type { Bank } from '$/types/database';
import { bankSchema } from '$/types/types';
import { browser } from '$app/environment';
import EventEmitter from 'eventemitter3';
import { get, type Writable } from 'svelte/store';
import Swal, { type SweetAlertOptions } from 'sweetalert2';
import { z } from 'zod';

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

type LocalStorageManagerEvents = {
    addBank: (bank: Bank) => void;
    removeBank: (uuid: string) => void;
    change: () => void;
};

const bankDataSchema = z.object({
    balance: z.number(),
    records: z.array(
        z.object({
            id: z.number(),
            uuid: z.string(),
            value: z.number(),
            description: z.string(),
            date: z.number()
        })
    )
});

export class LocalStorageManager extends EventEmitter<LocalStorageManagerEvents> {
    private banks: Bank[] = [];
    private banksData: Record<string, z.infer<typeof bankDataSchema>> = {};

    constructor() {
        super();
        this.loadStorage();
    }

    private loadBanks() {
        const item = localStorage.getItem('banks');

        if (item) {
            try {
                const json = JSON.parse(item);
                const parsed = z.array(bankSchema).parse(json);

                this.banks = parsed;
                return;
            } catch (_) {}
        }

        localStorage.setItem('banks', JSON.stringify(this.banks));
    }

    private loadBankData() {
        const itemData = localStorage.getItem('banksData');

        if (itemData) {
            try {
                const json = JSON.parse(itemData);
                const parsed = z.record(z.string(), bankDataSchema).parse(json);

                this.banksData = parsed;
                return;
            } catch (_) {
                console.log(itemData);
                console.log(_);
            }
        }
        localStorage.setItem('banksData', JSON.stringify(this.banksData));
    }

    private loadStorage() {
        //banks list
        this.loadBanks();
        //banks data
        this.loadBankData();
    }

    private syncStorage() {
        localStorage.setItem('banks', JSON.stringify(this.banks));
    }

    public getBanks() {
        return this.banks;
    }

    public getBankData(uuid: string) {
        const bank = this.banks.find((bank) => bank.uuid === uuid);
        if (!bank) return null;

        const bankData = this.banksData[uuid] ?? {
            balance: 0,
            records: []
        };

        const revenues = bankData.records.reduce((acc, record) => {
            if (record.value <= 0) {
                return acc;
            }

            //if record is from another month, ignore it
            const currentDate = new Date();
            const date = new Date(record.date);

            if (currentDate.getMonth() !== date.getMonth() || currentDate.getFullYear() !== date.getFullYear()) {
                return acc;
            }

            return acc + record.value;
        }, 0);

        const expenses = bankData.records.reduce((acc, record) => {
            if (record.value >= 0) {
                return acc;
            }

            //if record is from another month, ignore it
            const currentDate = new Date();
            const date = new Date(record.date);

            if (currentDate.getMonth() !== date.getMonth() || currentDate.getFullYear() !== date.getFullYear()) {
                return acc;
            }

            return acc + record.value;
        }, 0);

        let data = {
            name: bank.name,
            uuid: bank.uuid,
            revenues,
            expenses,
            balance: bankData.balance
        };

        return data;
    }

    public getRecords(uuid: string) {
        const bankData = this.banksData[uuid] ?? {
            balance: 0,
            records: []
        };

        return bankData.records.map((record) => {
            return {
                ...record,
                date: new Date(record.date)
            };
        });
    }

    public addBank(bank: Bank) {
        this.banks.push(bank);
        this.syncStorage();
        this.emit('addBank', bank);
        this.emit('change');
    }

    public addRecord(uuid: string, value: number, description = '') {
        console.log('CALLED');

        const bankData = this.banksData[uuid] ?? {
            balance: 0,
            records: []
        };

        console.log(bankData);

        bankData.balance += value;

        bankData.records.push({
            id: bankData.records.length,
            uuid,
            value,
            description,
            date: Date.now()
        });

        console.log(bankData);

        this.banksData[uuid] = bankData;

        console.log(this.banksData);
        localStorage.setItem('banksData', JSON.stringify(this.banksData));
        this.emit('change');
    }

    public removeRecord(uuid: string, id: number) {
        const bankData = this.banksData[uuid] ?? {
            balance: 0,
            records: []
        };

        const record = bankData.records.find((record) => record.id === id);
        if (!record) return;

        bankData.balance -= record.value;
        bankData.records = bankData.records.filter((record) => record.id !== id);

        this.banksData[uuid] = bankData;
        localStorage.setItem('banksData', JSON.stringify(this.banksData));
        this.emit('change');
    }

    public removeBank(uuid: string) {
        this.banks = this.banks.filter((bank) => bank.uuid !== uuid);
        this.syncStorage();
        this.emit('removeBank', uuid);
        this.emit('change');
    }
}

export const updateStoreDoSomething = <T>(store: Writable<T>, newValue: T, beforeUpdate: (current: T) => void) => {
    beforeUpdate(get(store));

    store.set(newValue);
};
