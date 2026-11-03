#!/bin/bash
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
"$DIR/stop.sh" >/dev/null 2>&1 || true

docker run -d --name nginx-vuln \
  --cpus="1.0" \
  -v "$DIR/nginx.conf":/etc/nginx/nginx.conf:ro \
  -v "$DIR/certs":/etc/nginx/certs:ro \
  -p 8443:443 \
  nginx:1.25.2
echo "[+] Started nginx-vuln (vulnerable v1.25.2) on https://127.0.0.1:8443"
