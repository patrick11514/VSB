<script lang="ts">
	import { browser } from '$app/environment';
	import { Button } from '$lib/components/ui/button';
	import { decryptWithPassphrase, sha256Hex } from '$lib/services/crypto';
	import { getFile, ipfsToGatewayUrl } from '$lib/services/ipfs';
	import { createEventDispatcher, onMount } from 'svelte';

	let { open = $bindable(), auction, winnerDetails } = $props();

	const dispatch = createEventDispatcher();

	let activeTab = $state<'manual' | 'automatic'>('automatic');
	let status = $state('');
	let error = $state('');
	let progress = $state(0);
	let fetchedBytes = $state<Uint8Array | null>(null);
	let decryptedBytes = $state<Uint8Array | null>(null);
	let passphrase = $state('');
	let autoOk = $state(false);
	let encryptedHash = $state('');
	let originalHash = $state('');
	let filename = $state('');

	onMount(() => {
		if (browser && auction?.address) {
			const stored = localStorage.getItem(`AUCTION_${auction.address}`);
			if (stored) passphrase = stored;
		}
	});

	function close() {
		open = false;
		status = '';
		error = '';
		progress = 0;
		fetchedBytes = null;
		decryptedBytes = null;
		autoOk = false;
		dispatch('close');
	}

	function manualChecksumDisplay() {
		return {
			encrypted: auction?.hashedFileHash ?? 'N/A',
			original: auction?.originalFileHash ?? 'N/A'
		};
	}

	async function startAutomatic() {
		error = '';
		status = 'Fetching encrypted payload from IPFS...';
		progress = 10;
		try {
			const cid = auction?.ipfsHandle?.startsWith('ipfs://')
				? auction.ipfsHandle.replace('ipfs://', '')
				: auction?.ipfsHandle?.startsWith('/ipfs/')
					? auction.ipfsHandle.replace('/ipfs/', '')
					: auction?.ipfsHandle;
			if (!cid) throw new Error('Missing IPFS handle');
			const bytes = await getFile(cid);
			fetchedBytes = bytes;
			progress = 30;

			// compute encrypted payload hash
			encryptedHash = await sha256Hex(fetchedBytes);
			status = 'Verifying encrypted payload checksum...';
			progress = 45;
			const expectedEnc = (auction?.hashedFileHash ?? '').replace(/^0x/, '');
			if (expectedEnc && encryptedHash !== expectedEnc) {
				error = `Encrypted payload hash mismatch (expected ${expectedEnc.slice(0, 8)}..., got ${encryptedHash.slice(0, 8)}...)`;
				status = '';
				return;
			}
			progress = 60;

			// request passphrase if not known
			if (!passphrase) {
				status = 'Waiting for passphrase...';
				return;
			}

			status = 'Decrypting payload...';
			progress = 70;

			// parse payload: salt(16) | iv(12) | cipher
			const payload = fetchedBytes;
			if (payload.length < 28) throw new Error('Encrypted payload too small');
			const salt = payload.slice(0, 16);
			const iv = payload.slice(16, 28);
			const cipher = payload.slice(28);

			const decrypted = await decryptWithPassphrase(cipher.buffer, passphrase, iv, salt);
			decryptedBytes = decrypted;
			progress = 85;

			// verify original file hash
			status = 'Verifying original file checksum...';
			originalHash = await sha256Hex(decryptedBytes);
			const expectedOrig = (auction?.originalFileHash ?? '').replace(/^0x/, '');
			if (expectedOrig && originalHash !== expectedOrig) {
				error = `Original file hash mismatch (expected ${expectedOrig.slice(0, 8)}..., got ${originalHash.slice(0, 8)}...)`;
				status = '';
				return;
			}

			progress = 100;
			status = 'Ready: decrypted file available';
			autoOk = true;
		} catch (err) {
			error = err instanceof Error ? err.message : String(err);
			status = '';
		}
	}

	function downloadDecrypted() {
		if (!decryptedBytes) return;
		const blob = new Blob([decryptedBytes as unknown as ArrayBuffer]);
		const url = URL.createObjectURL(blob);
		const a = document.createElement('a');
		a.href = url;
		const safe = (auction?.name ?? auction?.address ?? 'decrypted').replace(
			/[^a-zA-Z0-9._-]/g,
			'_'
		);
		a.download = `${safe}`;
		document.body.appendChild(a);
		a.click();
		a.remove();
		URL.revokeObjectURL(url);
		status = 'Downloaded';
		// clear decrypted from memory
		decryptedBytes = null;
		fetchedBytes = null;
		progress = 0;
		open = false;
	}
