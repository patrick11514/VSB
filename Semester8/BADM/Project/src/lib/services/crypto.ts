// Crypto utilities using Web Crypto API (browser)

function toHex(buffer: ArrayBuffer) {
	return Array.from(new Uint8Array(buffer))
		.map((b) => b.toString(16).padStart(2, '0'))
		.join('');
}

export async function sha256Hex(input: string | ArrayBuffer) {
	const data =
		typeof input === 'string'
			? new TextEncoder().encode(input)
			: new Uint8Array(input as ArrayBuffer);
	const hash = await crypto.subtle.digest('SHA-256', data);
	return toHex(hash);
}

export async function deriveKey(passphrase: string, salt?: Uint8Array) {
	const enc = new TextEncoder();
	const passKey = await crypto.subtle.importKey(
		'raw',
		enc.encode(passphrase),
		{ name: 'PBKDF2' },
		false,
		['deriveKey']
	);
	const s = salt ?? crypto.getRandomValues(new Uint8Array(16));
	const key = await (crypto.subtle.deriveKey as unknown as typeof crypto.subtle.deriveKey)(
		{ name: 'PBKDF2', salt: s.buffer as ArrayBuffer, iterations: 100000, hash: 'SHA-256' },
		passKey,
		{ name: 'AES-GCM', length: 256 },
		true,
		['encrypt', 'decrypt']
	);
	return { key, salt: s } as const;
}

export async function encryptWithPassphrase(plaintext: ArrayBuffer, passphrase: string) {
	const { key, salt } = await deriveKey(passphrase);
	const iv = crypto.getRandomValues(new Uint8Array(12));
	const cipher = await crypto.subtle.encrypt({ name: 'AES-GCM', iv }, key, plaintext);
	return { cipher: new Uint8Array(cipher), iv, salt };
}

export async function decryptWithPassphrase(
	cipher: ArrayBuffer,
	passphrase: string,
	iv: Uint8Array,
	salt: Uint8Array
) {
	const enc = new TextEncoder();
	const passKey = await crypto.subtle.importKey(
		'raw',
		enc.encode(passphrase),
		{ name: 'PBKDF2' },
		false,
		['deriveKey']
	);
	const key = await (crypto.subtle.deriveKey as unknown as typeof crypto.subtle.deriveKey)(
		{ name: 'PBKDF2', salt: salt.buffer as ArrayBuffer, iterations: 100000, hash: 'SHA-256' },
		passKey,
		{ name: 'AES-GCM', length: 256 },
		true,
		['decrypt']
	);
	const decrypted = await (crypto.subtle.decrypt as unknown as typeof crypto.subtle.decrypt)(
		{ name: 'AES-GCM', iv: iv as BufferSource },
		key,
		cipher as BufferSource
	);
	return new Uint8Array(decrypted);
}

export function generateRandomPassphrase(length = 32) {
	const arr = new Uint8Array(length);
	crypto.getRandomValues(arr);
	return Array.from(arr)
		.map((b) => b.toString(16).padStart(2, '0'))
		.join('');
}
