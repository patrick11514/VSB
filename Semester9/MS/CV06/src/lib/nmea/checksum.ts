/**
 * Computes the NMEA 0183 8-bit XOR checksum for a string.
 * The string should not include the leading '$' or trailing '*'.
 */
export function computeNmeaChecksum(payload: string): string {
	let checksum = 0;
	for (let i = 0; i < payload.length; i++) {
		checksum ^= payload.charCodeAt(i);
	}
	return checksum.toString(16).toUpperCase().padStart(2, '0');
}

/**
 * Validates the checksum of a raw NMEA 0183 sentence.
 * Returns true if valid or false if checksum fails or format is invalid.
 */
export function validateNmeaChecksum(sentence: string): boolean {
	const trimmed = sentence.trim();
	if (!trimmed.startsWith('$') && !trimmed.startsWith('!')) {
		return false;
	}

	const asteriskIndex = trimmed.indexOf('*');
	if (asteriskIndex === -1) {
		return false;
	}

	const payload = trimmed.substring(1, asteriskIndex);
	const expectedChecksum = trimmed.substring(asteriskIndex + 1, asteriskIndex + 3).toUpperCase();

	if (expectedChecksum.length < 2) {
		return false;
	}

	const actualChecksum = computeNmeaChecksum(payload);
	return actualChecksum === expectedChecksum;
}
