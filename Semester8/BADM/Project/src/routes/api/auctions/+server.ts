import type { RequestHandler } from '@sveltejs/kit';
import { promises as fs } from 'fs';
import { join } from 'path';

const DATA_DIR = process.cwd();
const DB_PATH = join(DATA_DIR, 'data', 'auctions.json');

async function ensureDb() {
	try {
		await fs.mkdir(join(DATA_DIR, 'data'), { recursive: true });
		await fs.access(DB_PATH);
	} catch (e) {
		await fs.writeFile(DB_PATH, JSON.stringify([]), 'utf-8');
	}
}

export const GET: RequestHandler = async () => {
	await ensureDb();
	try {
		const raw = await fs.readFile(DB_PATH, 'utf-8');
		const list = JSON.parse(raw || '[]');
		return new Response(JSON.stringify(list), { status: 200 });
	} catch (err) {
		return new Response(JSON.stringify([]), { status: 200 });
	}
};
