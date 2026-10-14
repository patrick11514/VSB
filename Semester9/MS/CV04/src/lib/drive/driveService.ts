import type { DriveFile } from './types.js';

export const FOLDER_MIME_TYPE = 'application/vnd.google-apps.folder';

export async function listDriveFiles(accessToken: string, folderId: string): Promise<DriveFile[]> {
	const sanitizedFolderId = folderId.replace(/'/g, "\\'");
	const query = `'${sanitizedFolderId}' in parents and trashed = false`;

	const fields =
		'files(id, name, mimeType, size, modifiedTime, webViewLink, iconLink, thumbnailLink, parents)';
	const orderBy = 'folder,name';

	const params = new URLSearchParams({
		q: query,
		fields: fields,
		orderBy: orderBy,
		pageSize: '1000'
	});

	const response = await fetch(`https://www.googleapis.com/drive/v3/files?${params.toString()}`, {
		headers: {
			Authorization: `Bearer ${accessToken}`,
			Accept: 'application/json'
		}
	});

	if (!response.ok) {
		const errorData = await response.json().catch(() => null);
		const errorMsg =
			errorData?.error?.message ||
			`Google Drive API error: ${response.status} ${response.statusText}`;
		throw new Error(errorMsg);
	}

	const data = await response.json();
	return (data.files || []) as DriveFile[];
}

export async function getDriveFileMetadata(
	accessToken: string,
	fileId: string
): Promise<DriveFile> {
	const fields = 'id, name, mimeType, size, modifiedTime, webViewLink, parents';
	const response = await fetch(
		`https://www.googleapis.com/drive/v3/files/${fileId}?fields=${fields}`,
		{
			headers: {
				Authorization: `Bearer ${accessToken}`,
				Accept: 'application/json'
			}
		}
	);

	if (!response.ok) {
		throw new Error(`Failed to get file metadata: ${response.statusText}`);
	}

	return (await response.json()) as DriveFile;
}

export async function downloadDriveFileBlob(accessToken: string, fileId: string): Promise<Blob> {
	const response = await fetch(`https://www.googleapis.com/drive/v3/files/${fileId}?alt=media`, {
		headers: {
			Authorization: `Bearer ${accessToken}`
		}
	});

	if (!response.ok) {
		throw new Error(`Failed to download file: ${response.statusText}`);
	}

	return await response.blob();
}

export async function downloadDriveFileText(accessToken: string, fileId: string): Promise<string> {
	const blob = await downloadDriveFileBlob(accessToken, fileId);
	return await blob.text();
}

export async function uploadDriveFile(
	accessToken: string,
	folderId: string,
	name: string,
	blob: Blob
): Promise<DriveFile> {
	const metadata = {
		name,
		parents: [folderId]
	};

	const form = new FormData();
	form.append('metadata', new Blob([JSON.stringify(metadata)], { type: 'application/json' }));
	form.append('file', blob);

	const response = await fetch(
		'https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart&fields=id,name,mimeType,size,modifiedTime,parents',
		{
			method: 'POST',
			headers: {
				Authorization: `Bearer ${accessToken}`
			},
			body: form
		}
	);

	if (!response.ok) {
		const errorData = await response.json().catch(() => null);
		throw new Error(errorData?.error?.message || `Upload failed: ${response.statusText}`);
	}

	return (await response.json()) as DriveFile;
}

export async function updateDriveFileContent(
	accessToken: string,
	fileId: string,
	blob: Blob
): Promise<DriveFile> {
	const response = await fetch(
		`https://www.googleapis.com/upload/drive/v3/files/${fileId}?uploadType=media&fields=id,name,mimeType,size,modifiedTime`,
		{
			method: 'PATCH',
			headers: {
				Authorization: `Bearer ${accessToken}`,
				'Content-Type': blob.type || 'application/octet-stream'
			},
			body: blob
		}
	);

	if (!response.ok) {
		const errorData = await response.json().catch(() => null);
		throw new Error(errorData?.error?.message || `Update failed: ${response.statusText}`);
	}

	return (await response.json()) as DriveFile;
}

export async function renameDriveFile(
	accessToken: string,
	fileId: string,
	newName: string
): Promise<DriveFile> {
	const response = await fetch(`https://www.googleapis.com/drive/v3/files/${fileId}`, {
		method: 'PATCH',
		headers: {
			Authorization: `Bearer ${accessToken}`,
			'Content-Type': 'application/json'
		},
		body: JSON.stringify({ name: newName })
	});

	if (!response.ok) {
		throw new Error(`Failed to rename: ${response.statusText}`);
	}

	return (await response.json()) as DriveFile;
}

export async function deleteDriveFile(accessToken: string, fileId: string): Promise<void> {
	const response = await fetch(`https://www.googleapis.com/drive/v3/files/${fileId}`, {
		method: 'DELETE',
		headers: {
			Authorization: `Bearer ${accessToken}`
		}
	});

	if (!response.ok) {
		throw new Error(`Failed to delete: ${response.statusText}`);
	}
}

export async function copyDriveFile(
	accessToken: string,
	fileId: string,
	destinationFolderId: string,
	newName?: string
): Promise<DriveFile> {
	const body: { parents: string[]; name?: string } = {
		parents: [destinationFolderId]
	};
	if (newName) body.name = newName;

	const response = await fetch(
		`https://www.googleapis.com/drive/v3/files/${fileId}/copy?fields=id,name,mimeType,size,modifiedTime,parents`,
		{
			method: 'POST',
			headers: {
				Authorization: `Bearer ${accessToken}`,
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(body)
		}
	);

	if (!response.ok) {
		throw new Error(`Failed to copy: ${response.statusText}`);
	}

	return (await response.json()) as DriveFile;
}

export async function moveDriveFile(
	accessToken: string,
	fileId: string,
	destinationFolderId: string,
	currentParents: string[] = []
): Promise<DriveFile> {
	const removeParents = currentParents.join(',');
	const params = new URLSearchParams({
		addParents: destinationFolderId,
		removeParents: removeParents,
		fields: 'id,name,mimeType,size,modifiedTime,parents'
	});

	const response = await fetch(
		`https://www.googleapis.com/drive/v3/files/${fileId}?${params.toString()}`,
		{
			method: 'PATCH',
			headers: {
				Authorization: `Bearer ${accessToken}`
			}
		}
	);

	if (!response.ok) {
		throw new Error(`Failed to move: ${response.statusText}`);
	}

	return (await response.json()) as DriveFile;
}

export async function createDriveFolder(
	accessToken: string,
	parentFolderId: string,
	folderName: string
): Promise<DriveFile> {
	const metadata = {
		name: folderName,
		mimeType: FOLDER_MIME_TYPE,
		parents: [parentFolderId]
	};

	const response = await fetch(
		'https://www.googleapis.com/drive/v3/files?fields=id,name,mimeType,size,modifiedTime,parents',
		{
			method: 'POST',
			headers: {
				Authorization: `Bearer ${accessToken}`,
				'Content-Type': 'application/json'
			},
			body: JSON.stringify(metadata)
		}
	);

	if (!response.ok) {
		throw new Error(`Failed to create folder: ${response.statusText}`);
	}

	return (await response.json()) as DriveFile;
}

export function isTextEditable(file: DriveFile): boolean {
	if (isFolder(file)) return false;
	const mime = (file.mimeType || '').toLowerCase();
	const name = (file.name || '').toLowerCase();

	if (
		mime.startsWith('text/') ||
		mime.includes('json') ||
		mime.includes('yaml') ||
		mime.includes('javascript') ||
		mime.includes('typescript') ||
		mime.includes('xml') ||
		mime.includes('csv') ||
		mime.includes('markdown')
	) {
		return true;
	}

	const textExtensions = [
		'.txt',
		'.md',
		'.json',
		'.yaml',
		'.yml',
		'.js',
		'.ts',
		'.jsx',
		'.tsx',
		'.svelte',
		'.html',
		'.css',
		'.scss',
		'.c',
		'.cpp',
		'.h',
		'.hpp',
		'.rs',
		'.py',
		'.java',
		'.sh',
		'.bash',
		'.xml',
		'.csv',
		'.log',
		'.env',
		'.toml',
		'.ini',
		'.sql'
	];

	return textExtensions.some((ext) => name.endsWith(ext));
}

export function formatFileSize(bytes?: string | number): string {
	if (!bytes) return '-';
	const num = typeof bytes === 'string' ? parseInt(bytes, 10) : bytes;
	if (isNaN(num)) return '-';
	if (num === 0) return '0 B';

	const k = 1024;
	const sizes = ['B', 'KB', 'MB', 'GB', 'TB'];
	const i = Math.floor(Math.log(num) / Math.log(k));

	return `${parseFloat((num / Math.pow(k, i)).toFixed(1))} ${sizes[i]}`;
}

export function formatDate(dateStr?: string): string {
	if (!dateStr) return '-';
	try {
		const d = new Date(dateStr);
		return d.toLocaleDateString(undefined, {
			year: 'numeric',
			month: 'short',
			day: 'numeric',
			hour: '2-digit',
			minute: '2-digit'
		});
	} catch {
		return dateStr;
	}
}

export function isFolder(file: DriveFile): boolean {
	return file.mimeType === FOLDER_MIME_TYPE;
}
