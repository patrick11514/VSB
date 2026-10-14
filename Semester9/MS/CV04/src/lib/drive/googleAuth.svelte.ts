import type { StoredCredentials } from './types.js';

const STORAGE_KEY = 'google_drive_credentials';

export interface TokenResponse {
	access_token?: string;
	error?: string;
	error_description?: string;
	expires_in?: number;
}

export interface TokenClient {
	requestAccessToken: (overrides?: { prompt?: string }) => void;
}

export interface GoogleAccountsOAuth2 {
	initTokenClient: (config: {
		client_id: string;
		scope: string;
		callback: (response: TokenResponse) => void;
		error_callback?: (error: Error) => void;
	}) => TokenClient;
}

declare global {
	interface Window {
		google?: {
			accounts?: {
				oauth2?: GoogleAccountsOAuth2;
			};
		};
	}
}

export class GoogleAuthService {
	accessToken = $state<string | null>(null);
	expiresAt = $state<number | null>(null);
	isLoading = $state<boolean>(false);
	error = $state<string | null>(null);
	isInitialized = $state<boolean>(false);

	private tokenClient: TokenClient | null = null;
	private clientId: string = '';

	get isAuthenticated(): boolean {
		if (!this.accessToken || !this.expiresAt) return false;
		// Consider expired if within 60s of expiration
		return Date.now() < this.expiresAt - 60000;
	}

	init(clientId: string) {
		this.clientId = clientId;
		this.loadStoredCredentials();
		this.loadGsiScript();
	}

	private loadStoredCredentials() {
		if (typeof window === 'undefined') return;

		try {
			const saved = localStorage.getItem(STORAGE_KEY);
			if (saved) {
				const creds: StoredCredentials = JSON.parse(saved);
				if (creds.accessToken && creds.expiresAt && Date.now() < creds.expiresAt - 60000) {
					this.accessToken = creds.accessToken;
					this.expiresAt = creds.expiresAt;
				} else {
					this.clearCredentials();
				}
			}
		} catch (err) {
			console.error('Failed to read credentials from localStorage', err);
			this.clearCredentials();
		}
	}

	private saveCredentials(token: string, expiresInSeconds: number) {
		const expiresAt = Date.now() + expiresInSeconds * 1000;
		this.accessToken = token;
		this.expiresAt = expiresAt;

		try {
			const creds: StoredCredentials = {
				accessToken: token,
				expiresAt
			};
			localStorage.setItem(STORAGE_KEY, JSON.stringify(creds));
		} catch (err) {
			console.error('Failed to persist credentials in localStorage', err);
		}
	}

	clearCredentials() {
		this.accessToken = null;
		this.expiresAt = null;
		if (typeof window !== 'undefined') {
			localStorage.removeItem(STORAGE_KEY);
		}
	}

	private loadGsiScript() {
		if (typeof window === 'undefined') return;

		if (window.google?.accounts?.oauth2) {
			this.setupTokenClient();
			return;
		}

		const existingScript = document.querySelector(
			'script[src="https://accounts.google.com/gsi/client"]'
		);
		if (existingScript) {
			existingScript.addEventListener('load', () => this.setupTokenClient());
			return;
		}

		const script = document.createElement('script');
		script.src = 'https://accounts.google.com/gsi/client';
		script.async = true;
		script.defer = true;
		script.onload = () => {
			this.setupTokenClient();
		};
		script.onerror = () => {
			this.error = 'Failed to load Google Identity Services script.';
		};
		document.head.appendChild(script);
	}

	private setupTokenClient() {
		if (!this.clientId || !window.google?.accounts?.oauth2) return;

		try {
			this.tokenClient = window.google.accounts.oauth2.initTokenClient({
				client_id: this.clientId,
				// Request full drive access so user can create, copy, move, rename, delete and upload files
				scope: 'https://www.googleapis.com/auth/drive',
				callback: (response: TokenResponse) => {
					this.isLoading = false;
					if (response.error) {
						this.error = response.error_description || response.error;
						return;
					}
					if (response.access_token) {
						this.error = null;
						const expiresIn = Number(response.expires_in) || 3600;
						this.saveCredentials(response.access_token, expiresIn);
					}
				},
				error_callback: (err: Error) => {
					this.isLoading = false;
					this.error = err.message || 'Google OAuth authentication failed';
				}
			});
			this.isInitialized = true;
		} catch (e: unknown) {
			const message =
				e instanceof Error ? e.message : 'Could not initialize Google OAuth token client';
			this.error = message;
		}
	}

	login() {
		this.error = null;
		if (!this.tokenClient) {
			if (!window.google?.accounts?.oauth2) {
				this.loadGsiScript();
			} else {
				this.setupTokenClient();
			}
		}

		if (this.tokenClient) {
			this.isLoading = true;
			this.tokenClient.requestAccessToken({ prompt: 'consent' });
		} else {
			this.error = 'Google authentication client is not ready. Please try again in a moment.';
		}
	}
}

export const authService = new GoogleAuthService();
