/**
 * Parses NMEA latitude string (ddmm.mmmm) into signed decimal degrees.
 * E.g. '4949.85156', 'N' -> 49 + 49.85156 / 60 = 49.83085933333333
 */
export function parseNmeaLatitude(coordStr: string, hemisphere: string): number | null {
	if (!coordStr || !hemisphere) return null;
	const trimmed = coordStr.trim();
	const hemi = hemisphere.trim().toUpperCase();

	if (trimmed.length < 4 || (hemi !== 'N' && hemi !== 'S')) {
		return null;
	}

	const degStr = trimmed.substring(0, 2);
	const minStr = trimmed.substring(2);

	const degrees = parseFloat(degStr);
	const minutes = parseFloat(minStr);

	if (isNaN(degrees) || isNaN(minutes)) {
		return null;
	}

	let dec = degrees + minutes / 60;
	if (hemi === 'S') {
		dec = -dec;
	}
	return dec;
}

/**
 * Parses NMEA longitude string (dddmm.mmmm) into signed decimal degrees.
 * E.g. '01809.70573', 'E' -> 18 + 09.70573 / 60 = 18.161762166666665
 */
export function parseNmeaLongitude(coordStr: string, hemisphere: string): number | null {
	if (!coordStr || !hemisphere) return null;
	const trimmed = coordStr.trim();
	const hemi = hemisphere.trim().toUpperCase();

	if (trimmed.length < 5 || (hemi !== 'E' && hemi !== 'W')) {
		return null;
	}

	const degStr = trimmed.substring(0, 3);
	const minStr = trimmed.substring(3);

	const degrees = parseFloat(degStr);
	const minutes = parseFloat(minStr);

	if (isNaN(degrees) || isNaN(minutes)) {
		return null;
	}

	let dec = degrees + minutes / 60;
	if (hemi === 'W') {
		dec = -dec;
	}
	return dec;
}

/**
 * Parses NMEA time (hhmmss or hhmmss.ss) and optional date (ddmmyy) into ISO 8601 string.
 * Example: timeStr = '080019.00', dateStr = '201026' -> '2026-10-20T08:00:19.000Z'
 */
export function parseNmeaDateTime(dateStr?: string, timeStr?: string): string | undefined {
	if (!timeStr || timeStr.length < 6) return undefined;

	const hours = timeStr.substring(0, 2);
	const minutes = timeStr.substring(2, 4);
	const seconds = timeStr.substring(4, 6);
	const ms = timeStr.includes('.') ? timeStr.split('.')[1].padEnd(3, '0').substring(0, 3) : '000';

	let year = '1970';
	let month = '01';
	let day = '01';

	if (dateStr && dateStr.length === 6) {
		day = dateStr.substring(0, 2);
		month = dateStr.substring(2, 4);
		const rawYear = parseInt(dateStr.substring(4, 6), 10);
		// NMEA 2-digit year: 80-99 -> 1980-1999, 00-79 -> 2000-2079
		const fullYear = rawYear >= 80 ? 1900 + rawYear : 2000 + rawYear;
		year = fullYear.toString();
	}

	return `${year}-${month}-${day}T${hours}:${minutes}:${seconds}.${ms}Z`;
}

/**
 * Converts timestamp (hhmmss.ss) and optional date (ddmmyy) into epoch milliseconds.
 */
export function parseTimestampToMillis(dateStr?: string, timeStr?: string): number {
	const iso = parseNmeaDateTime(dateStr, timeStr);
	if (!iso) return 0;
	return new Date(iso).getTime();
}
