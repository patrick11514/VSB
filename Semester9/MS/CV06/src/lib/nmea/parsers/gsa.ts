import type { GsaData } from '../types';

/**
 * Parses NMEA GSA sentence fields into structured GsaData.
 */
export function parseGsa(fields: string[]): GsaData | null {
	if (fields.length < 18) {
		return null;
	}

	const modeRaw = fields[1]?.trim().toUpperCase();
	const mode: 'M' | 'A' = modeRaw === 'M' ? 'M' : 'A';
	const fixMode = parseInt(fields[2]?.trim() || '1', 10) || 1;

	const activeSatPrns: number[] = [];
	for (let i = 3; i <= 14; i++) {
		const prnStr = fields[i]?.trim();
		if (prnStr) {
			const prn = parseInt(prnStr, 10);
			if (!isNaN(prn) && prn > 0) {
				activeSatPrns.push(prn);
			}
		}
	}

	const pdopStr = fields[15]?.trim();
	const hdopStr = fields[16]?.trim();
	const vdopStr = fields[17]?.trim();

	const pdop = pdopStr ? parseFloat(pdopStr) : undefined;
	const hdop = hdopStr ? parseFloat(hdopStr) : undefined;
	const vdop = vdopStr ? parseFloat(vdopStr) : undefined;

	let gnssSystemId: number | undefined = undefined;
	if (fields.length >= 19 && fields[18]?.trim()) {
		const sysId = parseInt(fields[18].trim(), 10);
		if (!isNaN(sysId)) {
			gnssSystemId = sysId;
		}
	}

	return {
		mode,
		fixMode,
		activeSatPrns,
		pdop: isNaN(pdop ?? NaN) ? undefined : pdop,
		hdop: isNaN(hdop ?? NaN) ? undefined : hdop,
		vdop: isNaN(vdop ?? NaN) ? undefined : vdop,
		gnssSystemId
	};
}
