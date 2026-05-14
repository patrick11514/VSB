import { ethers } from 'ethers';

export async function connectMetaMask(): Promise<ethers.BrowserProvider | null> {
	// @ts-expect-error - window.ethereum is injected by MetaMask
	if (typeof window === 'undefined' || !window.ethereum) return null;
	// @ts-expect-error - BrowserProvider constructor
	const provider = new ethers.BrowserProvider(window.ethereum);
	try {
		// @ts-expect-error - MetaMask request method
		await window.ethereum.request({ method: 'eth_requestAccounts' });
		if (!provider) {
			throw new Error('Failed to connect to MetaMask');
		}

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

export async function getCurrentAddress(provider: ethers.BrowserProvider): Promise<string | null> {
	try {
		const signer = await provider.getSigner();
		return await signer.getAddress();
	} catch (e) {
		console.error('Failed to get address', e);
		return null;
	}
}

export function isSepoliaNetwork(network: ethers.Network): boolean {
	return Number(network?.chainId) === 11155111; // Sepolia chain ID
}

export function disconnectWallet() {
	return true;
}

export function setupMetaMaskListeners(
	onAccountsChanged: (accounts: string[]) => void,
	onChainChanged: (chainId: string) => void
) {
	// @ts-expect-error - window.ethereum is injected by MetaMask
	if (typeof window === 'undefined' || !window.ethereum) return;
	// @ts-expect-error - MetaMask listener setup
	window.ethereum.on('accountsChanged', onAccountsChanged);
	// @ts-expect-error - MetaMask listener setup
	window.ethereum.on('chainChanged', onChainChanged);
}

export function removeMetaMaskListeners(
	onAccountsChanged: (accounts: string[]) => void,
	onChainChanged: (chainId: string) => void
) {
	// @ts-expect-error - window.ethereum is injected by MetaMask
	if (typeof window === 'undefined' || !window.ethereum) return;
	// @ts-expect-error - MetaMask listener removal
	window.ethereum.removeListener('accountsChanged', onAccountsChanged);
	// @ts-expect-error - MetaMask listener removal
	window.ethereum.removeListener('chainChanged', onChainChanged);
}
