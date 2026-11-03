<script lang="ts">
	import {
		calculateTrackMetrics,
		filterTrackPoints,
		generateGpx,
		parseNmeaText,
		SAMPLE_GPS_TRACK,
		SAMPLE_MULTI_GNSS_TRACK,
		type FilterSettings,
		type TrackPoint
	} from '../lib';

	// Input state
	let rawText = $state(SAMPLE_GPS_TRACK);
	let trackName = $state('My NMEA Track');
	let outputFormat = $state<'gpx' | 'kml'>('gpx');
	let showRawText = $state(false);

	// Filter settings
	let minTimeDeltaSeconds = $state(0);
	let minBearingDeltaDegrees = $state(0);
	let minDistanceMeters = $state(0);
	let gnssFilter = $state<FilterSettings['gnssFilter']>('ALL');
	let onlyValidFixes = $state(true);

	// Export toggles
	let includeExtensions = $state(true);
	let includeSatellites = $state(true);

	// Notification message
	let copyNotice = $state('');

	// Reactive parsing pipeline using Svelte 5 runes
	const parseResult = $derived.by(() => {
		if (!rawText.trim()) {
			return {
				points: [] as TrackPoint[],
				stats: {
					totalSentences: 0,
					validChecksums: 0,
					invalidChecksums: 0,
					totalRawPoints: 0,
					filteredPoints: 0,
					totalDistanceMeters: 0,
					durationSeconds: 0,
					talkerCounts: {},
					sentenceCounts: {}
				},
				sentences: []
			};
		}
		return parseNmeaText(rawText);
	});

	const filterSettings = $derived<FilterSettings>({
		minTimeDeltaSeconds,
		minBearingDeltaDegrees,
		minDistanceMeters,
		gnssFilter,
		onlyValidFixes
	});

	const filteredPoints = $derived.by<TrackPoint[]>(() => {
		return filterTrackPoints(parseResult.points, filterSettings);
	});

	const metrics = $derived.by(() => {
		return calculateTrackMetrics(filteredPoints, parseResult.stats);
	});

	const gpxXml = $derived.by(() => {
		return generateGpx(filteredPoints, {
			trackName,
			includeExtensions,
			includeSatellites
		});
	});

	const activeXmlOutput = $derived(gpxXml);

	// SVG Map path calculation
	const svgBounds = $derived.by(() => {
		if (filteredPoints.length === 0) return null;
		let minLat = Infinity,
			maxLat = -Infinity,
			minLon = Infinity,
			maxLon = -Infinity;
		for (const p of filteredPoints) {
			if (p.lat < minLat) minLat = p.lat;
			if (p.lat > maxLat) maxLat = p.lat;
			if (p.lon < minLon) minLon = p.lon;
			if (p.lon > maxLon) maxLon = p.lon;
		}

		// Add small padding to bounds
		const latPad = (maxLat - minLat) * 0.1 || 0.0005;
		const lonPad = (maxLon - minLon) * 0.1 || 0.0005;

		return {
			minLat: minLat - latPad,
			maxLat: maxLat + latPad,
			minLon: minLon - lonPad,
			maxLon: maxLon + lonPad
		};
	});

	function toSvgCoords(lat: number, lon: number, bounds: NonNullable<typeof svgBounds>) {
		const width = 600;
		const height = 300;
		const x = ((lon - bounds.minLon) / (bounds.maxLon - bounds.minLon)) * width;
		// Flip y because SVG 0 is top
		const y = height - ((lat - bounds.minLat) / (bounds.maxLat - bounds.minLat)) * height;
		return { x, y };
	}

	const svgPathData = $derived.by(() => {
		const b = svgBounds;
		if (!b || filteredPoints.length === 0) return '';
		return filteredPoints
			.map((p: TrackPoint, idx: number) => {
				const { x, y } = toSvgCoords(p.lat, p.lon, b);
				return `${idx === 0 ? 'M' : 'L'} ${x.toFixed(1)} ${y.toFixed(1)}`;
			})
			.join(' ');
	});

	// Handlers
	function handleFileUpload(event: Event) {
		const target = event.target as HTMLInputElement;
		const file = target.files?.[0];
		if (!file) return;

		trackName = file.name.replace(/\.[^/.]+$/, '');
		const reader = new FileReader();
		reader.onload = (e) => {
			rawText = (e.target?.result as string) || '';
		};
		reader.readAsText(file);
	}

	async function loadStaticSample(filename: string, name: string) {
		try {
			const res = await fetch(`/${filename}`);
			if (res.ok) {
				rawText = await res.text();
				trackName = name;
			}
		} catch (err) {
			console.error('Failed to load sample:', err);
		}
	}

	function downloadFile() {
		const ext = outputFormat;
		const filename = `${trackName.replace(/[^a-zA-Z0-9_-]/g, '_') || 'track'}.${ext}`;
		const mimeType = ext === 'gpx' ? 'application/gpx+xml' : 'application/vnd.google-earth.kml+xml';
		const blob = new Blob([activeXmlOutput], { type: mimeType });
		const url = URL.createObjectURL(blob);
		const a = document.createElement('a');
		a.href = url;
		a.download = filename;
		document.body.appendChild(a);
		a.click();
		document.body.removeChild(a);
		URL.revokeObjectURL(url);
	}

	async function copyToClipboard() {
		try {
			await navigator.clipboard.writeText(activeXmlOutput);
			copyNotice = 'Copied to clipboard!';
			setTimeout(() => {
				copyNotice = '';
			}, 2500);
		} catch (err) {
			copyNotice = 'Failed to copy';
		}
	}
