import type { TrackPoint } from './types';

export interface GpxExportConfig {
	trackName?: string;
	includeExtensions?: boolean;
	includeSatellites?: boolean;
	creator?: string;
}

function escapeXml(unsafe: string): string {
	return unsafe
		.replace(/&/g, '&amp;')
		.replace(/</g, '&lt;')
		.replace(/>/g, '&gt;')
		.replace(/"/g, '&quot;')
		.replace(/'/g, '&apos;');
}

/**
 * Generates standard-compliant GPX 1.1 XML string from TrackPoint array.
 */
export function generateGpx(
	points: TrackPoint[],
	config: GpxExportConfig = {}
): string {
	const trackName = escapeXml(config.trackName || 'Exported NMEA Track');
	const creator = escapeXml(config.creator || 'VSB Mobile Systems NMEA Converter');
	const includeExtensions = config.includeExtensions ?? true;
	const includeSatellites = config.includeSatellites ?? true;

	const nowIso = new Date().toISOString();
	const firstPointTime = points[0]?.isoTime || nowIso;

	const lines: string[] = [];

	lines.push('<?xml version="1.0" encoding="UTF-8"?>');
	lines.push(
		'<gpx version="1.1" creator="' +
			creator +
			'"\n' +
			'     xmlns="http://www.topografix.com/GPX/1/1"\n' +
			'     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"\n' +
			'     xmlns:nmea="http://moravec.cs.vsb.cz/nmea"\n' +
			'     xsi:schemaLocation="http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd">'
	);

	// Metadata
	lines.push('  <metadata>');
	lines.push(`    <name>${trackName}</name>`);
	lines.push(`    <time>${firstPointTime}</time>`);
	lines.push('  </metadata>');

	// Track
	lines.push('  <trk>');
	lines.push(`    <name>${trackName}</name>`);
	lines.push('    <trkseg>');

	for (const pt of points) {
		const latStr = pt.lat.toFixed(7);
		const lonStr = pt.lon.toFixed(7);

		lines.push(`      <trkpt lat="${latStr}" lon="${lonStr}">`);

		// GPX 1.1 strictly ordered elements:
		// 1. ele
		if (pt.ele !== undefined) {
			lines.push(`        <ele>${pt.ele.toFixed(2)}</ele>`);
		}

		// 2. time
		if (pt.isoTime) {
			lines.push(`        <time>${pt.isoTime}</time>`);
		}

		// 3. magvar (0 <= magvar < 360 degrees)
		if (pt.magVar !== undefined) {
			const normMagVar = (pt.magVar + 360) % 360;
			lines.push(`        <magvar>${normMagVar.toFixed(2)}</magvar>`);
		}

		// 4. geoidheight
		if (pt.geoidHeight !== undefined) {
			lines.push(`        <geoidheight>${pt.geoidHeight.toFixed(2)}</geoidheight>`);
		}

		// 12. fix (none, 2d, 3d, dgps, pps)
		if (pt.fix) {
			lines.push(`        <fix>${pt.fix}</fix>`);
		}

		// 13. sat
		if (pt.satCount !== undefined && pt.satCount > 0) {
			lines.push(`        <sat>${pt.satCount}</sat>`);
		}

		// 14. hdop
		if (pt.hdop !== undefined) {
			lines.push(`        <hdop>${pt.hdop.toFixed(2)}</hdop>`);
		}

		// 15. vdop
		if (pt.vdop !== undefined) {
			lines.push(`        <vdop>${pt.vdop.toFixed(2)}</vdop>`);
		}

		// 16. pdop
		if (pt.pdop !== undefined) {
			lines.push(`        <pdop>${pt.pdop.toFixed(2)}</pdop>`);
		}

		// 19. extensions
		if (includeExtensions) {
			const hasSpeed = pt.speedMps !== undefined;
			const hasCourse = pt.courseDeg !== undefined;
			const hasSats = includeSatellites && pt.satellites && pt.satellites.length > 0;

			if (hasSpeed || hasCourse || hasSats) {
				lines.push('        <extensions>');
				if (hasSpeed && pt.speedMps !== undefined) {
					lines.push(`          <nmea:speed>${pt.speedMps.toFixed(3)}</nmea:speed>`);
				}
				if (hasCourse && pt.courseDeg !== undefined) {
					lines.push(`          <nmea:course>${pt.courseDeg.toFixed(1)}</nmea:course>`);
				}
				if (hasSats && pt.satellites.length > 0) {
					lines.push('          <nmea:satellites>');
					for (const sat of pt.satellites) {
						let satTag = `            <nmea:sat prn="${sat.prn}" system="${escapeXml(sat.system)}"`;
						if (sat.elevation !== undefined) {
							satTag += ` elevation="${sat.elevation}"`;
						}
						if (sat.azimuth !== undefined) {
							satTag += ` azimuth="${sat.azimuth}"`;
						}
						if (sat.snr !== undefined) {
							satTag += ` snr="${sat.snr}"`;
						}
						satTag += ' />';
						lines.push(satTag);
					}
					lines.push('          </nmea:satellites>');
				}
				lines.push('        </extensions>');
			}
		}

		lines.push('      </trkpt>');
	}

	lines.push('    </trkseg>');
	lines.push('  </trk>');
	lines.push('</gpx>');

	return lines.join('\n');
}
