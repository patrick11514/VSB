export type TalkerId = 'GP' | 'GL' | 'GA' | 'BD' | 'GB' | 'GN' | 'Unknown';

export type SentenceType = 'GGA' | 'RMC' | 'GSA' | 'GSV' | 'VTG' | 'GLL' | 'TXT' | 'Unknown';

export type FixType = 'none' | '2d' | '3d' | 'dgps' | 'pps';

export interface SatelliteInfo {
	prn: number;
	elevation?: number; // 0 - 90 deg
	azimuth?: number; // 0 - 359 deg
	snr?: number; // 0 - 99 dBHz
	system: string; // 'GPS' | 'GLONASS' | 'Galileo' | 'BeiDou' | 'Unknown'
}

export interface RawParsedSentence {
	raw: string;
	talker: TalkerId;
	type: SentenceType;
	fields: string[];
	checksumValid: boolean;
}

export interface GgaData {
	utcTime: string; // hhmmss.ss
	latitude: number;
	longitude: number;
	fixQuality: number; // 0 = invalid, 1 = GPS SPS, 2 = DGPS, 3 = PPS, etc.
	satellitesUsed: number;
	hdop?: number;
	altitudeMeters?: number;
	geoidHeightMeters?: number;
}

export interface RmcData {
	utcTime: string; // hhmmss.ss
	status: 'A' | 'V'; // A = Active/valid, V = Void
	latitude: number;
	longitude: number;
	speedKnots?: number;
	speedMps?: number;
	courseDeg?: number;
	dateStr?: string; // ddmmyy
	magVarDeg?: number; // signed: positive = East, negative = West
}

export interface GsaData {
	mode: 'M' | 'A';
	fixMode: number; // 1 = no fix, 2 = 2D, 3 = 3D
	activeSatPrns: number[];
	pdop?: number;
	hdop?: number;
	vdop?: number;
	gnssSystemId?: number; // NMEA 4.10 system ID if present
}

export interface GsvData {
	totalMessages: number;
	messageNumber: number;
	totalSatellites: number;
	satellites: SatelliteInfo[];
}

export interface VtgData {
	courseTrueDeg?: number;
	courseMagDeg?: number;
	speedKnots?: number;
	speedKmh?: number;
	speedMps?: number;
}

export interface TrackPoint {
	talker: TalkerId;
	utcTime: string; // hhmmss.ss
	isoTime?: string; // YYYY-MM-DDTHH:mm:ss.sssZ
	dateStr?: string; // ddmmyy
	lat: number;
	lon: number;
	ele?: number; // elevation in meters
	geoidHeight?: number; // geoid separation in meters
	fix?: FixType;
	satCount?: number;
	hdop?: number;
	vdop?: number;
	pdop?: number;
	magVar?: number;
	speedMps?: number;
	courseDeg?: number;
	satellites: SatelliteInfo[];
}

export interface FilterSettings {
	minTimeDeltaSeconds: number;
	minBearingDeltaDegrees: number;
	minDistanceMeters: number;
	gnssFilter: 'ALL' | 'GP' | 'GL' | 'GA' | 'BD';
	onlyValidFixes: boolean;
}

export interface TrackStats {
	totalSentences: number;
	validChecksums: number;
	invalidChecksums: number;
	totalRawPoints: number;
	filteredPoints: number;
	totalDistanceMeters: number;
	durationSeconds: number;
	avgSpeedMps?: number;
	maxSpeedMps?: number;
	minElevation?: number;
	maxElevation?: number;
	talkerCounts: Record<string, number>;
	sentenceCounts: Record<string, number>;
}
