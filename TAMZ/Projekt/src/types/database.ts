import type { ColumnType } from "kysely";

export type Generated<T> = T extends ColumnType<infer S, infer I, infer U>
  ? ColumnType<S, I | undefined, U>
  : ColumnType<T, T | undefined, T>;

export interface Balance {
  uuid: string;
  value: number;
}

export interface Bank {
  name: string;
  password: string;
  uuid: string;
}

export interface Records {
  date: Date;
  description: string;
  id: Generated<number>;
  uuid: string;
  value: number;
}

export interface DB {
  balance: Balance;
  bank: Bank;
  records: Records;
}
