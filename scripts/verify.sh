#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

cleanup() {
  rm -rf .pio tests/__pycache__ scripts/__pycache__
}
trap cleanup EXIT
cleanup

python3 scripts/secret_scan.py
python3 scripts/check_repo.py
python3 -m unittest discover -s tests -p 'test_*.py' -v
pio run

echo "Publication, source-contract and ESP32 build gates: PASS"
