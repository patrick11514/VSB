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
    ),
    targets: z.array(
        z.object({
            id: z.number(),
            uuid: z.string(),
            targetValue: z.number(),
            name: z.string(),
            description: z.string(),
            date: z.number()
        })
    )
});

const defaultData = () => {
    return {
        balance: 0,
        records: [],
        targets: []
    };
};

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
            } catch (_) {}
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
        localStorage.setItem('banksData', JSON.stringify(this.banksData));
    }

    public getBanks() {
        return this.banks;
    }

    public getBankData(uuid: string) {
        const bank = this.banks.find((bank) => bank.uuid === uuid);
        if (!bank) return null;

        const bankData = this.banksData[uuid] ?? defaultData();

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
        const bankData = this.banksData[uuid] ?? defaultData();

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
        const bankData = this.banksData[uuid] ?? defaultData();

        bankData.balance += value;

        bankData.records.push({
            id: bankData.records.length,
            uuid,
            value,
            description,
            date: Date.now()
        });

        this.banksData[uuid] = bankData;

        this.syncStorage();
        this.emit('change');
    }

    public editRecord(uuid: string, id: number, value: number, description: string) {
        const bankData = this.banksData[uuid] ?? defaultData();

        const record = bankData.records.find((record) => record.id === id);
        if (!record) return;

        bankData.balance -= record.value;
        record.value = value;
        record.description = description;
        bankData.balance += value;

        this.banksData[uuid] = bankData;
        this.syncStorage();
        this.emit('change');
    }

    public removeRecord(uuid: string, id: number) {
        const bankData = this.banksData[uuid] ?? defaultData();

        const record = bankData.records.find((record) => record.id === id);
        if (!record) return;

        bankData.balance -= record.value;
        bankData.records = bankData.records.filter((record) => record.id !== id);

        this.banksData[uuid] = bankData;
        this.syncStorage();
        this.emit('change');
    }

    public getTargets(uuid: string) {
        const bankData = this.banksData[uuid] ?? defaultData();

        return bankData.targets.map((target) => {
            return {
                ...target,
                date: new Date(target.date)
            };
        });
    }

    public addTarget(uuid: string, targetValue: number, name: string, description: string) {
        const bankData = this.banksData[uuid] ?? defaultData();

        bankData.targets.push({
            id: bankData.targets.length,
            uuid,
            targetValue,
            name,
            description,
            date: Date.now()
        });

        this.banksData[uuid] = bankData;
        this.syncStorage();
        this.emit('change');
    }

    public editTarget(uuid: string, id: number, targetValue: number, name: string, description: string) {
        const bankData = this.banksData[uuid] ?? defaultData();

        const target = bankData.targets.find((target) => target.id === id);
        if (!target) return;

        target.targetValue = targetValue;
        target.name = name;
        target.description = description;

        this.banksData[uuid] = bankData;
        this.syncStorage();
        this.emit('change');
    }

    public removeTarget(uuid: string, id: number) {
        const bankData = this.banksData[uuid] ?? defaultData();

        bankData.targets = bankData.targets.filter((target) => target.id !== id);

        this.banksData[uuid] = bankData;
        this.syncStorage();
        this.emit('change');
    }

    public removeBank(uuid: string) {
        this.banks = this.banks.filter((bank) => bank.uuid !== uuid);
        this.syncStorage();
        this.emit('removeBank', uuid);
        this.emit('change');
    }

    public addBalance(uuid: string, value: number) {
        const bankData = this.banksData[uuid] ?? defaultData();
        bankData.balance += value;

        this.banksData[uuid] = bankData;
        this.syncStorage();
        this.emit('change');
    }

    public editBank(uuid: string, name: string) {
        const bank = this.banks.findIndex((bank) => bank.uuid === uuid);

        if (bank === -1) {
            return;
        }

        this.banks[bank].name = name;

        this.syncStorage();
        this.emit('change');
    }

    public exportBank(uuid: string) {
        const bankBaseData = this.banks.find((bank) => bank.uuid === uuid);
        if (!bankBaseData) {
            return null;
        }

        const bankData = this.banksData[uuid] ?? defaultData();

        return {
            ...bankBaseData,
            ...bankData
        };
    }

    public importBank(json: object): -1 | 0 | 1 {
        const schema = z.object({ ...bankSchema.shape, ...bankDataSchema.shape });

        const parsed = schema.safeParse(json);
        if (!parsed.success) {
            return -1;
        }

        const data = parsed.data;

        //if bank already exists
        if (this.banks.find((bank) => bank.uuid === data.uuid)) {
            return 0;
        }

        this.banks.push({
            name: data.name,
            password: data.password,
            uuid: data.uuid
        });

        this.banksData[data.uuid] = {
            balance: data.balance,
            records: data.records,
            targets: data.targets
        };

        this.syncStorage();
        this.emit('change');
        return 1;
    }
}

export const updateStoreDoSomething = <T>(store: Writable<T>, newValue: T, beforeUpdate: (current: T) => void) => {
    beforeUpdate(get(store));

    store.set(newValue);
};

export const formatDate = (date: Date, seconds = false, miliseconds = false) => {
    const day = date.getDate().toString().padStart(2, '0');
    const month = (date.getMonth() + 1).toString().padStart(2, '0');
    const year = date.getFullYear();

    const hours = date.getHours().toString().padStart(2, '0');
    const minutes = date.getMinutes().toString().padStart(2, '0');
    const secondsFormatted = date.getSeconds().toString().padStart(2, '0');
    const milisecondsFormatted = date.getMilliseconds().toString().padStart(3, '0');

    let baseString = `${day}.${month}.${year} ${hours}:${minutes}`;

    if (seconds) {
        baseString += `:${secondsFormatted}`;
    }

    if (miliseconds) {
        baseString += `:${milisecondsFormatted}`;
    }

    return baseString;
};
