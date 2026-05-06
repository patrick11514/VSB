import type { ethers } from 'ethers';
import { writable } from 'svelte/store';

export const walletAddress = writable<string | null>(null);
export const provider = writable<ethers.BrowserProvider | null>(null);
export const network = writable<ethers.Network | null>(null);
export const isAuthenticated = writable<boolean>(false);
export const isLoading = writable<boolean>(false);
export const error = writable<string | null>(null);

export function resetAuth() {
	walletAddress.set(null);
	provider.set(null);
	network.set(null);
	isAuthenticated.set(false);
	error.set(null);
}
