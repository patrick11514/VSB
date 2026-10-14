import { openDB, type IDBPDatabase } from 'idb';
import type { CachedFileItem, PendingUpload, PendingOperation, DriveFile } from './types.js';

const DB_NAME = 'drowser_offline_db';
const DB_VERSION = 2;

let dbPromise: Promise<IDBPDatabase> | null = null;

function getDB(): Promise<IDBPDatabase> {
	if (typeof window === 'undefined') {
		return Promise.reject(new Error('IndexedDB not available in SSR'));
	}
	if (!dbPromise) {
		dbPromise = openDB(DB_NAME, DB_VERSION, {
			upgrade(db) {
				if (!db.objectStoreNames.contains('cached_files')) {
					db.createObjectStore('cached_files', { keyPath: 'id' });
				}
				if (!db.objectStoreNames.contains('cached_folder_listings')) {
					db.createObjectStore('cached_folder_listings', { keyPath: 'folderId' });
				}
				if (!db.objectStoreNames.contains('pending_uploads')) {
					db.createObjectStore('pending_uploads', { keyPath: 'id' });
				}
				if (!db.objectStoreNames.contains('pending_operations')) {
					db.createObjectStore('pending_operations', { keyPath: 'id' });
				}
			}
		});
	}
	return dbPromise;
}

function cleanClone<T>(val: T): T {
	if (val === undefined || val === null) return val;
	try {
		return structuredClone(val);
	} catch {
		return JSON.parse(JSON.stringify(val));
	}
}

export async function saveCachedFile(item: CachedFileItem): Promise<void> {
	const db = await getDB();
	await db.put('cached_files', {
		...cleanClone({
			id: item.id,
			name: item.name,
			mimeType: item.mimeType,
			size: item.size,
			modifiedTime: item.modifiedTime,
			cachedAt: item.cachedAt,
			isDirty: item.isDirty,
			baseServerModifiedTime: item.baseServerModifiedTime
		}),
		data: item.data // Blob is structured-cloneable natively
	});
}

export async function getCachedFile(id: string): Promise<CachedFileItem | undefined> {
	const db = await getDB();
	return db.get('cached_files', id);
}

export async function getAllCachedFiles(): Promise<CachedFileItem[]> {
	const db = await getDB();
	return db.getAll('cached_files');
}

export async function removeCachedFile(id: string): Promise<void> {
	const db = await getDB();
	await db.delete('cached_files', id);
}

export async function saveFolderListing(folderId: string, files: DriveFile[]): Promise<void> {
	const db = await getDB();
	const sanitizedFiles = cleanClone(files);
	await db.put('cached_folder_listings', { folderId, files: sanitizedFiles, updatedAt: Date.now() });
}

export async function getFolderListing(folderId: string): Promise<DriveFile[] | undefined> {
	const db = await getDB();
	const entry = await db.get('cached_folder_listings', folderId);
	return entry ? entry.files : undefined;
}

export async function savePendingUpload(upload: PendingUpload): Promise<void> {
	const db = await getDB();
	await db.put('pending_uploads', {
		...cleanClone({
			id: upload.id,
			name: upload.name,
			mimeType: upload.mimeType,
			folderId: upload.folderId,
			createdAt: upload.createdAt,
			clientModifiedTime: upload.clientModifiedTime
		}),
		data: upload.data
	});
}

export async function getAllPendingUploads(): Promise<PendingUpload[]> {
	const db = await getDB();
	return db.getAll('pending_uploads');
}

export async function removePendingUpload(id: string): Promise<void> {
	const db = await getDB();
	await db.delete('pending_uploads', id);
}

export async function savePendingOperation(op: PendingOperation): Promise<void> {
	const db = await getDB();
	await db.put('pending_operations', cleanClone(op));
}

export async function getAllPendingOperations(): Promise<PendingOperation[]> {
	const db = await getDB();
	return db.getAll('pending_operations');
}

export async function removePendingOperation(id: string): Promise<void> {
	const db = await getDB();
	await db.delete('pending_operations', id);
}
