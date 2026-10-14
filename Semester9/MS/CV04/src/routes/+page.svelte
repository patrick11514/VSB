<script lang="ts">
	import { onMount, onDestroy } from 'svelte';
	import { SvelteSet } from 'svelte/reactivity';
	import JSZip from 'jszip';
	import { authService } from '#/lib/drive/googleAuth.svelte.js';
	import {
		listDriveFiles,
		getDriveFileMetadata,
		downloadDriveFileBlob,
		uploadDriveFile,
		updateDriveFileContent,
		renameDriveFile,
		deleteDriveFile,
		copyDriveFile,
		moveDriveFile,
		createDriveFolder,
		isTextEditable,
		formatFileSize,
		formatDate,
		isFolder
	} from '#/lib/drive/driveService.js';
	import {
		saveCachedFile,
		getCachedFile,
		getAllCachedFiles,
		removeCachedFile,
		saveFolderListing,
		getFolderListing,
		savePendingUpload,
		getAllPendingUploads,
		removePendingUpload,
		savePendingOperation,
		getAllPendingOperations,
		removePendingOperation
	} from '#/lib/drive/offlineStorage.js';
	import type {
		DriveFile,
		FolderBreadcrumb,
		AuthConfig,
		ClipboardItem,
		ConflictItem
	} from '#/lib/drive/types.js';

	import { Card, CardContent } from '#/lib/components/ui/card/index.js';
	import { Button } from '#/lib/components/ui/button/index.js';
	import {
		Table,
		TableHeader,
		TableBody,
		TableHead,
		TableRow,
		TableCell
	} from '#/lib/components/ui/table/index.js';
	import { Badge } from '#/lib/components/ui/badge/index.js';
	import { Spinner } from '#/lib/components/ui/spinner/index.js';
	import { Alert, AlertTitle, AlertDescription } from '#/lib/components/ui/alert/index.js';
	import { Checkbox } from '#/lib/components/ui/checkbox/index.js';
	import { Input } from '#/lib/components/ui/input/index.js';
	import { Textarea } from '#/lib/components/ui/textarea/index.js';
	import * as Dialog from '#/lib/components/ui/dialog/index.js';
	import {
		Breadcrumb,
		BreadcrumbList,
		BreadcrumbItem,
		BreadcrumbLink,
		BreadcrumbPage,
		BreadcrumbSeparator
	} from '#/lib/components/ui/breadcrumb/index.js';

	// Lucide icons
	import Folder from '@lucide/svelte/icons/folder';
	import FolderOpen from '@lucide/svelte/icons/folder-open';
	import FolderPlus from '@lucide/svelte/icons/folder-plus';
	import FileIcon from '@lucide/svelte/icons/file';
	import FileText from '@lucide/svelte/icons/file-text';
	import Image from '@lucide/svelte/icons/image';
	import Video from '@lucide/svelte/icons/video';
	import Music from '@lucide/svelte/icons/music';
	import LogOut from '@lucide/svelte/icons/log-out';
	import ExternalLink from '@lucide/svelte/icons/external-link';
	import RefreshCw from '@lucide/svelte/icons/refresh-cw';
	import House from '@lucide/svelte/icons/house';
	import ArrowLeft from '@lucide/svelte/icons/arrow-left';
	import Cloud from '@lucide/svelte/icons/cloud';
	import AlertCircle from '@lucide/svelte/icons/alert-circle';
	import Upload from '@lucide/svelte/icons/upload';
	import Download from '@lucide/svelte/icons/download';
	import Copy from '@lucide/svelte/icons/copy';
	import Scissors from '@lucide/svelte/icons/scissors';
	import Clipboard from '@lucide/svelte/icons/clipboard';
	import Trash from '@lucide/svelte/icons/trash';
	import Pencil from '@lucide/svelte/icons/pencil';
	import FilePen from '@lucide/svelte/icons/file-pen';
	import Wifi from '@lucide/svelte/icons/wifi';
	import WifiOff from '@lucide/svelte/icons/wifi-off';
	import HardDrive from '@lucide/svelte/icons/hard-drive';
	import TriangleAlert from '@lucide/svelte/icons/triangle-alert';

	let authConfig = $state<AuthConfig | null>(null);
	let configLoading = $state(true);
	let configError = $state<string | null>(null);

	// Online / Offline status
	let isOnline = $state(true);
	let isSyncing = $state(false);

	// Drive files state
	let files = $state<DriveFile[]>([]);
	let cachedFileIds = new SvelteSet<string>();
	let filesLoading = $state(false);
	let filesError = $state<string | null>(null);
	let statusMessage = $state<string | null>(null);

	// Selection state
	let selectedIds = new SvelteSet<string>();

	// Clipboard state
	let clipboard = $state<ClipboardItem | null>(null);

	// Breadcrumb navigation
	let breadcrumbs = $state<FolderBreadcrumb[]>([]);

	let currentFolderId = $derived(
		breadcrumbs.length > 0
			? breadcrumbs[breadcrumbs.length - 1].id
			: authConfig?.rootFolderId || 'root'
	);

	// Dialog states
	let newFolderOpen = $state(false);
	let newFolderName = $state('');

	let renameOpen = $state(false);
	let renamingFile = $state<DriveFile | null>(null);
	let renameNewName = $state('');

	let deleteConfirmOpen = $state(false);

	// Text File Editor Modal
	let editorOpen = $state(false);
	let editingFile = $state<DriveFile | null>(null);
	let editorContent = $state('');
	let editorLoading = $state(false);
	let editorSaving = $state(false);

	// Conflict Resolution Modal
	let conflictOpen = $state(false);
	let currentConflict = $state<ConflictItem | null>(null);

	// Hidden file input ref for uploading
	let fileInputRef = $state<HTMLInputElement | null>(null);

	onMount(async () => {
		isOnline = typeof navigator !== 'undefined' ? navigator.onLine : true;

		window.addEventListener('online', handleOnlineStatusChange);
		window.addEventListener('offline', handleOnlineStatusChange);

		try {
			const res = await fetch('/api/auth/config');
			if (!res.ok) throw new Error('Failed to load application configuration');
			const data: AuthConfig = await res.json();
			authConfig = data;

			if (!data.clientId) {
				configError =
					'Google Client ID is missing. Please check your secrets.json or GOOGLE_CLIENT_ID env variable.';
			} else {
				authService.init(data.clientId);
				breadcrumbs = [{ id: data.rootFolderId || 'root', name: 'Root' }];
			}
		} catch (err: unknown) {
			configError = err instanceof Error ? err.message : 'Error initializing authentication';
		} finally {
			configLoading = false;
		}

		await refreshCachedFileIds();
	});

	onDestroy(() => {
		if (typeof window !== 'undefined') {
			window.removeEventListener('online', handleOnlineStatusChange);
			window.removeEventListener('offline', handleOnlineStatusChange);
		}
	});

	function handleOnlineStatusChange() {
		isOnline = navigator.onLine;
		if (isOnline) {
			statusMessage = 'Internet connection restored. Synchronizing offline queue...';
			syncOfflineQueue();
		} else {
			statusMessage = 'You are currently offline. Changes will be queued and synced when online.';
		}
	}

	async function refreshCachedFileIds() {
		try {
			const cached = await getAllCachedFiles();
			cachedFileIds.clear();
			for (const c of cached) {
				cachedFileIds.add(c.id);
			}
		} catch (err) {
			console.error('Failed to get cached files', err);
		}
	}

	// Trigger fetch when authenticated and folder changes
	$effect(() => {
		if (authService.isAuthenticated && currentFolderId) {
			fetchFiles(currentFolderId);
		}
	});

	async function fetchFiles(folderId: string) {
		filesLoading = true;
		filesError = null;
		selectedIds.clear();

		try {
			await refreshCachedFileIds();

			let loadedFiles: DriveFile[] = [];

			if (isOnline && authService.accessToken) {
				try {
					loadedFiles = await listDriveFiles(authService.accessToken, folderId);
					// Save folder listing in cache
					await saveFolderListing(folderId, loadedFiles);

					// Check if any cached files need background update
					checkAndSyncCachedFiles(loadedFiles);
				} catch (err: unknown) {
					// Fall back to offline cached listing if API fails
					console.warn('API fetch failed, checking offline cache...', err);
					const cached = await getFolderListing(folderId);
					if (cached) {
						loadedFiles = cached;
						statusMessage = 'Showing cached files due to network/API error.';
					} else {
						throw err;
					}
				}
			} else {
				// Offline mode: load from cache
				const cached = await getFolderListing(folderId);
				if (cached) {
					loadedFiles = cached;
				} else {
					loadedFiles = [];
				}
			}

			// Apply any local pending operations (renames, deletions, creations)
			const pendingOps = await getAllPendingOperations();
			for (const op of pendingOps) {
				if (op.type === 'create_folder' && op.folderId === folderId && op.name) {
					if (!loadedFiles.some((f) => f.id === op.id)) {
						loadedFiles.push({
							id: op.id,
							name: op.name,
							mimeType: 'application/vnd.google-apps.folder',
							modifiedTime: new Date(op.createdAt).toISOString(),
							isOfflinePending: true
						});
					}
				} else if (op.type === 'rename' && op.targetId && op.name) {
					const target = loadedFiles.find((f) => f.id === op.targetId);
					if (target) {
						target.name = op.name;
						target.isOfflinePending = true;
					}
				} else if (op.type === 'delete' && op.targetId) {
					loadedFiles = loadedFiles.filter((f) => f.id !== op.targetId);
				}
			}

			// Merge pending uploads for this folder
			const pendingUploads = await getAllPendingUploads();
			const folderPending = pendingUploads.filter((p) => p.folderId === folderId);

			for (const p of folderPending) {
				if (!loadedFiles.some((f) => f.name === p.name || f.id === p.id)) {
					loadedFiles.push({
						id: p.id,
						name: p.name,
						mimeType: p.mimeType,
						size: String(p.data.size),
						modifiedTime: p.clientModifiedTime,
						isOfflinePending: true
					});
				}
			}

			// Mark cached files
			for (const f of loadedFiles) {
				f.isCached = cachedFileIds.has(f.id);
			}

			files = loadedFiles;
		} catch (err: unknown) {
			const message =
				err instanceof Error ? err.message : 'Failed to fetch files from Google Drive';
			filesError = message;
			if (message.includes('401') || message.includes('Invalid Credentials')) {
				authService.clearCredentials();
			}
		} finally {
			filesLoading = false;
		}
	}

	// Check if cached files have new versions on server
	async function checkAndSyncCachedFiles(serverFiles: DriveFile[]) {
		if (!authService.accessToken || !isOnline) return;

		for (const sFile of serverFiles) {
			if (cachedFileIds.has(sFile.id)) {
				const localItem = await getCachedFile(sFile.id);
				if (localItem && !localItem.isDirty) {
					if (sFile.modifiedTime && localItem.modifiedTime !== sFile.modifiedTime) {
						try {
							const freshBlob = await downloadDriveFileBlob(authService.accessToken, sFile.id);
							await saveCachedFile({
								id: sFile.id,
								name: sFile.name,
								mimeType: sFile.mimeType,
								size: sFile.size,
								modifiedTime: sFile.modifiedTime,
								cachedAt: Date.now(),
								data: freshBlob
							});
						} catch (e) {
							console.warn('Failed background cache update for', sFile.name, e);
						}
					}
				}
			}
		}
	}

	// Sync offline queue (pending operations, uploads & edited cached files)
	async function syncOfflineQueue() {
		if (!isOnline || !authService.accessToken || isSyncing) return;
		isSyncing = true;
		statusMessage = 'Syncing offline changes with Google Drive...';

		try {
			// 1. Process pending operations (create folder, rename, delete, copy, move)
			const pendingOps = await getAllPendingOperations();
			for (const op of pendingOps) {
				try {
					if (op.type === 'create_folder' && op.folderId && op.name) {
						await createDriveFolder(authService.accessToken, op.folderId, op.name);
						await removePendingOperation(op.id);
					} else if (op.type === 'rename' && op.targetId && op.name) {
						// Check for conflict: was file modified on server after our rename operation was created?
						const serverMeta = await getDriveFileMetadata(authService.accessToken, op.targetId);
						if (
							op.originalModifiedTime &&
							serverMeta.modifiedTime &&
							serverMeta.modifiedTime !== op.originalModifiedTime
						) {
							// Conflict! File modified on server while renamed locally
							currentConflict = {
								fileId: op.targetId,
								fileName: serverMeta.name,
								folderId: op.folderId || currentFolderId,
								serverModifiedTime: serverMeta.modifiedTime,
								localModifiedTime: new Date(op.createdAt).toISOString(),
								type: 'rename',
								proposedName: op.name
							};
							conflictOpen = true;
							await removePendingOperation(op.id);
							return; // Pause sync for user decision
						} else {
							await renameDriveFile(authService.accessToken, op.targetId, op.name);
							await removePendingOperation(op.id);
						}
					} else if (op.type === 'delete' && op.targetId) {
						// Check for conflict: was file modified on server after delete requested?
						try {
							const serverMeta = await getDriveFileMetadata(authService.accessToken, op.targetId);
							if (
								op.originalModifiedTime &&
								serverMeta.modifiedTime &&
								serverMeta.modifiedTime !== op.originalModifiedTime
							) {
								currentConflict = {
									fileId: op.targetId,
									fileName: serverMeta.name,
									folderId: op.folderId || currentFolderId,
									serverModifiedTime: serverMeta.modifiedTime,
									localModifiedTime: new Date(op.createdAt).toISOString(),
									type: 'delete'
								};
								conflictOpen = true;
								await removePendingOperation(op.id);
								return; // Pause sync for user decision
							} else {
								await deleteDriveFile(authService.accessToken, op.targetId);
								await removePendingOperation(op.id);
							}
						} catch {
							// File already gone on server
							await removePendingOperation(op.id);
						}
					} else if (op.type === 'copy' && op.targetId && op.folderId) {
						await copyDriveFile(authService.accessToken, op.targetId, op.folderId, op.name);
						await removePendingOperation(op.id);
					} else if (op.type === 'move' && op.targetId && op.folderId) {
						await moveDriveFile(
							authService.accessToken,
							op.targetId,
							op.folderId,
							op.parents || []
						);
						await removePendingOperation(op.id);
					}
				} catch (opErr) {
					console.error('Failed to sync operation', op, opErr);
				}
			}

			// 2. Process pending uploads
			const pendingUploads = await getAllPendingUploads();
			for (const p of pendingUploads) {
				try {
					// Check if a file with same name already exists in target folder to detect conflict
					const existing = await listDriveFiles(authService.accessToken, p.folderId);
					const match = existing.find((f) => f.name === p.name);

					if (match) {
						// Conflict detected!
						const serverBlob = await downloadDriveFileBlob(authService.accessToken, match.id);
						const isText = isTextEditable(match);

						currentConflict = {
							fileId: match.id,
							fileName: match.name,
							folderId: p.folderId,
							serverModifiedTime: match.modifiedTime || '',
							localModifiedTime: p.clientModifiedTime,
							localBlob: p.data,
							serverBlob: serverBlob,
							localContent: isText ? await p.data.text() : undefined,
							serverContent: isText ? await serverBlob.text() : undefined,
							type: 'content'
						};
						conflictOpen = true;
						await removePendingUpload(p.id);
						return; // Pause processing until user resolves conflict
					} else {
						// Safe upload
						const uploaded = await uploadDriveFile(
							authService.accessToken,
							p.folderId,
							p.name,
							p.data
						);
						await removePendingUpload(p.id);
						// Save in cache
						await saveCachedFile({
							id: uploaded.id,
							name: uploaded.name,
							mimeType: uploaded.mimeType,
							size: uploaded.size,
							modifiedTime: uploaded.modifiedTime || new Date().toISOString(),
							cachedAt: Date.now(),
							data: p.data
						});
					}
				} catch (err) {
					console.error('Failed to sync upload for ' + p.name, err);
				}
			}

			// 3. Process dirty cached files edited locally
			const allCached = await getAllCachedFiles();
			for (const item of allCached) {
				if (item.isDirty) {
					try {
						const serverMeta = await getDriveFileMetadata(authService.accessToken, item.id);
						const baseTime = item.baseServerModifiedTime;

						// Check if server modifiedTime is different from the base time we started editing from
						const serverChanged =
							baseTime && serverMeta.modifiedTime && serverMeta.modifiedTime !== baseTime;

						if (serverChanged) {
							// Download server content to see if it's genuinely different
							const serverBlob = await downloadDriveFileBlob(authService.accessToken, item.id);
							const isText = isTextEditable(serverMeta);

							let contentDiffers = true;
							if (isText) {
								const localText = await item.data.text();
								const serverText = await serverBlob.text();
								contentDiffers = localText !== serverText;
							} else {
								contentDiffers = item.data.size !== serverBlob.size;
							}

							if (contentDiffers) {
								currentConflict = {
									fileId: item.id,
									fileName: item.name,
									folderId: currentFolderId,
									serverModifiedTime: serverMeta.modifiedTime || '',
									localModifiedTime: new Date(item.cachedAt).toISOString(),
									localBlob: item.data,
									serverBlob: serverBlob,
									localContent: isText ? await item.data.text() : undefined,
									serverContent: isText ? await serverBlob.text() : undefined,
									type: 'content'
								};
								conflictOpen = true;
								return;
							}
						}

						// Safe update: server hasn't changed since edit started, or content is identical
						const updated = await updateDriveFileContent(
							authService.accessToken,
							item.id,
							item.data
						);
						item.isDirty = false;
						item.modifiedTime = updated.modifiedTime || new Date().toISOString();
						item.baseServerModifiedTime = updated.modifiedTime;
						await saveCachedFile(item);
					} catch (e) {
						console.error('Failed to sync dirty cached file ' + item.name, e);
					}
				}
			}

			statusMessage = 'Synchronization complete!';
			setTimeout(() => {
				if (statusMessage === 'Synchronization complete!') statusMessage = null;
			}, 4000);

			await refreshCachedFileIds();
			fetchFiles(currentFolderId);
		} catch (err: unknown) {
			console.error('Sync failed', err);
			statusMessage = 'Sync failed. Will retry later.';
		} finally {
			isSyncing = false;
		}
	}

	// Conflict resolution handlers
	async function resolveConflictKeepServer() {
		if (!currentConflict) return;
		const fileId = currentConflict.fileId;
		// Clear dirty state on local cached file if it exists
		const cached = await getCachedFile(fileId);
		if (cached) {
			cached.isDirty = false;
			if (currentConflict.serverBlob) {
				cached.data = currentConflict.serverBlob;
			}
			cached.modifiedTime = currentConflict.serverModifiedTime;
			cached.baseServerModifiedTime = currentConflict.serverModifiedTime;
			await saveCachedFile(cached);
		}
		conflictOpen = false;
		currentConflict = null;
		statusMessage = 'Server version kept.';
		fetchFiles(currentFolderId);
		syncOfflineQueue();
	}

	async function resolveConflictKeepLocal() {
		if (!currentConflict || !authService.accessToken) return;
		try {
			if (currentConflict.type === 'rename' && currentConflict.proposedName) {
				await renameDriveFile(
					authService.accessToken,
					currentConflict.fileId,
					currentConflict.proposedName
				);
				statusMessage = `Renamed to "${currentConflict.proposedName}"!`;
			} else if (currentConflict.type === 'delete') {
				await deleteDriveFile(authService.accessToken, currentConflict.fileId);
				statusMessage = 'Deleted from server as requested.';
			} else if (currentConflict.localBlob) {
				const updated = await updateDriveFileContent(
					authService.accessToken,
					currentConflict.fileId,
					currentConflict.localBlob
				);
				// Mark as clean in cache
				const cached = await getCachedFile(currentConflict.fileId);
				if (cached) {
					cached.isDirty = false;
					cached.modifiedTime = updated.modifiedTime || new Date().toISOString();
					cached.baseServerModifiedTime = updated.modifiedTime;
					await saveCachedFile(cached);
				}
				statusMessage = 'Server file overwritten with local version.';
			}
			conflictOpen = false;
			currentConflict = null;
			fetchFiles(currentFolderId);
			syncOfflineQueue();
		} catch (err: unknown) {
			alert('Failed to resolve conflict: ' + (err instanceof Error ? err.message : String(err)));
		}
	}

	async function resolveConflictKeepBoth() {
		if (!currentConflict || !authService.accessToken) return;
		try {
			if (currentConflict.localBlob) {
				const dotIndex = currentConflict.fileName.lastIndexOf('.');
				const newName =
					dotIndex !== -1
						? `${currentConflict.fileName.substring(0, dotIndex)} (Local Copy)${currentConflict.fileName.substring(dotIndex)}`
						: `${currentConflict.fileName} (Local Copy)`;

				await uploadDriveFile(
					authService.accessToken,
					currentConflict.folderId,
					newName,
					currentConflict.localBlob
				);
				// Mark original cached item as clean
				const cached = await getCachedFile(currentConflict.fileId);
				if (cached) {
					cached.isDirty = false;
					cached.modifiedTime = currentConflict.serverModifiedTime;
					cached.baseServerModifiedTime = currentConflict.serverModifiedTime;
					await saveCachedFile(cached);
				}
				statusMessage = 'Saved local copy with separate name.';
			} else if (currentConflict.type === 'rename' && currentConflict.proposedName) {
				// Keep server original name and also make a copy with the proposed name
				await copyDriveFile(
					authService.accessToken,
					currentConflict.fileId,
					currentConflict.folderId,
					currentConflict.proposedName
				);
				statusMessage = `Created copy named "${currentConflict.proposedName}"!`;
			}
			conflictOpen = false;
			currentConflict = null;
			fetchFiles(currentFolderId);
			syncOfflineQueue();
		} catch (err: unknown) {
			alert('Failed to save copy: ' + (err instanceof Error ? err.message : String(err)));
		}
	}

	// File Upload Handler
	async function handleUploadFiles(e: Event) {
		const target = e.target as HTMLInputElement;
		if (!target.files || target.files.length === 0) return;

		const uploadedFilesList = Array.from(target.files);
		target.value = ''; // Reset input

		for (const file of uploadedFilesList) {
			if (isOnline && authService.accessToken) {
				try {
					statusMessage = `Uploading "${file.name}"...`;
					const uploaded = await uploadDriveFile(
						authService.accessToken,
						currentFolderId,
						file.name,
						file
					);
					// Cache uploaded file
					await saveCachedFile({
						id: uploaded.id,
						name: uploaded.name,
						mimeType: uploaded.mimeType,
						size: uploaded.size,
						modifiedTime: uploaded.modifiedTime || new Date().toISOString(),
						cachedAt: Date.now(),
						data: file
					});
				} catch (err: unknown) {
					console.error('Online upload failed, queuing for offline sync...', err);
					await queueOfflineUpload(file);
				}
			} else {
				// Offline upload queue
				await queueOfflineUpload(file);
			}
		}

		statusMessage = 'Upload processed.';
		setTimeout(() => {
			if (statusMessage === 'Upload processed.') statusMessage = null;
		}, 3000);
		await refreshCachedFileIds();
		fetchFiles(currentFolderId);
	}

	async function queueOfflineUpload(file: File) {
		const pendingId = 'pending_' + Date.now() + '_' + Math.random().toString(36).slice(2, 7);
		await savePendingUpload({
			id: pendingId,
			name: file.name,
			mimeType: file.type || 'application/octet-stream',
			folderId: currentFolderId,
			data: file,
			createdAt: Date.now(),
			clientModifiedTime: new Date().toISOString()
		});
		// Also cache locally
		await saveCachedFile({
			id: pendingId,
			name: file.name,
			mimeType: file.type || 'application/octet-stream',
			size: String(file.size),
			modifiedTime: new Date().toISOString(),
			cachedAt: Date.now(),
			data: file
		});
		statusMessage = `Saved "${file.name}" locally. It will upload when online.`;
	}

	// Toggle Cache for a file
	async function toggleCacheFile(file: DriveFile) {
		if (cachedFileIds.has(file.id)) {
			// Remove from cache
			await removeCachedFile(file.id);
			await refreshCachedFileIds();
			file.isCached = false;
			statusMessage = `Removed "${file.name}" from offline cache.`;
		} else {
			// Download to cache
			if (!authService.accessToken || !isOnline) {
				alert('Cannot cache file without online connection.');
				return;
			}
			try {
				statusMessage = `Caching "${file.name}" for offline use...`;
				const blob = await downloadDriveFileBlob(authService.accessToken, file.id);
				await saveCachedFile({
					id: file.id,
					name: file.name,
					mimeType: file.mimeType,
					size: file.size,
					modifiedTime: file.modifiedTime || new Date().toISOString(),
					cachedAt: Date.now(),
					data: blob
				});
				await refreshCachedFileIds();
				file.isCached = true;
				statusMessage = `Cached "${file.name}" for offline use!`;
			} catch (err: unknown) {
				alert(`Failed to cache: ${err instanceof Error ? err.message : String(err)}`);
			}
		}
		setTimeout(() => {
			statusMessage = null;
		}, 3000);
	}

	// Download single or multiple selected files
	async function downloadSelectedFiles() {
		const itemsToDownload = files.filter((f) => selectedIds.has(f.id) && !isFolder(f));
		if (itemsToDownload.length === 0) return;

		statusMessage = `Preparing download of ${itemsToDownload.length} file(s)...`;

		try {
			if (itemsToDownload.length === 1) {
				// Single file download
				const file = itemsToDownload[0];
				let blob: Blob;

				const cached = await getCachedFile(file.id);
				if (cached) {
					blob = cached.data;
				} else if (isOnline && authService.accessToken) {
					blob = await downloadDriveFileBlob(authService.accessToken, file.id);
				} else {
					throw new Error('File is not cached and you are currently offline.');
				}

				saveBlobToUserDisk(blob, file.name);
				statusMessage = `Downloaded "${file.name}"!`;
			} else {
				// Multiple files: bundle into ZIP
				const zip = new JSZip();

				for (const file of itemsToDownload) {
					let blob: Blob;
					const cached = await getCachedFile(file.id);
					if (cached) {
						blob = cached.data;
					} else if (isOnline && authService.accessToken) {
						blob = await downloadDriveFileBlob(authService.accessToken, file.id);
					} else {
						console.warn(`Skipping uncached file "${file.name}" while in offline mode.`);
						continue;
					}
					zip.file(file.name, blob);
				}

				const zipBlob = await zip.generateAsync({ type: 'blob' });
				saveBlobToUserDisk(zipBlob, `drowser_files_${Date.now()}.zip`);
				statusMessage = 'Downloaded selected files as ZIP bundle!';
			}
		} catch (err: unknown) {
			alert(`Download failed: ${err instanceof Error ? err.message : String(err)}`);
		} finally {
			setTimeout(() => {
				statusMessage = null;
			}, 3000);
		}
	}

	function saveBlobToUserDisk(blob: Blob, filename: string) {
		const url = URL.createObjectURL(blob);
		const a = document.createElement('a');
		a.href = url;
		a.download = filename;
		document.body.appendChild(a);
		a.click();
		document.body.removeChild(a);
		URL.revokeObjectURL(url);
	}

	// Single file download button
	async function downloadSingleFile(file: DriveFile) {
		try {
			let blob: Blob;
			const cached = await getCachedFile(file.id);
			if (cached) {
				blob = cached.data;
			} else if (isOnline && authService.accessToken) {
				blob = await downloadDriveFileBlob(authService.accessToken, file.id);
			} else {
				alert('This file is not cached and you are offline.');
				return;
			}
			saveBlobToUserDisk(blob, file.name);
		} catch (err: unknown) {
			alert(`Download error: ${err instanceof Error ? err.message : String(err)}`);
		}
	}

	// Selection helpers
	function toggleSelectAll() {
		if (selectedIds.size === files.length && files.length > 0) {
			selectedIds.clear();
		} else {
			selectedIds.clear();
			for (const f of files) {
				selectedIds.add(f.id);
			}
		}
	}

	function toggleSelectItem(id: string) {
		if (selectedIds.has(id)) {
			selectedIds.delete(id);
		} else {
			selectedIds.add(id);
		}
	}

	// Clipboard Actions: Copy, Cut, Paste (supports both online & offline queued)
	function handleCopy() {
		const selectedFiles = files
			.filter((f) => selectedIds.has(f.id))
			.map((f) => $state.snapshot(f));
		if (selectedFiles.length === 0) return;
		clipboard = { action: 'copy', files: selectedFiles };
		statusMessage = `Copied ${selectedFiles.length} item(s) to clipboard.`;
	}

	function handleCut() {
		const selectedFiles = files
			.filter((f) => selectedIds.has(f.id))
			.map((f) => $state.snapshot(f));
		if (selectedFiles.length === 0) return;
		clipboard = { action: 'cut', files: selectedFiles };
		statusMessage = `Cut ${selectedFiles.length} item(s) to clipboard.`;
	}

	async function handlePaste() {
		if (!clipboard) return;

		statusMessage = `Pasting ${clipboard.files.length} item(s)...`;

		try {
			if (isOnline && authService.accessToken) {
				for (const file of clipboard.files) {
					if (clipboard.action === 'copy') {
						await copyDriveFile(authService.accessToken, file.id, currentFolderId);
					} else if (clipboard.action === 'cut') {
						await moveDriveFile(
							authService.accessToken,
							file.id,
							currentFolderId,
							file.parents || []
						);
					}
				}
			} else {
				// Queue copy/move offline
				for (const file of clipboard.files) {
					const cleanFile = $state.snapshot(file);
					await savePendingOperation({
						id: 'op_' + Date.now() + '_' + Math.random().toString(36).slice(2, 7),
						type: clipboard.action === 'copy' ? 'copy' : 'move',
						targetId: cleanFile.id,
						folderId: currentFolderId,
						name: cleanFile.name,
						parents: cleanFile.parents ? [...cleanFile.parents] : undefined,
						createdAt: Date.now(),
						originalModifiedTime: cleanFile.modifiedTime
					});
				}
				statusMessage = `Pasted ${clipboard.files.length} item(s) locally. Will sync when online.`;
			}

			if (clipboard.action === 'cut') {
				clipboard = null;
			}

			statusMessage = 'Pasting completed!';
			fetchFiles(currentFolderId);
		} catch (err: unknown) {
			alert(`Paste failed: ${err instanceof Error ? err.message : String(err)}`);
		} finally {
			setTimeout(() => {
				statusMessage = null;
			}, 3000);
		}
	}

	// Create Folder (supports online & offline queued)
	async function handleCreateFolder() {
		if (!newFolderName.trim()) return;

		const folderName = newFolderName.trim();
		newFolderOpen = false;
		newFolderName = '';

		try {
			if (isOnline && authService.accessToken) {
				await createDriveFolder(authService.accessToken, currentFolderId, folderName);
				statusMessage = 'Folder created!';
			} else {
				// Offline creation
				await savePendingOperation({
					id: 'folder_' + Date.now() + '_' + Math.random().toString(36).slice(2, 7),
					type: 'create_folder',
					folderId: currentFolderId,
					name: folderName,
					createdAt: Date.now()
				});
				statusMessage = `Created folder "${folderName}" locally. Will sync when online.`;
			}
			fetchFiles(currentFolderId);
		} catch (err: unknown) {
			alert(`Failed to create folder: ${err instanceof Error ? err.message : String(err)}`);
		}
	}

	// Rename File/Folder (supports online & offline queued)
	function openRename(file: DriveFile) {
		renamingFile = file;
		renameNewName = file.name;
		renameOpen = true;
	}

	async function handleRename() {
		if (!renamingFile || !renameNewName.trim()) return;

		const newName = renameNewName.trim();
		const targetFile = renamingFile;
		renameOpen = false;
		renamingFile = null;

		try {
			if (isOnline && authService.accessToken) {
				await renameDriveFile(authService.accessToken, targetFile.id, newName);
				// Update in cache if present
				const cached = await getCachedFile(targetFile.id);
				if (cached) {
					cached.name = newName;
					await saveCachedFile(cached);
				}
				statusMessage = `Renamed to "${newName}"!`;
			} else {
				// Offline rename
				await savePendingOperation({
					id: 'op_' + Date.now() + '_' + Math.random().toString(36).slice(2, 7),
					type: 'rename',
					targetId: targetFile.id,
					name: newName,
					folderId: currentFolderId,
					createdAt: Date.now(),
					originalModifiedTime: targetFile.modifiedTime
				});

				const cached = await getCachedFile(targetFile.id);
				if (cached) {
					cached.name = newName;
					await saveCachedFile(cached);
				}
				statusMessage = `Renamed to "${newName}" locally. Will sync when online.`;
			}
			fetchFiles(currentFolderId);
		} catch (err: unknown) {
			alert(`Failed to rename: ${err instanceof Error ? err.message : String(err)}`);
		}
	}

	// Delete Selected (supports online & offline queued)
	async function handleDeleteSelected() {
		if (selectedIds.size === 0) return;

		deleteConfirmOpen = false;
		statusMessage = `Deleting ${selectedIds.size} item(s)...`;

		try {
			if (isOnline && authService.accessToken) {
				for (const id of selectedIds) {
					await deleteDriveFile(authService.accessToken, id);
					if (cachedFileIds.has(id)) {
						await removeCachedFile(id);
					}
				}
				statusMessage = 'Deleted successfully!';
			} else {
				// Offline delete queue
				for (const id of selectedIds) {
					const f = files.find((item) => item.id === id);
					await savePendingOperation({
						id: 'op_' + Date.now() + '_' + Math.random().toString(36).slice(2, 7),
						type: 'delete',
						targetId: id,
						folderId: currentFolderId,
						createdAt: Date.now(),
						originalModifiedTime: f?.modifiedTime
					});
					if (cachedFileIds.has(id)) {
						await removeCachedFile(id);
					}
				}
				statusMessage = 'Deleted locally. Will sync deletion when online.';
			}
			selectedIds.clear();
			fetchFiles(currentFolderId);
		} catch (err: unknown) {
			alert(`Deletion failed: ${err instanceof Error ? err.message : String(err)}`);
		} finally {
			setTimeout(() => {
				statusMessage = null;
			}, 3000);
		}
	}

	// Text File Editor
	async function openTextEditor(file: DriveFile) {
		editingFile = file;
		editorLoading = true;
		editorOpen = true;
		editorContent = '';

		try {
			const cached = await getCachedFile(file.id);
			if (cached) {
				editorContent = await cached.data.text();
				if (!cached.baseServerModifiedTime && file.modifiedTime) {
					cached.baseServerModifiedTime = file.modifiedTime;
					await saveCachedFile(cached);
				}
			} else if (isOnline && authService.accessToken) {
				const blob = await downloadDriveFileBlob(authService.accessToken, file.id);
				editorContent = await blob.text();
				// Also cache it locally with baseServerModifiedTime
				await saveCachedFile({
					id: file.id,
					name: file.name,
					mimeType: file.mimeType,
					size: file.size,
					modifiedTime: file.modifiedTime || new Date().toISOString(),
					baseServerModifiedTime: file.modifiedTime,
					cachedAt: Date.now(),
					data: blob,
					isDirty: false
				});
				await refreshCachedFileIds();
			} else {
				throw new Error('File content is not available offline.');
			}
		} catch (err: unknown) {
			alert(`Failed to open file: ${err instanceof Error ? err.message : String(err)}`);
			editorOpen = false;
		} finally {
			editorLoading = false;
		}
	}

	async function handleSaveEditedText() {
		if (!editingFile) return;
		editorSaving = true;

		try {
			const blob = new Blob([editorContent], {
				type: editingFile.mimeType || 'text/plain'
			});

			if (isOnline && authService.accessToken) {
				const updated = await updateDriveFileContent(authService.accessToken, editingFile.id, blob);
				await saveCachedFile({
					id: editingFile.id,
					name: editingFile.name,
					mimeType: editingFile.mimeType,
					size: String(blob.size),
					modifiedTime: updated.modifiedTime || new Date().toISOString(),
					cachedAt: Date.now(),
					data: blob,
					isDirty: false
				});
				statusMessage = `Saved "${editingFile.name}" to Google Drive!`;
			} else {
				const existingCached = await getCachedFile(editingFile.id);
				const baseTime =
					existingCached?.baseServerModifiedTime ||
					existingCached?.modifiedTime ||
					editingFile.modifiedTime ||
					new Date().toISOString();

				await saveCachedFile({
					id: editingFile.id,
					name: editingFile.name,
					mimeType: editingFile.mimeType,
					size: String(blob.size),
					modifiedTime: new Date().toISOString(),
					baseServerModifiedTime: baseTime,
					cachedAt: Date.now(),
					data: blob,
					isDirty: true
				});
				statusMessage = `Saved "${editingFile.name}" locally. Will sync when online.`;
			}

			editorOpen = false;
			editingFile = null;
			await refreshCachedFileIds();
			fetchFiles(currentFolderId);
		} catch (err: unknown) {
			alert(`Save failed: ${err instanceof Error ? err.message : String(err)}`);
		} finally {
			editorSaving = false;
		}
	}

	// Folder drilldown
	function handleFolderClick(folder: DriveFile) {
		breadcrumbs = [...breadcrumbs, { id: folder.id, name: folder.name }];
		fetchFiles(folder.id);
	}

	function navigateToBreadcrumb(index: number) {
		if (index === breadcrumbs.length - 1) return;
		breadcrumbs = breadcrumbs.slice(0, index + 1);
		fetchFiles(breadcrumbs[breadcrumbs.length - 1].id);
	}

	function navigateUp() {
		if (breadcrumbs.length <= 1) return;
		navigateToBreadcrumb(breadcrumbs.length - 2);
	}

	function getFileIcon(file: DriveFile) {
		if (isFolder(file)) return Folder;
		const mime = file.mimeType.toLowerCase();
		if (mime.includes('image')) return Image;
		if (mime.includes('video')) return Video;
		if (mime.includes('audio')) return Music;
		if (mime.includes('pdf') || mime.includes('document') || mime.includes('text')) return FileText;
		return FileIcon;
	}
