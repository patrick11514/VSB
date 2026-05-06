import type { IPFSHTTPClient } from 'ipfs-http-client';
import { create } from 'ipfs-http-client';

const IPFS_URL = import.meta.env.VITE_IPFS_URL || 'https://ipfs.io';

let client: IPFSHTTPClient | null = null;

function getClient() {
	if (client) return client;
	// Use ipfs-http-client with a configurable URL
	client = create({ url: IPFS_URL });
	return client;
}

export async function uploadFile(file: File): Promise<string> {
	const c = getClient();
	const added = await c.add(file);
	return added.cid.toString();
}

export async function getFile(cid: string): Promise<Uint8Array> {
	const c = getClient();
	const stream = c.cat(cid);
	const chunks: Uint8Array[] = [];
	for await (const chunk of stream) {
		chunks.push(chunk as Uint8Array);
	}
	// concat
	const length = chunks.reduce((sum, c) => sum + c.length, 0);
	const result = new Uint8Array(length);
	let offset = 0;
	for (const chunk of chunks) {
		result.set(chunk, offset);
		offset += chunk.length;
	}
	return result;
}
