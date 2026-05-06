import { writable } from 'svelte/store';

export const walletAddress = writable<string | null>(null);
export const provider = writable<any | null>(null);
export const network = writable<any | null>(null);
