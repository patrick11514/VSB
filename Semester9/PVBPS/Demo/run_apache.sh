#!/usr/bin/env bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
"$DIR/stop.sh" >/dev/null 2>&1 || true

docker run -d --name apache-vuln \
  --cpus="1.0" \
  -v "$DIR/httpd.conf":/usr/local/apache2/conf/httpd.conf:ro \
  -v "$DIR/modules.conf":/usr/local/apache2/conf/modules.conf:ro \
  -v "$DIR/certs":/usr/local/apache2/certs:ro \
  -p 8443:443 \
  httpd:2.4.57
echo "[+] Started apache-vuln (vulnerable v2.4.57) on https://127.0.0.1:8443"
