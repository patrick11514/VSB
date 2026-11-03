import { parseTimestampToMillis } from './coordinates';
import { angularDifference, calculateBearing, haversineDistance } from './geo';
import type { FilterSettings, TrackPoint, TrackStats } from './types';

/**
 * Checks if a point matches the selected GNSS constellation filter.
 */
export function matchesGnssFilter(point: TrackPoint, filter: FilterSettings['gnssFilter']): boolean {
	if (filter === 'ALL') return true;

	// Check if talker directly matches
	if (point.talker === filter) return true;

	// If talker was GN (combined receiver), check if satellites contain target system
	if (point.talker === 'GN') {
		const targetSystem =
			filter === 'GP'
				? 'GPS'
				: filter === 'GL'
					? 'GLONASS'
					: filter === 'GA'
						? 'Galileo'
						: filter === 'BD'
							? 'BeiDou'
							: '';
		return point.satellites.some((s) => s.system === targetSystem);
	}

	return false;
}

/**
 * Applies spatial, temporal, and constellation filters to raw track points.
 */
export function filterTrackPoints(
	rawPoints: TrackPoint[],
	settings: FilterSettings
): TrackPoint[] {
	if (rawPoints.length === 0) return [];

	const filtered: TrackPoint[] = [];
	let lastExported: TrackPoint | null = null;
	let lastExportedMillis = 0;
	let lastExportedBearing: number | undefined = undefined;

	for (const pt of rawPoints) {
		// Fix validity filter
		if (settings.onlyValidFixes && pt.fix === 'none') {
			continue;
		}

		// GNSS constellation filter
		if (!matchesGnssFilter(pt, settings.gnssFilter)) {
			continue;
		}

		// First point always passes if it meets constellation & fix criteria
		if (!lastExported) {
			filtered.push(pt);
			lastExported = pt;
			lastExportedMillis = parseTimestampToMillis(pt.dateStr, pt.utcTime);
			lastExportedBearing = pt.courseDeg;
			continue;
		}

		const currentMillis = parseTimestampToMillis(pt.dateStr, pt.utcTime);
		const timeDeltaSeconds =
			lastExportedMillis && currentMillis
				? Math.abs(currentMillis - lastExportedMillis) / 1000
				: 0;

		const distanceMeters = haversineDistance(
			lastExported.lat,
			lastExported.lon,
			pt.lat,
			pt.lon
		);

		// Determine bearing: from point's courseDeg or computed geodesic bearing
		const currentBearing =
			pt.courseDeg !== undefined
				? pt.courseDeg
				: calculateBearing(lastExported.lat, lastExported.lon, pt.lat, pt.lon);

		const bearingDeltaDegrees =
			lastExportedBearing !== undefined
				? angularDifference(lastExportedBearing, currentBearing)
				: 0;

		// Distance check
		const satisfiesDistance =
			settings.minDistanceMeters <= 0 || distanceMeters >= settings.minDistanceMeters;

		// Time change check
		const satisfiesTime =
			settings.minTimeDeltaSeconds <= 0 || timeDeltaSeconds >= settings.minTimeDeltaSeconds;

		// Direction change check
		const satisfiesBearing =
			settings.minBearingDeltaDegrees <= 0 || bearingDeltaDegrees >= settings.minBearingDeltaDegrees;

		// A point is accepted if:
		// 1. Distance constraint is satisfied (if specified), AND
		// 2. Either time delta OR bearing delta is satisfied (if either is specified)
		const requiresTimeOrBearing =
			settings.minTimeDeltaSeconds > 0 || settings.minBearingDeltaDegrees > 0;

		const satisfiesTimeOrBearing =
			!requiresTimeOrBearing ||
			(settings.minTimeDeltaSeconds > 0 && satisfiesTime) ||
			(settings.minBearingDeltaDegrees > 0 && satisfiesBearing);

		if (satisfiesDistance && satisfiesTimeOrBearing) {
			filtered.push(pt);
			lastExported = pt;
			lastExportedMillis = currentMillis;
			lastExportedBearing = currentBearing;
		}
	}

	return filtered;
}

/**
 * Calculates track summary metrics on filtered points.
 */
export function calculateTrackMetrics(
	points: TrackPoint[],
	baseStats: TrackStats
): TrackStats {
	let totalDist = 0;
	let maxSpeed = 0;
	let speedSum = 0;
	let speedCount = 0;
	let minEle: number | undefined = undefined;
	let maxEle: number | undefined = undefined;

	for (let i = 0; i < points.length; i++) {
		const pt = points[i];

		if (i > 0) {
			const prev = points[i - 1];
			totalDist += haversineDistance(prev.lat, prev.lon, pt.lat, pt.lon);
		}

		if (pt.speedMps !== undefined) {
			speedSum += pt.speedMps;
			speedCount++;
			if (pt.speedMps > maxSpeed) {
				maxSpeed = pt.speedMps;
			}
		}

		if (pt.ele !== undefined) {
			if (minEle === undefined || pt.ele < minEle) minEle = pt.ele;
			if (maxEle === undefined || pt.ele > maxEle) maxEle = pt.ele;
		}
	}

	let durationSeconds = 0;
	if (points.length > 1) {
		const firstMillis = parseTimestampToMillis(points[0].dateStr, points[0].utcTime);
		const lastMillis = parseTimestampToMillis(
			points[points.length - 1].dateStr,
			points[points.length - 1].utcTime
		);
		if (firstMillis && lastMillis && lastMillis > firstMillis) {
			durationSeconds = Math.round((lastMillis - firstMillis) / 1000);
		}
	}

	return {
		...baseStats,
		filteredPoints: points.length,
		totalDistanceMeters: Math.round(totalDist * 10) / 10,
		durationSeconds,
		avgSpeedMps: speedCount > 0 ? Math.round((speedSum / speedCount) * 100) / 100 : undefined,
		maxSpeedMps: speedCount > 0 ? Math.round(maxSpeed * 100) / 100 : undefined,
		minElevation: minEle,
		maxElevation: maxEle
	};
}
