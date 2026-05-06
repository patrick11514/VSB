import { ethers } from 'ethers';

export async function connectMetaMask(): Promise<ethers.BrowserProvider | null> {
	// @ts-ignore
	if (typeof window === 'undefined' || !window.ethereum) return null;
	// @ts-ignore
	const provider = new ethers.BrowserProvider(window.ethereum);
	try {
		// request accounts
		// @ts-ignore
		await window.ethereum.request({ method: 'eth_requestAccounts' });
		return provider;
	} catch (e) {
		console.error('MetaMask connection rejected', e);
		return null;
	}
}

export function getSigner(provider: ethers.BrowserProvider) {
	return provider.getSigner();
}

export function formatEth(value: ethers.BigNumberish) {
	return ethers.formatEther(value);
}

export function parseEth(amount: string) {
	return ethers.parseEther(amount);
}

export async function getNetwork(provider: ethers.BrowserProvider) {
	return provider.getNetwork();
}
