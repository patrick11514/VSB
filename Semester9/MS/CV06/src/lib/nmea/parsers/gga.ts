import { parseNmeaLatitude, parseNmeaLongitude } from '../coordinates';
import type { GgaData } from '../types';

/**
 * Parses NMEA GGA sentence fields into structured GgaData.
 * Input `fields` contains the comma-separated parts of the sentence (including header at index 0).
 */
export function parseGga(fields: string[]): GgaData | null {
	// Need at least up to field 6 (fix quality)
	if (fields.length < 7) {
		return null;
	}

	const utcTime = fields[1]?.trim() || '';
	const latStr = fields[2]?.trim() || '';
	const latHemi = fields[3]?.trim() || '';
	const lonStr = fields[4]?.trim() || '';
	const lonHemi = fields[5]?.trim() || '';
	const fixQualityStr = fields[6]?.trim() || '0';

	const latitude = parseNmeaLatitude(latStr, latHemi);
	const longitude = parseNmeaLongitude(lonStr, lonHemi);

	if (latitude === null || longitude === null) {
		return null;
	}

	const fixQuality = parseInt(fixQualityStr, 10) || 0;
	const satellitesUsed = parseInt(fields[7]?.trim() || '0', 10) || 0;

	const hdopStr = fields[8]?.trim();
	const hdop = hdopStr ? parseFloat(hdopStr) : undefined;

	const altStr = fields[9]?.trim();
	const altitudeMeters = altStr ? parseFloat(altStr) : undefined;

	const geoidStr = fields[11]?.trim();
	const geoidHeightMeters = geoidStr ? parseFloat(geoidStr) : undefined;

	return {
		utcTime,
		latitude,
		longitude,
		fixQuality,
		satellitesUsed,
		hdop: isNaN(hdop ?? NaN) ? undefined : hdop,
		altitudeMeters: isNaN(altitudeMeters ?? NaN) ? undefined : altitudeMeters,
		geoidHeightMeters: isNaN(geoidHeightMeters ?? NaN) ? undefined : geoidHeightMeters
	};
}
