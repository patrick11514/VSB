import { parseNmeaLatitude, parseNmeaLongitude } from '../coordinates';
import { knotsToMps } from '../geo';
import type { RmcData } from '../types';

/**
 * Parses NMEA RMC sentence fields into structured RmcData.
 */
export function parseRmc(fields: string[]): RmcData | null {
	if (fields.length < 10) {
		return null;
	}

	const utcTime = fields[1]?.trim() || '';
	const statusRaw = fields[2]?.trim().toUpperCase();
	const status: 'A' | 'V' = statusRaw === 'A' ? 'A' : 'V';

	const latStr = fields[3]?.trim() || '';
	const latHemi = fields[4]?.trim() || '';
	const lonStr = fields[5]?.trim() || '';
	const lonHemi = fields[6]?.trim() || '';

	const latitude = parseNmeaLatitude(latStr, latHemi);
	const longitude = parseNmeaLongitude(lonStr, lonHemi);

	if (latitude === null || longitude === null) {
		return null;
	}

	const speedKnotsStr = fields[7]?.trim();
	const speedKnots = speedKnotsStr ? parseFloat(speedKnotsStr) : undefined;
	const speedMps = speedKnots !== undefined && !isNaN(speedKnots) ? knotsToMps(speedKnots) : undefined;

	const courseDegStr = fields[8]?.trim();
	const courseDeg = courseDegStr ? parseFloat(courseDegStr) : undefined;

	const dateStr = fields[9]?.trim() || undefined;

	const magVarStr = fields[10]?.trim();
	const magVarDir = fields[11]?.trim().toUpperCase();
	let magVarDeg: number | undefined = undefined;

	if (magVarStr && !isNaN(parseFloat(magVarStr))) {
		magVarDeg = parseFloat(magVarStr);
		if (magVarDir === 'W') {
			magVarDeg = -magVarDeg;
		}
	}

	return {
		utcTime,
		status,
		latitude,
		longitude,
		speedKnots: isNaN(speedKnots ?? NaN) ? undefined : speedKnots,
		speedMps: isNaN(speedMps ?? NaN) ? undefined : speedMps,
		courseDeg: isNaN(courseDeg ?? NaN) ? undefined : courseDeg,
		dateStr,
		magVarDeg
	};
}
