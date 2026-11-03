use http::{Method, Request};
use std::sync::Arc;
use std::time::{Duration, Instant};
use tokio::net::TcpStream;
use tokio::time::timeout;
use tokio_rustls::rustls;
use tokio_rustls::rustls::pki_types::ServerName;
use tokio_rustls::TlsConnector;

const HOST: &str = "127.0.0.1";
const PORT: u16 = 8443;
const PING_INTERVAL_MS: u64 = 300;
const TIMEOUT_MS: u64 = 2000;

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

fn create_connector() -> TlsConnector {
    let root_store = rustls::RootCertStore::empty();
    let mut config = rustls::ClientConfig::builder()
        .with_root_certificates(root_store)
        .with_no_client_auth();
    config.alpn_protocols = vec![b"h2".to_vec()];
    config
        .dangerous()
        .set_certificate_verifier(Arc::new(NoVerifier));
    TlsConnector::from(Arc::new(config))
}

fn render_bar(latency_ms: f64) -> String {
    let total_slots = 20;
    // Scale: 0 to 500ms
    let filled = ((latency_ms / 500.0) * total_slots as f64).round() as usize;
    let filled = filled.clamp(1, total_slots);
    let empty = total_slots - filled;

    let bar_chars = "█".repeat(filled) + &"░".repeat(empty);

    if latency_ms < 25.0 {
        format!("\x1b[32m{}\x1b[0m", bar_chars) // Green
    } else if latency_ms < 150.0 {
        format!("\x1b[33m{}\x1b[0m", bar_chars) // Yellow
    } else {
        format!("\x1b[31m{}\x1b[0m", bar_chars) // Red
    }
}

async fn single_request(
    connector: &TlsConnector,
    domain: &ServerName<'static>,
    addr: &str,
) -> Result<(u16, Duration), String> {
    let start = Instant::now();

    // 1. TCP Connect
    let tcp_stream = TcpStream::connect(addr)
        .await
        .map_err(|e| format!("TCP Error: {}", e))?;
    let _ = tcp_stream.set_nodelay(true);

    // 2. TLS Handshake
    let tls_stream = connector
        .connect(domain.clone(), tcp_stream)
        .await
        .map_err(|e| format!("TLS Error: {}", e))?;

    // 3. HTTP/2 Handshake
    let (client, h2_driver) = h2::client::handshake(tls_stream)
        .await
        .map_err(|e| format!("H2 Handshake Error: {}", e))?;

    tokio::spawn(async move {
        let _ = h2_driver.await;
    });

    let request = Request::builder()
        .method(Method::GET)
        .uri(format!("https://{}:{}/", HOST, PORT))
        .body(())
        .unwrap();

    let mut ready_client = client
        .ready()
        .await
        .map_err(|e| format!("H2 Ready Error: {}", e))?;

    let (response_future, _) = ready_client
        .send_request(request, true)
        .map_err(|e| format!("H2 Send Error: {}", e))?;

    let response = response_future
        .await
        .map_err(|e| format!("H2 Response Error: {}", e))?;

    let elapsed = start.elapsed();
    Ok((response.status().as_u16(), elapsed))
}

#[tokio::main]
async fn main() {
    println!("\x1b[1;35m====================================================================\x1b[0m");
    println!("\x1b[1;35m   Legitimate User Latency & Availability Monitor (HTTP/2)          \x1b[0m");
    println!("\x1b[1;35m====================================================================\x1b[0m");
    println!("Target:   https://{}:{}", HOST, PORT);
    println!("Interval: {} ms | Timeout: {} ms\n", PING_INTERVAL_MS, TIMEOUT_MS);

    let connector = create_connector();
    let domain = ServerName::try_from(HOST.to_string()).unwrap().to_owned();
    let addr = format!("{}:{}", HOST, PORT);

    let mut req_num = 0u64;

    loop {
        req_num += 1;
        let result = timeout(
            Duration::from_millis(TIMEOUT_MS),
            single_request(&connector, &domain, &addr),
        )
        .await;

        match result {
            Ok(Ok((status, duration))) => {
                let ms = duration.as_secs_f64() * 1000.0;
                let bar = render_bar(ms);
                let tag = if ms < 25.0 {
                    "\x1b[1;32m[OK - FAST]\x1b[0m"
                } else if ms < 150.0 {
                    "\x1b[1;33m[DEGRADED ]\x1b[0m"
                } else {
                    "\x1b[1;31m[CRITICAL ]\x1b[0m"
                };

                println!(
                    "#{:<4} {} Status: \x1b[1m{}\x1b[0m | Latency: {:6.2} ms | {}",
                    req_num, tag, status, ms, bar
                );
            }
            Ok(Err(err)) => {
                println!(
                    "#{:<4} \x1b[1;31m[FAILED   ]\x1b[0m Error: {}",
                    req_num, err
                );
            }
            Err(_) => {
                println!(
                    "#{:<4} \x1b[1;41;37m[TIMEOUT  ]\x1b[0m Request exceeded {} ms! Server Unresponsive!",
                    req_num, TIMEOUT_MS
                );
            }
        }

        tokio::time::sleep(Duration::from_millis(PING_INTERVAL_MS)).await;
    }
}
