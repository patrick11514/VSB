import { parseGga } from './parsers/gga';
import { parseRmc } from './parsers/rmc';
import { parseGsa } from './parsers/gsa';
import { parseGsv } from './parsers/gsv';
import { parseVtg } from './parsers/vtg';
import { parseRawSentence } from './sentenceParser';
import { parseNmeaDateTime } from './coordinates';
import type {
	FixType,
	RawParsedSentence,
	SatelliteInfo,
	TrackPoint,
	TrackStats,
	TalkerId
} from './types';

interface EpochBuilder {
	talker: TalkerId;
	utcTime: string;
	lat?: number;
	lon?: number;
	ele?: number;
	geoidHeight?: number;
	fixQuality?: number;
	fixMode?: number;
	satCount?: number;
	hdop?: number;
	vdop?: number;
	pdop?: number;
	magVar?: number;
	speedMps?: number;
	courseDeg?: number;
	satellites: Map<string, SatelliteInfo>; // Key by "system:prn" to prevent duplicates
}

function createEmptyEpoch(talker: TalkerId = 'GP', utcTime: string = ''): EpochBuilder {
	return {
		talker,
		utcTime,
		satellites: new Map()
	};
}

function resolveFixType(fixQuality?: number, fixMode?: number): FixType {
	if (fixQuality === 2) return 'dgps';
	if (fixQuality === 3) return 'pps';
	if (fixMode === 2) return '2d';
	if (fixMode === 3) return '3d';
	if (fixQuality && fixQuality > 0) return '3d';
	return 'none';
}

function finalizeEpoch(
	builder: EpochBuilder,
	currentDateStr?: string
): TrackPoint | null {
	if (builder.lat === undefined || builder.lon === undefined) {
		return null;
	}

	const fix = resolveFixType(builder.fixQuality, builder.fixMode);
	const isoTime = parseNmeaDateTime(currentDateStr, builder.utcTime);

	return {
		talker: builder.talker,
		utcTime: builder.utcTime,
		isoTime,
		dateStr: currentDateStr,
		lat: builder.lat,
		lon: builder.lon,
		ele: builder.ele,
		geoidHeight: builder.geoidHeight,
		fix,
		satCount: builder.satCount,
		hdop: builder.hdop,
		vdop: builder.vdop,
		pdop: builder.pdop,
		magVar: builder.magVar,
		speedMps: builder.speedMps,
		courseDeg: builder.courseDeg,
		satellites: Array.from(builder.satellites.values())
	};
}

export interface ParseResult {
	points: TrackPoint[];
	stats: TrackStats;
	sentences: RawParsedSentence[];
}

/**
 * Ingests a raw NMEA file content string, processes line by line,
 * aggregates sentences into synchronized epochs/track points,
 * and collects comprehensive statistics.
 */
