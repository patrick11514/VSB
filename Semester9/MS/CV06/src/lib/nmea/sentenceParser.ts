import { validateNmeaChecksum } from './checksum';
import type { RawParsedSentence, SentenceType, TalkerId } from './types';

export function extractTalkerAndType(header: string): { talker: TalkerId; type: SentenceType } {
	// Header starts with $ or !, e.g., $GPGGA, $GNRMC, $GLGSV, $BDGGA
	const cleaned = header.replace(/^[$!]/, '').toUpperCase();
	if (cleaned.length < 5) {
		return { talker: 'Unknown', type: 'Unknown' };
	}

	const talkerCode = cleaned.substring(0, 2);
	const typeCode = cleaned.substring(2);

	let talker: TalkerId = 'Unknown';
	if (talkerCode === 'GP') talker = 'GP';
	else if (talkerCode === 'GL') talker = 'GL';
	else if (talkerCode === 'GA') talker = 'GA';
	else if (talkerCode === 'BD' || talkerCode === 'GB') talker = 'BD';
	else if (talkerCode === 'GN') talker = 'GN';

	let type: SentenceType = 'Unknown';
	if (typeCode === 'GGA') type = 'GGA';
	else if (typeCode === 'RMC') type = 'RMC';
	else if (typeCode === 'GSA') type = 'GSA';
	else if (typeCode === 'GSV') type = 'GSV';
	else if (typeCode === 'VTG') type = 'VTG';
	else if (typeCode === 'GLL') type = 'GLL';
	else if (typeCode === 'TXT') type = 'TXT';

	return { talker, type };
}

/**
 * Parses a raw line of text into a RawParsedSentence.
 */
export function parseRawSentence(line: string): RawParsedSentence | null {
	const trimmed = line.trim();
	if (!trimmed || (!trimmed.startsWith('$') && !trimmed.startsWith('!'))) {
		return null;
	}

	const checksumValid = validateNmeaChecksum(trimmed);

	// Strip checksum part after '*' if present
	const asteriskIdx = trimmed.indexOf('*');
	const mainBody = asteriskIdx !== -1 ? trimmed.substring(0, asteriskIdx) : trimmed;

	const fields = mainBody.split(',');
	const header = fields[0] || '';
	const { talker, type } = extractTalkerAndType(header);

	return {
		raw: trimmed,
		talker,
		type,
		fields,
		checksumValid
	};
}
