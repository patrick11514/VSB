#!/usr/bin/env bash
docker rm -f apache-vuln nginx-vuln apache-fixed nginx-fixed 2>/dev/null || true
