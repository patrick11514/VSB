const JWT = import.meta.env.VITE_PINATA_JWT;

//eslint-disable-next-line
const pinataHTTP = async (url: string, method: string, body: any) => {
	const response = await fetch(url, {
		method,
		headers: {
			Authorization: `Bearer ${JWT}`
		},
		body
	});

	if (!response.ok) {
		const errText = await response.text();
		throw new Error(`Pinata API error: ${response.status} ${response.statusText} - ${errText}`);
	}

	return response;
};

export async function uploadFile(file: File): Promise<string> {
	const fd = new FormData();
	fd.append('file', file);

	const response = await pinataHTTP('https://api.pinata.cloud/pinning/pinFileToIPFS', 'POST', fd);
	const json = await response.json();
	return json.IpfsHash;
}

export async function getFile(cid: string): Promise<Uint8Array> {
	const response = await pinataHTTP(
		`https://pink-cheerful-anglerfish-366.mypinata.cloud/ipfs/${cid}`,
		'GET',
		undefined
	);
	const buffer = await response.arrayBuffer();
	return new Uint8Array(buffer);
}
