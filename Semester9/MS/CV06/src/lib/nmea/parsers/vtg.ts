import { knotsToMps } from '../geo';
import type { VtgData } from '../types';

/**
 * Parses NMEA VTG sentence fields into structured VtgData.
 */
export function parseVtg(fields: string[]): VtgData | null {
	if (fields.length < 9) {
		return null;
	}

	const courseTrueStr = fields[1]?.trim();
	const courseTrueDeg = courseTrueStr ? parseFloat(courseTrueStr) : undefined;

	const courseMagStr = fields[3]?.trim();
	const courseMagDeg = courseMagStr ? parseFloat(courseMagStr) : undefined;

	const speedKnotsStr = fields[5]?.trim();
	const speedKnots = speedKnotsStr ? parseFloat(speedKnotsStr) : undefined;

	const speedKmhStr = fields[7]?.trim();
	const speedKmh = speedKmhStr ? parseFloat(speedKmhStr) : undefined;

	let speedMps: number | undefined = undefined;
	if (speedKnots !== undefined && !isNaN(speedKnots)) {
		speedMps = knotsToMps(speedKnots);
	} else if (speedKmh !== undefined && !isNaN(speedKmh)) {
		speedMps = speedKmh / 3.6;
	}

	return {
		courseTrueDeg: isNaN(courseTrueDeg ?? NaN) ? undefined : courseTrueDeg,
		courseMagDeg: isNaN(courseMagDeg ?? NaN) ? undefined : courseMagDeg,
		speedKnots: isNaN(speedKnots ?? NaN) ? undefined : speedKnots,
		speedKmh: isNaN(speedKmh ?? NaN) ? undefined : speedKmh,
		speedMps
	};
}