</script>

<svelte:head>
	<title>NMEA to GPX / KML Converter</title>
</svelte:head>

<main
	style="max-width: 1000px; margin: 20px auto; padding: 0 16px; font-family: sans-serif; color: #222;"
>
	<header style="border-bottom: 2px solid #ccc; padding-bottom: 12px; margin-bottom: 20px;">
		<h1 style="margin: 0 0 6px 0; font-size: 1.6rem;">NMEA 0183 to GPX / KML Converter</h1>
	</header>

	<!-- Source Selection & Presets -->
	<section
		style="background: #f8f9fa; border: 1px solid #ddd; padding: 14px; border-radius: 6px; margin-bottom: 20px;"
	>
		<h3 style="margin-top: 0; margin-bottom: 10px; font-size: 1.1rem;">1. Select Data Source</h3>
		<div
			style="display: flex; flex-wrap: wrap; gap: 10px; align-items: center; margin-bottom: 12px;"
		>
			<label style="font-weight: bold;">
				Upload NMEA File:
				<input
					type="file"
					accept=".nmea,.txt,.log"
					onchange={handleFileUpload}
					style="margin-left: 8px;"
				/>
			</label>
			<span style="color: #888;">or load presets:</span>
			<button
				type="button"
				onclick={() => {
					rawText = SAMPLE_GPS_TRACK;
					trackName = 'Sample GPS Track';
				}}
				style="padding: 5px 10px; cursor: pointer;"
			>
				Small GPS Sample
			</button>
			<button
				type="button"
				onclick={() => {
					rawText = SAMPLE_MULTI_GNSS_TRACK;
					trackName = 'Sample Multi-GNSS Track';
				}}
				style="padding: 5px 10px; cursor: pointer;"
			>
				Small Multi-GNSS Sample
			</button>
			<button
				type="button"
				onclick={() => loadStaticSample('fei-2026-10-20-2.nmea', 'FEI 2026-10-20 Track')}
				style="padding: 5px 10px; cursor: pointer;"
			>
				FEI VSB Track (1.2 MB)
			</button>
			<button
				type="button"
				onclick={() => loadStaticSample('2015-08-12.nmea', '2015-08-12 Multi-GNSS Track')}
				style="padding: 5px 10px; cursor: pointer;"
			>
				Multi-GNSS Track (2.2 MB)
			</button>
		</div>

		<div>
			<label
				style="display: inline-flex; align-items: center; gap: 6px; cursor: pointer; font-size: 0.9rem;"
			>
				<input type="checkbox" bind:checked={showRawText} />
				Show / edit raw NMEA text ({rawText.split('\n').filter(Boolean).length} lines)
			</label>
			{#if showRawText}
				<textarea
					bind:value={rawText}
					rows="8"
					style="width: 100%; box-sizing: border-box; font-family: monospace; font-size: 0.85rem; margin-top: 8px; padding: 8px;"
					placeholder="Paste $GPRMC, $GPGGA sentences here..."></textarea>
			{/if}
		</div>
	</section>

	<!-- Filters & GNSS Selection -->
	<section
		style="background: #f8f9fa; border: 1px solid #ddd; padding: 14px; border-radius: 6px; margin-bottom: 20px;"
	>
		<h3 style="margin-top: 0; margin-bottom: 12px; font-size: 1.1rem;">
			2. Filters & GNSS Selection
		</h3>

		<div
			style="display: grid; grid-template-columns: repeat(auto-fit, minmax(220px, 1fr)); gap: 14px; margin-bottom: 14px;"
		>
			<div>
				<label
					for="time-delta"
					style="display: block; font-weight: bold; margin-bottom: 4px; font-size: 0.9rem;"
				>
					Min Time Delta (seconds):
				</label>
				<input
					id="time-delta"
					type="number"
					min="0"
					step="1"
					bind:value={minTimeDeltaSeconds}
					style="width: 100%; padding: 6px; box-sizing: border-box;"
				/>
				<span style="font-size: 0.8rem; color: #666;"
					>Filter out points closer in time than X sec.</span
				>
			</div>

			<div>
				<label
					for="bearing-delta"
					style="display: block; font-weight: bold; margin-bottom: 4px; font-size: 0.9rem;"
				>
					Min Bearing Change (degrees):
				</label>
				<input
					id="bearing-delta"
					type="number"
					min="0"
					max="180"
					step="1"
					bind:value={minBearingDeltaDegrees}
					style="width: 100%; padding: 6px; box-sizing: border-box;"
				/>
				<span style="font-size: 0.8rem; color: #666;"
					>Filter out points with direction change &lt; Y deg.</span
				>
			</div>

			<div>
				<label
					for="dist-delta"
					style="display: block; font-weight: bold; margin-bottom: 4px; font-size: 0.9rem;"
				>
					Min Distance Delta (meters):
				</label>
				<input
					id="dist-delta"
					type="number"
					min="0"
					step="1"
					bind:value={minDistanceMeters}
					style="width: 100%; padding: 6px; box-sizing: border-box;"
				/>
				<span style="font-size: 0.8rem; color: #666;"
					>Filter points closer than Z meters (Haversine).</span
				>
			</div>

			<div>
				<label
					for="gnss-filter"
					style="display: block; font-weight: bold; margin-bottom: 4px; font-size: 0.9rem;"
				>
					GNSS Constellation Filter:
				</label>
				<select
					id="gnss-filter"
					bind:value={gnssFilter}
					style="width: 100%; padding: 6px; box-sizing: border-box;"
				>
					<option value="ALL">All GNSS (GPS, GLONASS, Galileo, BeiDou, Combined)</option>
					<option value="GP">GPS only ($GP)</option>
					<option value="GL">GLONASS only ($GL)</option>
					<option value="GA">Galileo only ($GA)</option>
					<option value="BD">BeiDou only ($BD)</option>
				</select>
				<span style="font-size: 0.8rem; color: #666;">Include only specific satellite system.</span>
			</div>
		</div>

		<div
			style="display: flex; flex-wrap: wrap; gap: 16px; align-items: center; padding-top: 8px; border-top: 1px solid #e0e0e0; font-size: 0.9rem;"
		>
			<label style="display: inline-flex; align-items: center; gap: 6px; cursor: pointer;">
				<input type="checkbox" bind:checked={onlyValidFixes} />
				Only include points with valid fix
			</label>
			<label style="display: inline-flex; align-items: center; gap: 6px; cursor: pointer;">
				<input type="checkbox" bind:checked={includeExtensions} />
				Export speed & course extensions (nmea:speed, nmea:course)
			</label>
			<label style="display: inline-flex; align-items: center; gap: 6px; cursor: pointer;">
				<input type="checkbox" bind:checked={includeSatellites} />
				Export satellite details (PRN, elevation, azimuth, SNR)
			</label>
		</div>
	</section>

	<!-- Track Metrics & Stats -->
	<section
		style="background: #fff; border: 1px solid #ddd; padding: 14px; border-radius: 6px; margin-bottom: 20px;"
	>
		<h3 style="margin-top: 0; margin-bottom: 10px; font-size: 1.1rem;">3. Track Statistics</h3>
		<div
			style="display: grid; grid-template-columns: repeat(auto-fit, minmax(160px, 1fr)); gap: 12px; margin-bottom: 12px;"
		>
			<div style="background: #f0f4f8; padding: 10px; border-radius: 4px;">
				<div style="font-size: 0.8rem; color: #666;">Raw / Filtered Points</div>
				<div style="font-size: 1.25rem; font-weight: bold;">
					{metrics.filteredPoints}
					<span style="font-size: 0.85rem; font-weight: normal; color: #666;"
						>/ {metrics.totalRawPoints}</span
					>
				</div>
				{#if metrics.totalRawPoints > 0}
					<div style="font-size: 0.75rem; color: #888;">
						Reduction: {Math.round((1 - metrics.filteredPoints / metrics.totalRawPoints) * 100)}%
					</div>
				{/if}
			</div>

			<div style="background: #f0f4f8; padding: 10px; border-radius: 4px;">
				<div style="font-size: 0.8rem; color: #666;">Total Distance</div>
				<div style="font-size: 1.25rem; font-weight: bold;">
					{#if metrics.totalDistanceMeters >= 1000}
						{(metrics.totalDistanceMeters / 1000).toFixed(2)} km
					{:else}
						{metrics.totalDistanceMeters.toFixed(0)} m
					{/if}
				</div>
			</div>

			<div style="background: #f0f4f8; padding: 10px; border-radius: 4px;">
				<div style="font-size: 0.8rem; color: #666;">Duration</div>
				<div style="font-size: 1.25rem; font-weight: bold;">
					{Math.floor(metrics.durationSeconds / 60)}m {metrics.durationSeconds % 60}s
				</div>
			</div>

			<div style="background: #f0f4f8; padding: 10px; border-radius: 4px;">
				<div style="font-size: 0.8rem; color: #666;">Elevation Range</div>
				<div style="font-size: 1.25rem; font-weight: bold;">
					{#if metrics.minElevation !== undefined && metrics.maxElevation !== undefined}
						{metrics.minElevation.toFixed(1)} – {metrics.maxElevation.toFixed(1)} m
					{:else}
						N/A
					{/if}
				</div>
			</div>

			<div style="background: #f0f4f8; padding: 10px; border-radius: 4px;">
				<div style="font-size: 0.8rem; color: #666;">Max / Avg Speed</div>
				<div style="font-size: 1.25rem; font-weight: bold;">
					{#if metrics.maxSpeedMps !== undefined}
						{metrics.maxSpeedMps.toFixed(1)}
						<span style="font-size: 0.85rem; font-weight: normal; color: #666;">m/s</span>
					{:else}
						N/A
					{/if}
				</div>
			</div>

			<div style="background: #f0f4f8; padding: 10px; border-radius: 4px;">
				<div style="font-size: 0.8rem; color: #666;">Checksum Valid / Error</div>
				<div
					style="font-size: 1.25rem; font-weight: bold; color: {metrics.invalidChecksums > 0
						? '#b91c1c'
						: '#15803d'};"
				>
					{metrics.validChecksums}
					<span style="font-size: 0.85rem; font-weight: normal; color: #666;"
						>/ {metrics.invalidChecksums} err</span
					>
				</div>
			</div>
		</div>

		<!-- Talkers breakdown -->
		{#if Object.keys(metrics.talkerCounts).length > 0}
			<div style="font-size: 0.85rem; color: #555;">
				<strong>Talker distribution:</strong>
				{#each Object.entries(metrics.talkerCounts) as [talker, count]}
					<span
						style="display: inline-block; background: #eee; padding: 2px 6px; border-radius: 3px; margin: 2px 4px;"
					>
						{talker}: {count}
					</span>
				{/each}
			</div>
		{/if}
	</section>

	<!-- Visual Map / SVG Path Preview -->
	{#if filteredPoints.length > 1 && svgBounds}
		<section
			style="background: #fff; border: 1px solid #ddd; padding: 14px; border-radius: 6px; margin-bottom: 20px;"
		>
			<h3 style="margin-top: 0; margin-bottom: 10px; font-size: 1.1rem;">
				4. Track Route Visualizer
			</h3>
			<div
				style="width: 100%; border: 1px solid #ccc; background: #fafafa; border-radius: 4px; overflow: hidden;"
			>
				<svg viewBox="0 0 600 300" style="width: 100%; height: auto; display: block;">
					<!-- Track line -->
					<path
						d={svgPathData}
						fill="none"
						stroke="#2563eb"
						stroke-width="3"
						stroke-linejoin="round"
						stroke-linecap="round"
					/>

					<!-- Start point (green circle) -->
					{#if filteredPoints[0]}
						{@const startPos = toSvgCoords(filteredPoints[0].lat, filteredPoints[0].lon, svgBounds)}
						<circle
							cx={startPos.x}
							cy={startPos.y}
							r="6"
							fill="#16a34a"
							stroke="#ffffff"
							stroke-width="2"
						/>
						<text
							x={startPos.x + 8}
							y={startPos.y + 4}
							font-size="11"
							fill="#16a34a"
							font-weight="bold">Start</text
						>
					{/if}

					<!-- End point (red circle) -->
					{#if filteredPoints.length > 1}
						{@const endPos = toSvgCoords(
							filteredPoints[filteredPoints.length - 1].lat,
							filteredPoints[filteredPoints.length - 1].lon,
							svgBounds
						)}
						<circle
							cx={endPos.x}
							cy={endPos.y}
							r="6"
							fill="#dc2626"
							stroke="#ffffff"
							stroke-width="2"
						/>
						<text x={endPos.x + 8} y={endPos.y + 4} font-size="11" fill="#dc2626" font-weight="bold"
							>End</text
						>
					{/if}
				</svg>
			</div>
			<div
				style="font-size: 0.8rem; color: #666; margin-top: 6px; display: flex; justify-content: space-between;"
			>
				<span>Lat bounds: {svgBounds.minLat.toFixed(5)}° to {svgBounds.maxLat.toFixed(5)}°</span>
				<span>Lon bounds: {svgBounds.minLon.toFixed(5)}° to {svgBounds.maxLon.toFixed(5)}°</span>
			</div>
		</section>
	{/if}

	<!-- Export Options & XML Preview -->
	<section
		style="background: #f8f9fa; border: 1px solid #ddd; padding: 14px; border-radius: 6px; margin-bottom: 20px;"
	>
		<div
			style="display: flex; flex-wrap: wrap; justify-content: space-between; align-items: center; margin-bottom: 12px; gap: 10px;"
		>
			<h3 style="margin: 0; font-size: 1.1rem;">5. Export & Output Preview</h3>

			<div style="display: flex; gap: 8px; align-items: center;">
				<label style="font-weight: bold; font-size: 0.9rem;">
					Track Name:
					<input type="text" bind:value={trackName} style="padding: 4px 8px; margin-left: 4px;" />
				</label>

				<label style="font-weight: bold; font-size: 0.9rem;"> Format: GPX 1.1 </label>
			</div>
		</div>

		<div style="display: flex; gap: 10px; margin-bottom: 12px;">
			<button
				type="button"
				onclick={downloadFile}
				disabled={filteredPoints.length === 0}
				style="padding: 8px 16px; font-weight: bold; background: #2563eb; color: #fff; border: none; border-radius: 4px; cursor: pointer;"
			>
				Download .{outputFormat.toUpperCase()} File
			</button>

			<button
				type="button"
				onclick={copyToClipboard}
				disabled={filteredPoints.length === 0}
				style="padding: 8px 16px; background: #e5e7eb; border: 1px solid #ccc; border-radius: 4px; cursor: pointer;"
			>
				Copy XML to Clipboard
			</button>

			{#if copyNotice}
				<span style="color: #16a34a; font-size: 0.9rem; align-self: center; font-weight: bold;">
					{copyNotice}
				</span>
			{/if}
		</div>

		<div>
			<label
				for="xml-preview"
				style="display: block; font-size: 0.85rem; font-weight: bold; margin-bottom: 4px;"
			>
				Generated {outputFormat.toUpperCase()} XML ({activeXmlOutput.split('\n').length} lines):
			</label>
			<textarea
				id="xml-preview"
				readonly
				value={activeXmlOutput}
				rows="14"
				style="width: 100%; box-sizing: border-box; font-family: monospace; font-size: 0.8rem; background: #ffffff; padding: 10px; border: 1px solid #ccc; border-radius: 4px;"
			></textarea>
		</div>
	</section>

	<!-- Sample Points Table Preview -->
	{#if filteredPoints.length > 0}
		<section
			style="background: #fff; border: 1px solid #ddd; padding: 14px; border-radius: 6px; margin-bottom: 30px;"
		>
			<h3 style="margin-top: 0; margin-bottom: 10px; font-size: 1.1rem;">
				6. Sample Filtered Points (first {Math.min(filteredPoints.length, 15)} of {filteredPoints.length})
			</h3>
			<div style="overflow-x: auto;">
				<table
					style="width: 100%; border-collapse: collapse; font-size: 0.85rem; text-align: left;"
				>
					<thead>
						<tr style="background: #f1f5f9; border-bottom: 2px solid #cbd5e1;">
							<th style="padding: 6px 8px;">#</th>
							<th style="padding: 6px 8px;">Time (UTC)</th>
							<th style="padding: 6px 8px;">Latitude</th>
							<th style="padding: 6px 8px;">Longitude</th>
							<th style="padding: 6px 8px;">Elevation</th>
							<th style="padding: 6px 8px;">Fix</th>
							<th style="padding: 6px 8px;">Sats</th>
							<th style="padding: 6px 8px;">HDOP</th>
							<th style="padding: 6px 8px;">Speed (m/s)</th>
							<th style="padding: 6px 8px;">Course</th>
						</tr>
					</thead>
					<tbody>
						{#each filteredPoints.slice(0, 15) as pt, idx}
							<tr
								style="border-bottom: 1px solid #e2e8f0; background: {idx % 2 === 0
									? '#ffffff'
									: '#f8fafc'};"
							>
								<td style="padding: 6px 8px;">{idx + 1}</td>
								<td style="padding: 6px 8px; font-family: monospace;"
									>{pt.isoTime || pt.utcTime || '—'}</td
								>
								<td style="padding: 6px 8px; font-family: monospace;">{pt.lat.toFixed(6)}°</td>
								<td style="padding: 6px 8px; font-family: monospace;">{pt.lon.toFixed(6)}°</td>
								<td style="padding: 6px 8px;"
									>{pt.ele !== undefined ? `${pt.ele.toFixed(1)} m` : '—'}</td
								>
								<td style="padding: 6px 8px;">{pt.fix || '—'}</td>
								<td style="padding: 6px 8px;">{pt.satCount ?? pt.satellites.length ?? '—'}</td>
								<td style="padding: 6px 8px;">{pt.hdop?.toFixed(2) ?? '—'}</td>
								<td style="padding: 6px 8px;"
									>{pt.speedMps !== undefined ? pt.speedMps.toFixed(2) : '—'}</td
								>
								<td style="padding: 6px 8px;"
									>{pt.courseDeg !== undefined ? `${pt.courseDeg.toFixed(1)}°` : '—'}</td
								>
							</tr>
						{/each}
					</tbody>
				</table>
			</div>
		</section>
	{/if}
</main>
