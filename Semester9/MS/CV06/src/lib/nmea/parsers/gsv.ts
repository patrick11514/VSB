import type { GsvData, SatelliteInfo, TalkerId } from '../types';

function getConstellationName(talker: TalkerId, prn: number): string {
	switch (talker) {
		case 'GP':
			return 'GPS';
		case 'GL':
			return 'GLONASS';
		case 'GA':
			return 'Galileo';
		case 'BD':
		case 'GB':
			return 'BeiDou';
		default:
			// If talker is GN, PRN ranges can sometimes differentiate
			if (prn >= 1 && prn <= 32) return 'GPS';
			if (prn >= 65 && prn <= 96) return 'GLONASS';
			if (prn >= 101 && prn <= 136) return 'Galileo';
			return 'GNSS';
	}
}

/**
 * Parses a single NMEA GSV sentence fields.
 */
export function parseGsv(fields: string[], talker: TalkerId): GsvData | null {
	if (fields.length < 4) {
		return null;
	}

	const totalMessages = parseInt(fields[1]?.trim() || '1', 10) || 1;
	const messageNumber = parseInt(fields[2]?.trim() || '1', 10) || 1;
	const totalSatellites = parseInt(fields[3]?.trim() || '0', 10) || 0;

	const satellites: SatelliteInfo[] = [];

	// Each satellite has 4 fields: PRN, elevation, azimuth, snr
	for (let i = 4; i + 3 < fields.length; i += 4) {
		const prnStr = fields[i]?.trim();
		if (!prnStr) continue;

		const prn = parseInt(prnStr, 10);
		if (isNaN(prn)) continue;

		const eleStr = fields[i + 1]?.trim();
		const aziStr = fields[i + 2]?.trim();
		const snrStr = fields[i + 3]?.trim();

		const elevation = eleStr ? parseInt(eleStr, 10) : undefined;
		const azimuth = aziStr ? parseInt(aziStr, 10) : undefined;
		const snr = snrStr ? parseInt(snrStr, 10) : undefined;

		satellites.push({
			prn,
			elevation: isNaN(elevation ?? NaN) ? undefined : elevation,
			azimuth: isNaN(azimuth ?? NaN) ? undefined : azimuth,
			snr: isNaN(snr ?? NaN) ? undefined : snr,
			system: getConstellationName(talker, prn)
		});
	}

	return {
		totalMessages,
		messageNumber,
		totalSatellites,
		satellites
	};
}
