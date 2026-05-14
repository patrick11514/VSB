const JWT = import.meta.env.VITE_PINATA_JWT;
const PINATA_API = import.meta.env.VITE_PINATA_API_URL || 'https://api.pinata.cloud';
const IPFS_GATEWAY_BASE = (
	import.meta.env.VITE_IPFS_GATEWAY_BASE || 'https://pink-cheerful-anglerfish-366.mypinata.cloud'
).replace(/\/$/, '');

//eslint-disable-next-line @typescript-eslint/no-explicit-any
async function pinataHTTP(url: string, method: string, body: any) {
	const headers: Record<string, string> = {};
	if (JWT) headers['Authorization'] = `Bearer ${JWT}`;

	const response = await fetch(url, {
		method,
		headers,
		body
	});

	if (!response.ok) {
		const errText = await response.text();
		throw new Error(`Pinata API error: ${response.status} ${response.statusText} - ${errText}`);
	}

	return response;
}

export async function uploadFile(file: File): Promise<string> {
	const fd = new FormData();
	fd.append('file', file);

	const url = `${PINATA_API.replace(/\/$/, '')}/pinning/pinFileToIPFS`;
	const response = await pinataHTTP(url, 'POST', fd);
	const json = await response.json();
	return json.IpfsHash;
}

export function ipfsToGatewayUrl(handleOrCid: string) {
	// Accept formats: ipfs://CID or CID or /ipfs/CID
	if (!handleOrCid) return '';
	let cid = handleOrCid;
	if (cid.startsWith('ipfs://')) cid = cid.replace('ipfs://', '');
	if (cid.startsWith('/ipfs/')) cid = cid.replace('/ipfs/', '');
	return `${IPFS_GATEWAY_BASE}/ipfs/${cid}`;
}

export async function getFile(cid: string): Promise<Uint8Array> {
	const url = ipfsToGatewayUrl(cid);
	const response = await fetch(url);
	if (!response.ok) throw new Error(`Gateway error: ${response.status} ${response.statusText}`);
	const buffer = await response.arrayBuffer();
	return new Uint8Array(buffer);
}
