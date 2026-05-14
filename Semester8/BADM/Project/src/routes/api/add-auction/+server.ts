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

export const POST: RequestHandler = async ({ request }) => {
	await ensureDb();
	try {
		const payload = await request.json();
		const raw = await fs.readFile(DB_PATH, 'utf-8');
		const list = JSON.parse(raw || '[]');
		list.push({ ...payload, createdAt: new Date().toISOString() });
		await fs.writeFile(DB_PATH, JSON.stringify(list, null, 2), 'utf-8');
		return new Response(JSON.stringify({ ok: true }), { status: 201 });
	} catch (err) {
		return new Response(JSON.stringify({ ok: false, error: String(err) }), { status: 500 });
	}
};

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
