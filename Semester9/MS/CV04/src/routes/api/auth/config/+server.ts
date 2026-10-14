import { json } from '@sveltejs/kit';
import fs from 'node:fs';
import path from 'node:path';

export async function GET() {
	let clientId = process.env.GOOGLE_CLIENT_ID || '';
	const rootFolderId = process.env.GOOGLE_DRIVE_ROOT_FOLDER_ID || 'root';

	// If not directly set in env, check secrets.json at project root
	if (!clientId) {
		try {
			const secretsPath = path.resolve('secrets.json');
			if (fs.existsSync(secretsPath)) {
				const secrets = JSON.parse(fs.readFileSync(secretsPath, 'utf-8'));
				clientId = secrets.web?.client_id || secrets.installed?.client_id || '';
			}
		} catch (err) {
			console.error('Failed to read secrets.json:', err);
		}
	}

	return json({
		clientId,
		rootFolderId
	});
}
