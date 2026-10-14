export interface DriveFile {
	id: string;
	name: string;
	mimeType: string;
	size?: string;
	modifiedTime?: string;
	webViewLink?: string;
	iconLink?: string;
	thumbnailLink?: string;
	parents?: string[];
	// UI & Offline properties
	isOfflinePending?: boolean;
	isCached?: boolean;
	hasConflict?: boolean;
	localBlob?: Blob;
	localText?: string;
}

export interface FolderBreadcrumb {
	id: string;
	name: string;
}

export interface AuthConfig {
	clientId: string;
	rootFolderId: string;
}

export interface StoredCredentials {
	accessToken: string;
	expiresAt: number; // timestamp in ms
}

export interface PendingUpload {
	id: string; // temporary local id
	name: string;
	mimeType: string;
	folderId: string;
	data: Blob;
	createdAt: number;
	clientModifiedTime: string;
}

export interface PendingOperation {
	id: string;
	type: 'rename' | 'delete' | 'create_folder' | 'copy' | 'move';
	targetId?: string; // file or folder id (if existing)
	name?: string; // new name or folder name
	folderId?: string; // target folder id
	parents?: string[];
	createdAt: number;
	originalModifiedTime?: string; // for conflict detection on rename/move/delete
}

export interface CachedFileItem {
	id: string;
	name: string;
	mimeType: string;
	size?: string;
	modifiedTime: string;
	cachedAt: number;
	data: Blob;
	isDirty?: boolean; // edited locally while offline
	baseServerModifiedTime?: string; // the server's modifiedTime when this file was originally fetched
}

export interface ConflictItem {
	fileId: string;
	fileName: string;
	folderId: string;
	serverModifiedTime: string;
	localModifiedTime: string;
	serverContent?: string;
	localContent?: string;
	serverBlob?: Blob;
	localBlob?: Blob;
	type?: 'content' | 'rename' | 'delete';
	proposedName?: string;
}

export interface ClipboardItem {
	action: 'copy' | 'cut';
	files: DriveFile[];
}
