import { z } from 'zod';
import type { Bank, Records, Targets } from './database';

export type UserData = {
    banks: Omit<Bank, 'name'>[];
};

export const bankSchema = z.object({
    uuid: z.string().length(36),
    name: z.string(),
    password: z.string()
});

export type BankRecord = Omit<Records, 'id'> & {
    id: number;
};

export type BankTarget = Omit<Targets, 'id'> & {
    id: number;
};

export type BankWithoutHash = Omit<Bank, 'password'> & {
    balance: number;
    revenues: number;
    expenses: number;
};

export type Response = {
    status: true;
};

export type ResponseWithData<T> = Response & {
    data: T;
};
