/**
 * Mean radius of Earth in meters.
 */
const EARTH_RADIUS_METERS = 6371000;

function toRadians(deg: number): number {
	return (deg * Math.PI) / 180;
}

function toDegrees(rad: number): number {
	return (rad * 180) / Math.PI;
}

/**
 * Calculates the great-circle distance between two points on Earth using Haversine formula.
 * Returns distance in meters.
 */
export function haversineDistance(
	lat1: number,
	lon1: number,
	lat2: number,
	lon2: number
): number {
	const phi1 = toRadians(lat1);
	const phi2 = toRadians(lat2);
	const deltaPhi = toRadians(lat2 - lat1);
	const deltaLambda = toRadians(lon2 - lon1);

	const a =
		Math.sin(deltaPhi / 2) * Math.sin(deltaPhi / 2) +
		Math.cos(phi1) * Math.cos(phi2) * Math.sin(deltaLambda / 2) * Math.sin(deltaLambda / 2);

	const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));

	return EARTH_RADIUS_METERS * c;
}

/**
 * Calculates the initial bearing (forward azimuth) from point 1 to point 2 in degrees (0 - 360).
 */
export function calculateBearing(
	lat1: number,
	lon1: number,
	lat2: number,
	lon2: number
): number {
	const phi1 = toRadians(lat1);
	const phi2 = toRadians(lat2);
	const deltaLambda = toRadians(lon2 - lon1);

	const y = Math.sin(deltaLambda) * Math.cos(phi2);
	const x =
		Math.cos(phi1) * Math.sin(phi2) -
		Math.sin(phi1) * Math.cos(phi2) * Math.cos(deltaLambda);

	const theta = Math.atan2(y, x);
	const deg = toDegrees(theta);
	return (deg + 360) % 360;
}

/**
 * Calculates the minimum angular difference between two bearings in degrees (0 to 180).
 * Handles the 359° to 1° wrap-around correctly.
 */
export function angularDifference(bearing1: number, bearing2: number): number {
	const diff = Math.abs(bearing1 - bearing2) % 360;
	return diff > 180 ? 360 - diff : diff;
}

/**
 * Converts speed in knots to meters per second (m/s).
 * 1 international nautical mile = 1852 meters. 1 hour = 3600 seconds.
 * 1 knot = 1852 / 3600 = 0.514444... m/s
 */
export function knotsToMps(knots: number): number {
	return knots * (1852 / 3600);
}