export function parseNmeaText(rawContent: string): ParseResult {
	const lines = rawContent.split(/\r?\n/);

	const stats: TrackStats = {
		totalSentences: 0,
		validChecksums: 0,
		invalidChecksums: 0,
		totalRawPoints: 0,
		filteredPoints: 0,
		totalDistanceMeters: 0,
		durationSeconds: 0,
		talkerCounts: {},
		sentenceCounts: {}
	};

	const points: TrackPoint[] = [];
	const parsedSentences: RawParsedSentence[] = [];

	let currentEpoch: EpochBuilder | null = null;
	let lastKnownDateStr: string | undefined = undefined;

	function flushCurrentEpoch() {
		if (currentEpoch) {
			const pt = finalizeEpoch(currentEpoch, lastKnownDateStr);
			if (pt) {
				points.push(pt);
			}
			currentEpoch = null;
		}
	}

	for (const line of lines) {
		const rawParsed = parseRawSentence(line);
		if (!rawParsed) continue;

		stats.totalSentences++;
		stats.talkerCounts[rawParsed.talker] = (stats.talkerCounts[rawParsed.talker] || 0) + 1;
		stats.sentenceCounts[rawParsed.type] = (stats.sentenceCounts[rawParsed.type] || 0) + 1;

		if (rawParsed.checksumValid) {
			stats.validChecksums++;
		} else {
			stats.invalidChecksums++;
		}

		parsedSentences.push(rawParsed);

		// If this sentence carries UTC time and it differs from current active epoch, flush previous epoch
		let sentenceTime: string | null = null;
		if (rawParsed.type === 'GGA' || rawParsed.type === 'RMC') {
			sentenceTime = rawParsed.fields[1]?.trim() || null;
		}

		if (sentenceTime && currentEpoch && currentEpoch.utcTime && sentenceTime !== currentEpoch.utcTime) {
			flushCurrentEpoch();
		}

		if (!currentEpoch) {
			currentEpoch = createEmptyEpoch(rawParsed.talker, sentenceTime || '');
		} else if (sentenceTime && !currentEpoch.utcTime) {
			currentEpoch.utcTime = sentenceTime;
		}

		// Dispatch to specific sentence parsers
		switch (rawParsed.type) {
			case 'GGA': {
				const gga = parseGga(rawParsed.fields);
				if (gga) {
					currentEpoch.lat = gga.latitude;
					currentEpoch.lon = gga.longitude;
					currentEpoch.fixQuality = gga.fixQuality;
					currentEpoch.satCount = gga.satellitesUsed;
					if (gga.altitudeMeters !== undefined) currentEpoch.ele = gga.altitudeMeters;
					if (gga.geoidHeightMeters !== undefined) currentEpoch.geoidHeight = gga.geoidHeightMeters;
					if (gga.hdop !== undefined) currentEpoch.hdop = gga.hdop;
					if (rawParsed.talker !== 'Unknown') currentEpoch.talker = rawParsed.talker;
				}
				break;
			}

			case 'RMC': {
				const rmc = parseRmc(rawParsed.fields);
				if (rmc) {
					if (rmc.status === 'A') {
						if (currentEpoch.lat === undefined) currentEpoch.lat = rmc.latitude;
						if (currentEpoch.lon === undefined) currentEpoch.lon = rmc.longitude;
					}
					if (rmc.speedMps !== undefined) currentEpoch.speedMps = rmc.speedMps;
					if (rmc.courseDeg !== undefined) currentEpoch.courseDeg = rmc.courseDeg;
					if (rmc.magVarDeg !== undefined) currentEpoch.magVar = rmc.magVarDeg;
					if (rmc.dateStr) {
						lastKnownDateStr = rmc.dateStr;
					}
					if (rawParsed.talker !== 'Unknown' && currentEpoch.talker === 'Unknown') {
						currentEpoch.talker = rawParsed.talker;
					}
				}
				break;
			}

			case 'GSA': {
				const gsa = parseGsa(rawParsed.fields);
				if (gsa) {
					currentEpoch.fixMode = gsa.fixMode;
					if (gsa.hdop !== undefined) currentEpoch.hdop = gsa.hdop;
					if (gsa.vdop !== undefined) currentEpoch.vdop = gsa.vdop;
					if (gsa.pdop !== undefined) currentEpoch.pdop = gsa.pdop;
				}
				break;
			}

			case 'GSV': {
				const gsv = parseGsv(rawParsed.fields, rawParsed.talker);
				if (gsv) {
					for (const sat of gsv.satellites) {
						const key = `${sat.system}:${sat.prn}`;
						currentEpoch.satellites.set(key, sat);
					}
				}
				break;
			}

			case 'VTG': {
				const vtg = parseVtg(rawParsed.fields);
				if (vtg) {
					if (vtg.speedMps !== undefined && currentEpoch.speedMps === undefined) {
						currentEpoch.speedMps = vtg.speedMps;
					}
					if (vtg.courseTrueDeg !== undefined && currentEpoch.courseDeg === undefined) {
						currentEpoch.courseDeg = vtg.courseTrueDeg;
					}
				}
				break;
			}
		}
	}

	// Flush trailing epoch
	flushCurrentEpoch();

	stats.totalRawPoints = points.length;
	return { points, stats, sentences: parsedSentences };
}
