import { browser } from '$app/environment';
import { ethers } from 'ethers';

export const AUCTION_ABI = [
	'constructor(uint256 _minBid, string _name, string _description, bytes32 _originalFileHash, bytes32 _hashedFileHash, string _ipfsHandle, bytes32 _passphraseHash, uint256 _endAt)',
	'function isOwner() view returns (bool)',
	'function isWinner() view returns (bool)',
	'function canRefund() view returns (uint256)',
	'function canEndAuction() view returns (bool)',
	'function checkStatus() view returns (string, string, address, uint256, uint256, uint8, string, bytes32, bytes32)',
	'function bid() payable',
	'function endAuction()',
	'function revealPassphrase(string passphrase)',
	'function givePassphrase() view returns (string ipfsHandle, string revealedPassphrase, bytes32 originalFileHash, bytes32 passphraseHash)',
	'function refund()'
] as const;

const AUCTION_BYTECODE = import.meta.env.VITE_AUCTION_BYTECODE?.trim() ?? '';

if (!browser) {
	import('node:crypto').then((crypto) => {
		crypto.createHash('sha256').update(AUCTION_BYTECODE).digest('hex');
	});
}

export type AuctionStateLabel = 'Bidding' | 'WaitingPassphrase' | 'Completed';

export interface AuctionDetails {
	address: string;
	owner: string;
	name: string;
	description: string;
	ipfsHandle: string;
	originalFileHash?: string;
	hashedFileHash: string;
	endAt: bigint;
	maxBid: bigint;
	state: number;
}

export interface AuctionRevealDetails {
	ipfsHandle: string;
	revealedPassphrase: string;
	originalFileHash: string;
	passphraseHash: string;
}

export function getAuctionContract(address: string, runner: ethers.ContractRunner) {
	return new ethers.Contract(address, AUCTION_ABI, runner);
}

export function isValidAuctionAddress(address: string) {
	return ethers.isAddress(address);
}

export function formatAuctionState(state: number): AuctionStateLabel {
	if (state === 1) {
		return 'WaitingPassphrase';
	}

	if (state === 2) {
		return 'Completed';
	}

	return 'Bidding';
}

export async function readAuction(
	address: string,
	runner: ethers.ContractRunner
): Promise<AuctionDetails> {
	const contract = getAuctionContract(address, runner);
	const [
		name,
		description,
		owner,
		endAt,
		maxBid,
		state,
		ipfsHandle,
		originalFileHash,
		hashedFileHash
	] = await contract.checkStatus();

	return {
		address,
		owner,
		name,
		description,
		ipfsHandle,
		originalFileHash: originalFileHash ?? '',
		hashedFileHash,
		endAt: BigInt(endAt),
		maxBid: BigInt(maxBid),
		state: Number(state)
	};
}

export async function getAuctionPermissions(address: string, runner: ethers.ContractRunner) {
	const contract = getAuctionContract(address, runner);
	const [isOwner, isWinner, refundAmount, canEndAuction] = await Promise.all([
		contract.isOwner(),
		contract.isWinner(),
		contract.canRefund(),
		contract.canEndAuction()
	]);

	return {
		isOwner: Boolean(isOwner),
		isWinner: Boolean(isWinner),
		canRefund: BigInt(refundAmount) > 0n,
		canEndAuction: Boolean(canEndAuction),
		refundAmount: BigInt(refundAmount ?? 0)
	};
}

export async function getWinnerFileDetails(
	address: string,
	runner: ethers.ContractRunner
): Promise<AuctionRevealDetails> {
	const contract = getAuctionContract(address, runner);
	const [ipfsHandle, revealedPassphrase, originalFileHash, passphraseHash] =
		await contract.givePassphrase();

	return {
		ipfsHandle,
		revealedPassphrase,
		originalFileHash,
		passphraseHash
	};
}

export async function deployAuctionContract(
	signer: ethers.Signer,
	params: {
		minBid: bigint;
		name: string;
		description: string;
		originalFileHash: string;
		hashedFileHash: string;
		ipfsHandle: string;
		passphraseHash: string;
		endAt: bigint;
	}
) {
	if (!AUCTION_BYTECODE) {
		throw new Error(
			'Missing VITE_AUCTION_BYTECODE. Add compiled auction bytecode to deploy from the UI.'
		);
	}

	const factory = new ethers.ContractFactory(AUCTION_ABI, AUCTION_BYTECODE, signer);
	const contract = await factory.deploy(
		params.minBid,
		params.name,
		params.description,
		params.originalFileHash,
		params.hashedFileHash,
		params.ipfsHandle,
		params.passphraseHash,
		params.endAt
	);

	await contract.waitForDeployment();
	return contract.getAddress();
}

export async function bidOnAuction(address: string, signer: ethers.Signer, amount: bigint) {
	const contract = getAuctionContract(address, signer);
	const tx = await contract.bid({ value: amount });
	await tx.wait();
}

export async function endAuction(address: string, signer: ethers.Signer) {
	const contract = getAuctionContract(address, signer);
	const tx = await contract.endAuction();
	await tx.wait();
}

export async function revealPassphrase(address: string, signer: ethers.Signer, passphrase: string) {
	const contract = getAuctionContract(address, signer);
	const tx = await contract.revealPassphrase(passphrase);
	await tx.wait();
}

export async function refundAuction(address: string, signer: ethers.Signer) {
	const contract = getAuctionContract(address, signer);
	const tx = await contract.refund();
	await tx.wait();
}