</script>

<div class="flex min-h-screen flex-col bg-muted/20 text-foreground">
	<!-- Navbar / Header -->
	<header
		class="sticky top-0 z-10 border-b bg-background/95 backdrop-blur supports-[backdrop-filter]:bg-background/60"
	>
		<div class="container mx-auto flex h-16 items-center justify-between px-4">
			<div class="flex items-center gap-3">
				<div class="flex items-center justify-center rounded-lg bg-primary/10 p-2 text-primary">
					<Cloud class="size-5" />
				</div>
				<div>
					<h1 class="text-lg font-semibold tracking-tight">Drowser</h1>
				</div>
			</div>

			<div class="flex items-center gap-3">
				<!-- Single Online / Offline network status badge -->
				{#if isOnline}
					<Badge
						variant="outline"
						class="gap-1.5 border-emerald-500/20 bg-emerald-500/10 px-2.5 py-1 text-xs font-normal text-emerald-700 dark:text-emerald-400"
					>
						<Wifi class="size-3.5" />
						Online
					</Badge>
				{:else}
					<Badge
						variant="outline"
						class="gap-1.5 border-amber-500/20 bg-amber-500/10 px-2.5 py-1 text-xs font-normal text-amber-700 dark:text-amber-400"
					>
						<WifiOff class="size-3.5" />
						Offline
					</Badge>
				{/if}

				{#if authService.isAuthenticated}
					<Button
						variant="outline"
						size="sm"
						onclick={() => fetchFiles(currentFolderId)}
						disabled={filesLoading}
						title="Refresh folder"
					>
						<RefreshCw class="size-4 {filesLoading ? 'animate-spin' : ''}" />
						<span class="hidden sm:inline">Refresh</span>
					</Button>
					<Button
						variant="ghost"
						size="sm"
						onclick={() => authService.clearCredentials()}
						class="text-destructive hover:bg-destructive/10"
					>
						<LogOut class="size-4" />
						<span class="hidden sm:inline">Disconnect</span>
					</Button>
				{/if}
			</div>
		</div>
	</header>

	<!-- Status / Info Banner -->
	{#if statusMessage}
		<div class="border-b bg-primary/10 px-4 py-2 text-center text-xs font-medium text-primary">
			{statusMessage}
		</div>
	{/if}

	<!-- Main Body -->
	<main class="container mx-auto flex flex-1 flex-col px-4 py-8">
		{#if configLoading}
			<div class="flex flex-1 flex-col items-center justify-center gap-3">
				<Spinner class="size-8 text-primary" />
				<p class="text-sm text-muted-foreground">Initializing...</p>
			</div>
		{:else if configError}
			<div class="mx-auto my-auto w-full max-w-md">
				<Alert variant="destructive">
					<AlertCircle class="size-5" />
					<AlertTitle>Configuration Error</AlertTitle>
					<AlertDescription class="mt-2 text-sm">{configError}</AlertDescription>
				</Alert>
			</div>
		{:else if !authService.isAuthenticated}
			<!-- Centered Auth Login Button -->
			<div class="flex flex-1 items-center justify-center p-4">
				<Card class="w-full max-w-sm border-border/80 p-6 shadow-lg">
					<CardContent class="flex flex-col items-center justify-center gap-4 p-0">
						<div
							class="flex size-14 items-center justify-center rounded-2xl bg-primary/10 text-primary"
						>
							<Cloud class="size-8" />
						</div>
						<div class="text-center">
							<h2 class="text-xl font-bold tracking-tight">Drowser</h2>
							<p class="text-xs text-muted-foreground">Sign in to browse your Google Drive</p>
						</div>

						{#if authService.error}
							<Alert variant="destructive" class="w-full">
								<AlertCircle class="size-4" />
								<AlertTitle>Authentication Failed</AlertTitle>
								<AlertDescription class="text-xs">{authService.error}</AlertDescription>
							</Alert>
						{/if}

						<Button
							class="h-11 w-full gap-2 text-sm font-medium shadow-sm"
							onclick={() => authService.login()}
							disabled={authService.isLoading}
						>
							{#if authService.isLoading}
								<Spinner class="size-4" />
								<span>Authorizing popup...</span>
							{:else}
								<svg class="size-4" viewBox="0 0 24 24">
									<path
										fill="#4285F4"
										d="M22.56 12.25c0-.78-.07-1.53-.2-2.25H12v4.26h5.92c-.26 1.37-1.04 2.53-2.21 3.31v2.77h3.57c2.08-1.92 3.28-4.74 3.28-8.09z"
									/>
									<path
										fill="#34A853"
										d="M12 23c2.97 0 5.46-.98 7.28-2.66l-3.57-2.77c-.98.66-2.23 1.06-3.71 1.06-2.86 0-5.29-1.93-6.16-4.53H2.18v2.84C3.99 20.53 7.7 23 12 23z"
									/>
									<path
										fill="#FBBC05"
										d="M5.84 14.09c-.22-.66-.35-1.36-.35-2.09s.13-1.43.35-2.09V7.06H2.18C1.43 8.55 1 10.22 1 12s.43 3.45 1.18 4.94l2.85-2.22.81-.63z"
									/>
									<path
										fill="#EA4335"
										d="M12 5.38c1.62 0 3.06.56 4.21 1.64l3.15-3.15C17.45 2.09 14.97 1 12 1 7.7 1 3.99 3.47 2.18 7.06l3.66 2.84c.87-2.6 3.3-4.52 6.16-4.52z"
									/>
								</svg>
								<span>Sign in with Google</span>
							{/if}
						</Button>
					</CardContent>
				</Card>
			</div>
		{:else}
			<!-- Authenticated Drive Browser UI -->
			<div class="flex flex-1 flex-col space-y-4">
				<!-- Action Toolbar -->
				<div
					class="flex flex-wrap items-center justify-between gap-3 rounded-xl border bg-background p-3"
				>
					<div class="flex flex-wrap items-center gap-2">
						<!-- Hidden upload input -->
						<input
							type="file"
							multiple
							class="hidden"
							bind:this={fileInputRef}
							onchange={handleUploadFiles}
						/>

						<Button
							size="sm"
							class="gap-1.5"
							onclick={() => fileInputRef?.click()}
							title="Upload files"
						>
							<Upload class="size-4" />
							<span>Upload</span>
						</Button>

						<Button
							variant="outline"
							size="sm"
							class="gap-1.5"
							onclick={() => (newFolderOpen = true)}
							title="Create new folder"
						>
							<FolderPlus class="size-4" />
							<span class="hidden sm:inline">New Folder</span>
						</Button>

						{#if selectedIds.size > 0}
							<!-- Download Selected (Supports multiple via ZIP) -->
							<Button
								variant="outline"
								size="sm"
								class="gap-1.5"
								onclick={downloadSelectedFiles}
								title="Download selected files"
							>
								<Download class="size-4" />
								<span>Download ({selectedIds.size})</span>
							</Button>

							<!-- Copy -->
							<Button
								variant="outline"
								size="sm"
								class="gap-1.5"
								onclick={handleCopy}
								title="Copy selected"
							>
								<Copy class="size-4" />
								<span class="hidden md:inline">Copy</span>
							</Button>

							<!-- Cut -->
							<Button
								variant="outline"
								size="sm"
								class="gap-1.5"
								onclick={handleCut}
								title="Cut selected"
							>
								<Scissors class="size-4" />
								<span class="hidden md:inline">Cut</span>
							</Button>

							<!-- Delete -->
							<Button
								variant="destructive"
								size="sm"
								class="gap-1.5"
								onclick={() => (deleteConfirmOpen = true)}
								title="Delete selected"
							>
								<Trash class="size-4" />
								<span class="hidden sm:inline">Delete</span>
							</Button>
						{/if}

						{#if clipboard && clipboard.files.length > 0}
							<!-- Paste -->
							<Button
								variant="secondary"
								size="sm"
								class="gap-1.5 border border-primary/20"
								onclick={handlePaste}
								title="Paste clipboard files here"
							>
								<Clipboard class="size-4" />
								<span>Paste ({clipboard.files.length} {clipboard.action})</span>
							</Button>
						{/if}
					</div>

					<div class="flex items-center gap-2 text-xs text-muted-foreground">
						{#if isSyncing}
							<div class="flex items-center gap-1.5 text-primary">
								<Spinner class="size-3.5" />
								<span>Syncing...</span>
							</div>
						{/if}
						<span>{files.length} {files.length === 1 ? 'item' : 'items'}</span>
					</div>
				</div>

				<!-- Navigation bar & breadcrumbs -->
				<div
					class="flex flex-wrap items-center justify-between gap-4 rounded-xl border bg-background p-4"
				>
					<div class="flex flex-wrap items-center gap-2">
						{#if breadcrumbs.length > 1}
							<Button
								variant="outline"
								size="icon-sm"
								onclick={navigateUp}
								title="Go up one folder"
							>
								<ArrowLeft class="size-4" />
							</Button>
						{/if}

						<Breadcrumb>
							<BreadcrumbList>
								{#each breadcrumbs as crumb, index (crumb.id + index)}
									<BreadcrumbItem>
										{#if index === breadcrumbs.length - 1}
											<BreadcrumbPage class="flex items-center gap-1.5 font-medium">
												{#if index === 0}
													<House class="size-4" />
												{:else}
													<FolderOpen class="size-4 text-primary" />
												{/if}
												<span>{crumb.name}</span>
											</BreadcrumbPage>
										{:else}
											<BreadcrumbLink
												href="#"
												onclick={(e: MouseEvent) => {
													e.preventDefault();
													navigateToBreadcrumb(index);
												}}
												class="flex items-center gap-1.5 hover:text-foreground"
											>
												{#if index === 0}
													<House class="size-4" />
												{:else}
													<Folder class="size-4" />
												{/if}
												<span>{crumb.name}</span>
											</BreadcrumbLink>
										{/if}
									</BreadcrumbItem>
									{#if index < breadcrumbs.length - 1}
										<BreadcrumbSeparator />
									{/if}
								{/each}
							</BreadcrumbList>
						</Breadcrumb>
					</div>
				</div>

				<!-- Error alert if files failed to load -->
				{#if filesError}
					<Alert variant="destructive">
						<AlertCircle class="size-4" />
						<AlertTitle>Error loading folder</AlertTitle>
						<AlertDescription class="mt-1 flex items-center justify-between text-sm">
							<span>{filesError}</span>
							<Button size="sm" variant="outline" onclick={() => fetchFiles(currentFolderId)}>
								Try again
							</Button>
						</AlertDescription>
					</Alert>
				{/if}

				<!-- Files Table / Content -->
				<div class="flex flex-1 flex-col overflow-hidden rounded-xl border bg-background shadow-xs">
					{#if filesLoading}
						<div class="flex min-h-[300px] flex-1 flex-col items-center justify-center gap-3">
							<Spinner class="size-7 text-primary" />
							<p class="text-sm text-muted-foreground">Loading folder contents...</p>
						</div>
					{:else if files.length === 0}
						<div
							class="flex min-h-[300px] flex-1 flex-col items-center justify-center p-8 text-center"
						>
							<div class="mb-4 flex size-16 items-center justify-center rounded-full bg-muted">
								<FolderOpen class="size-8 text-muted-foreground" />
							</div>
							<h3 class="text-lg font-semibold">Folder is empty</h3>
							<p class="mt-1 max-w-sm text-sm text-muted-foreground">
								Upload files or create subfolders to get started.
							</p>
						</div>
					{:else}
						<Table>
							<TableHeader>
								<TableRow class="hover:bg-transparent">
									<TableHead class="w-12 text-center">
										<Checkbox
											checked={selectedIds.size === files.length && files.length > 0}
											indeterminate={selectedIds.size > 0 && selectedIds.size < files.length}
											onCheckedChange={toggleSelectAll}
										/>
									</TableHead>
									<TableHead class="w-[42%]">Name</TableHead>
									<TableHead class="w-[14%]">Status / Cache</TableHead>
									<TableHead class="w-[14%]">Size</TableHead>
									<TableHead class="w-[16%]">Last Modified</TableHead>
									<TableHead class="w-[14%] text-right">Actions</TableHead>
								</TableRow>
							</TableHeader>
							<TableBody>
								{#each files as file (file.id)}
									{@const isDir = isFolder(file)}
									{@const isEditable = isTextEditable(file)}
									{@const IconComponent = getFileIcon(file)}
									<TableRow class="group {selectedIds.has(file.id) ? 'bg-muted/40' : ''}">
										<!-- Select Checkbox -->
										<TableCell class="text-center">
											<Checkbox
												checked={selectedIds.has(file.id)}
												onCheckedChange={() => toggleSelectItem(file.id)}
											/>
										</TableCell>

										<!-- Name & Icon -->
										<TableCell class="font-medium">
											<div class="flex items-center gap-3">
												<div
													class="flex size-8 shrink-0 items-center justify-center rounded-lg {isDir
														? 'bg-amber-500/10 text-amber-600 dark:text-amber-400'
														: 'bg-muted text-muted-foreground'}"
												>
													<IconComponent class="size-4" />
												</div>

												{#if isDir}
													<button
														type="button"
														class="max-w-md truncate text-left font-medium text-foreground hover:underline focus:outline-hidden"
														onclick={() => handleFolderClick(file)}
													>
														{file.name}
													</button>
												{:else}
													<div class="flex max-w-md items-center gap-1.5 truncate">
														<span class="truncate">{file.name}</span>
														{#if file.webViewLink}
															<a
																href={file.webViewLink}
																target="_blank"
																rel="noopener noreferrer"
																class="text-muted-foreground transition-colors hover:text-primary"
																title="Open in Google Drive"
															>
																<ExternalLink class="size-3.5" />
															</a>
														{/if}
													</div>
												{/if}
											</div>
										</TableCell>

										<!-- Status / Offline Cache Marker -->
										<TableCell>
											<div class="flex flex-wrap items-center gap-1.5">
												{#if file.isOfflinePending}
													<Badge
														variant="outline"
														class="border-amber-500/30 bg-amber-500/10 text-[11px] font-normal text-amber-600 dark:text-amber-400"
													>
														Pending Sync
													</Badge>
												{:else if isDir}
													<Badge variant="secondary" class="text-[11px] font-normal">Folder</Badge>
												{:else if cachedFileIds.has(file.id)}
													<Badge
														variant="outline"
														class="border-emerald-500/30 bg-emerald-500/10 text-[11px] font-normal text-emerald-600 dark:text-emerald-400"
													>
														Cached Offline
													</Badge>
												{:else}
													<span class="text-xs text-muted-foreground">Online only</span>
												{/if}
											</div>
										</TableCell>

										<!-- File Size -->
										<TableCell class="text-xs text-muted-foreground">
											{isDir ? '-' : formatFileSize(file.size)}
										</TableCell>

										<!-- Last Modified -->
										<TableCell class="text-xs text-muted-foreground">
											{formatDate(file.modifiedTime)}
										</TableCell>

										<!-- Action buttons -->
										<TableCell class="text-right">
											<div class="flex items-center justify-end gap-1">
												{#if isEditable}
													<!-- Edit text file inline -->
													<Button
														variant="ghost"
														size="icon-xs"
														onclick={() => openTextEditor(file)}
														title="Edit text file"
													>
														<FilePen class="size-3.5" />
													</Button>
												{/if}

												{#if !isDir}
													<!-- Toggle Cache button -->
													<Button
														variant="ghost"
														size="icon-xs"
														onclick={() => toggleCacheFile(file)}
														class={cachedFileIds.has(file.id) ? 'text-emerald-600' : ''}
														title={cachedFileIds.has(file.id)
															? 'Remove from cache'
															: 'Cache file offline'}
													>
														<HardDrive class="size-3.5" />
													</Button>

													<!-- Download single -->
													<Button
														variant="ghost"
														size="icon-xs"
														onclick={() => downloadSingleFile(file)}
														title="Download file"
													>
														<Download class="size-3.5" />
													</Button>
												{/if}

												<!-- Rename button -->
												<Button
													variant="ghost"
													size="icon-xs"
													onclick={() => openRename(file)}
													title="Rename"
												>
													<Pencil class="size-3.5" />
												</Button>
											</div>
										</TableCell>
									</TableRow>
								{/each}
							</TableBody>
						</Table>
					{/if}
				</div>
			</div>
		{/if}
	</main>
</div>

<!-- Dialog: Create New Folder -->
<Dialog.Root bind:open={newFolderOpen}>
	<Dialog.Content>
		<Dialog.Header>
			<Dialog.Title>Create New Folder</Dialog.Title>
			<Dialog.Description>
				Enter a name for the new folder in the current directory.
			</Dialog.Description>
		</Dialog.Header>
		<div class="py-2">
			<Input
				placeholder="Folder name"
				bind:value={newFolderName}
				onkeydown={(e: KeyboardEvent) => e.key === 'Enter' && handleCreateFolder()}
			/>
		</div>
		<Dialog.Footer>
			<Button variant="outline" onclick={() => (newFolderOpen = false)}>Cancel</Button>
			<Button onclick={handleCreateFolder}>Create</Button>
		</Dialog.Footer>
	</Dialog.Content>
</Dialog.Root>

<!-- Dialog: Rename Item -->
<Dialog.Root bind:open={renameOpen}>
	<Dialog.Content>
		<Dialog.Header>
			<Dialog.Title>Rename Item</Dialog.Title>
			<Dialog.Description>Enter a new name for "{renamingFile?.name}".</Dialog.Description>
		</Dialog.Header>
		<div class="py-2">
			<Input
				placeholder="New name"
				bind:value={renameNewName}
				onkeydown={(e: KeyboardEvent) => e.key === 'Enter' && handleRename()}
			/>
		</div>
		<Dialog.Footer>
			<Button variant="outline" onclick={() => (renameOpen = false)}>Cancel</Button>
			<Button onclick={handleRename}>Rename</Button>
		</Dialog.Footer>
	</Dialog.Content>
</Dialog.Root>

<!-- Dialog: Delete Confirmation -->
<Dialog.Root bind:open={deleteConfirmOpen}>
	<Dialog.Content>
		<Dialog.Header>
			<Dialog.Title>Delete {selectedIds.size} Item(s)?</Dialog.Title>
			<Dialog.Description>
				Are you sure you want to delete the selected items?
				{#if !isOnline}
					Deletion will be recorded locally and synced when online.
				{/if}
			</Dialog.Description>
		</Dialog.Header>
		<Dialog.Footer>
			<Button variant="outline" onclick={() => (deleteConfirmOpen = false)}>Cancel</Button>
			<Button variant="destructive" onclick={handleDeleteSelected}>Delete</Button>
		</Dialog.Footer>
	</Dialog.Content>
</Dialog.Root>

<!-- Dialog: Text File Editor -->
<Dialog.Root bind:open={editorOpen}>
	<Dialog.Content class="max-w-2xl sm:max-w-2xl">
		<Dialog.Header>
			<Dialog.Title class="flex items-center gap-2">
				<FilePen class="size-4" />
				<span>Edit {editingFile?.name}</span>
			</Dialog.Title>
			<Dialog.Description>
				{#if isOnline}
					Editing text file online. Changes will update directly in Google Drive.
				{:else}
					Offline mode. Changes will be saved locally to cache and synced when connection restores.
				{/if}
			</Dialog.Description>
		</Dialog.Header>

		<div class="py-2">
			{#if editorLoading}
				<div class="flex h-64 items-center justify-center gap-2">
					<Spinner class="size-6 text-primary" />
					<span class="text-sm text-muted-foreground">Loading file contents...</span>
				</div>
			{:else}
				<Textarea
					class="field-sizing-content min-h-[300px] resize-y font-mono text-xs"
					bind:value={editorContent}
					rows={15}
				/>
			{/if}
		</div>

		<Dialog.Footer>
			<Button variant="outline" onclick={() => (editorOpen = false)}>Cancel</Button>
			<Button
				onclick={handleSaveEditedText}
				disabled={editorLoading || editorSaving}
				class="gap-1.5"
			>
				{#if editorSaving}
					<Spinner class="size-4" />
					<span>Saving...</span>
				{:else}
					<span>Save Changes</span>
				{/if}
			</Button>
		</Dialog.Footer>
	</Dialog.Content>
</Dialog.Root>

<!-- Dialog: Conflict Resolution -->
<Dialog.Root bind:open={conflictOpen}>
	<Dialog.Content class="max-w-xl sm:max-w-xl">
		<Dialog.Header>
			<Dialog.Title class="flex items-center gap-2 text-destructive">
				<TriangleAlert class="size-5" />
				<span>Sync Conflict Detected</span>
			</Dialog.Title>
			<Dialog.Description>
				{#if currentConflict?.type === 'rename'}
					The item <strong>"{currentConflict?.fileName}"</strong> was renamed offline to
					<strong>"{currentConflict?.proposedName}"</strong>, but was also modified on Google Drive
					in the meantime.
				{:else if currentConflict?.type === 'delete'}
					The item <strong>"{currentConflict?.fileName}"</strong> was scheduled for deletion offline,
					but was modified on Google Drive in the meantime.
				{:else}
					The file <strong>"{currentConflict?.fileName}"</strong> was modified on Google Drive while you
					made local changes or uploaded an offline version.
				{/if}
			</Dialog.Description>
		</Dialog.Header>

		<div class="space-y-4 py-2 text-xs">
			<div class="grid grid-cols-2 gap-3 rounded-lg border bg-muted/30 p-3">
				<div>
					<p class="font-semibold text-foreground">Local Action / Version</p>
					<p class="text-muted-foreground">
						Modified: {formatDate(currentConflict?.localModifiedTime)}
					</p>
					{#if currentConflict?.proposedName}
						<p class="mt-1 text-foreground">
							Proposed Name: <span class="font-semibold">{currentConflict.proposedName}</span>
						</p>
					{/if}
				</div>
				<div>
					<p class="font-semibold text-foreground">Server Version</p>
					<p class="text-muted-foreground">
						Modified: {formatDate(currentConflict?.serverModifiedTime)}
					</p>
				</div>
			</div>

			{#if currentConflict?.localContent !== undefined && currentConflict?.serverContent !== undefined}
				<div class="space-y-2">
					<p class="font-medium text-foreground">Content Preview:</p>
					<div class="grid max-h-48 grid-cols-2 gap-2 overflow-y-auto">
						<div class="rounded border bg-background p-2 font-mono text-[10px]">
							<span class="mb-1 block font-bold text-muted-foreground">Local:</span>
							<pre class="whitespace-pre-wrap">{currentConflict.localContent.slice(0, 400)}</pre>
						</div>
						<div class="rounded border bg-background p-2 font-mono text-[10px]">
							<span class="mb-1 block font-bold text-muted-foreground">Server:</span>
							<pre class="whitespace-pre-wrap">{currentConflict.serverContent.slice(0, 400)}</pre>
						</div>
					</div>
				</div>
			{/if}
		</div>

		<Dialog.Footer class="flex flex-col gap-2 sm:flex-row">
			<Button variant="outline" size="sm" onclick={resolveConflictKeepServer}>
				Keep Server Version
			</Button>
			{#if currentConflict?.type !== 'delete'}
				<Button variant="secondary" size="sm" onclick={resolveConflictKeepBoth}>
					Keep Both (Copy)
				</Button>
			{/if}
			<Button variant="destructive" size="sm" onclick={resolveConflictKeepLocal}>
				{#if currentConflict?.type === 'rename'}
					Apply Rename
				{:else if currentConflict?.type === 'delete'}
					Delete Anyway
				{:else}
					Overwrite Server
				{/if}
			</Button>
		</Dialog.Footer>
	</Dialog.Content>
</Dialog.Root>