</script>

{#if open}
	<div class="fixed inset-0 z-50 flex items-center justify-center bg-slate-950/60 px-4">
		<div class="w-full max-w-2xl rounded-2xl bg-white p-6 shadow-2xl">
			<div class="flex items-start justify-between gap-4">
				<div>
					<h2 class="text-xl font-semibold text-slate-950">Retrieve File</h2>
					<p class="text-sm text-slate-600">Choose Manual or Automatic retrieval</p>
				</div>
				<button class="text-slate-700" onclick={close}>Close</button>
			</div>

			<div class="mt-4 grid grid-cols-1 gap-4">
				<div class="flex gap-2">
					<button
						class="rounded-md px-3 py-1"
						class:font-semibold={activeTab === 'automatic'}
						onclick={() => (activeTab = 'automatic')}>Automatic</button
					>
					<button
						class="rounded-md px-3 py-1"
						class:font-semibold={activeTab === 'manual'}
						onclick={() => (activeTab = 'manual')}>Manual</button
					>
				</div>

				{#if activeTab === 'manual'}
					<div class="prose text-sm">
						<h3>Manual retrieval</h3>
						<ol>
							<li>
								Download encrypted payload from IPFS: <a
									href={ipfsToGatewayUrl(auction?.ipfsHandle)}
									target="_blank"
									rel="noreferrer">{auction?.ipfsHandle}</a
								>
							</li>
							<li>
								Compute checksum: <code>sha256sum &lt;file&gt;</code> and compare with encrypted payload
								hash below.
							</li>
							<li>Decrypt locally using your passphrase (example using node):</li>
							<p class="text-sm">Download helper: <a href="/decrypt_helper.js" download class="text-blue-600 underline">decrypt_helper.js</a></p>
							<pre><code>node decrypt_helper.js &lt;encrypted&gt; &lt;passphrase&gt; &gt; decrypted.bin</code></pre>
							<li>Compute checksum of decrypted file and compare with original file hash below.</li>
						</ol>

						<div class="mt-3">
							<p><strong>Encrypted payload hash:</strong> {manualChecksumDisplay().encrypted}</p>
							<p><strong>Original file hash:</strong> {manualChecksumDisplay().original}</p>
						</div>
					</div>
				{:else}
					<div>
						<div class="grid gap-2">
							<label for="passphrase-input" class="text-sm font-medium">Passphrase</label>
							<input
								id="passphrase-input"
								class="rounded-md border px-2 py-1"
								bind:value={passphrase}
								placeholder="Enter passphrase"
							/>
						</div>

						<div class="mt-4">
							<p class="text-sm text-slate-600">Status: {status}</p>
							{#if error}
								<p class="text-sm text-red-600">{error}</p>
							{/if}
							<div class="mt-3">
								<Button onclick={startAutomatic}>Fetch & Verify</Button>
								{#if autoOk}
									<Button onclick={downloadDecrypted}>Download decrypted file</Button>
								{/if}
							</div>
							{#if progress}
								<div class="mt-3 w-full rounded bg-slate-100">
									<div class="h-2 rounded bg-sky-600" style={`width: ${progress}%`}></div>
								</div>
							{/if}
						</div>
					</div>
				{/if}
			</div>
		</div>
	</div>
{/if}
