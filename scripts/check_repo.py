#!/usr/bin/env python3
"""Check reproducible structure and the project's safety wording contract."""
from __future__ import annotations

from pathlib import Path
import configparser
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
REQUIRED = (
    "README.md", "LICENSE", "SECURITY.md", "THIRD_PARTY_NOTICES.md", "HARDWARE.md",
    "platformio.ini", "src/main.cpp", "src/config.h", "src/wifi_credentials.example.h",
    "src/wifi_server.cpp", "src/posture.cpp", "lib/HCSR04/hcsr04.cpp", "lib/SoundDetector/sound.cpp",
    "hardware/BOM.csv", "hardware/wiring-diagram.svg", "docs/SOURCE_PROVENANCE.md",
    "docs/PROJECT_STATUS.md", "docs/VERIFICATION.md", "docs/PROTOCOL.md", "docs/GITHUB_METADATA.md",
    "docs/HARDWARE_LAB_CARD.md", "scripts/verify.sh", "scripts/secret_scan.py",
    "tests/test_source_contracts.py", ".github/workflows/validate.yml",
)


def fail(message: str) -> None:
    raise SystemExit(f"FAIL: {message}")


def main() -> None:
    missing = [path for path in REQUIRED if not (ROOT / path).is_file()]
    if missing:
        fail("missing required files: " + ", ".join(missing))

    config = configparser.ConfigParser()
    config.read(ROOT / "platformio.ini")
    env = config["env:esp32dev"]
    if env.get("platform") != "espressif32@6.13.0":
        fail("platformio platform must be fixed to espressif32@6.13.0")
    for dependency in ("ArduinoJson@6.21.6", "DHT sensor library@1.4.7", "Adafruit Unified Sensor@1.1.15"):
        if dependency not in env.get("lib_deps", ""):
            fail(f"fixed dependency missing: {dependency}")

    root = ET.parse(ROOT / "hardware/wiring-diagram.svg").getroot()
    if root.tag != "{http://www.w3.org/2000/svg}svg" or not root.get("viewBox"):
        fail("wiring diagram must be a valid self-contained SVG with viewBox")

    readme = (ROOT / "README.md").read_text(encoding="utf-8")
    for fact in ("不是婴儿安全监护", "5 V", "/api/status", "可信局域网"):
        if fact not in readme:
            fail(f"README is missing required boundary: {fact}")
    if "Android APP 远程查看状态" in readme:
        fail("README must not claim missing Android app is reproducible")

    example = (ROOT / "src/wifi_credentials.example.h").read_text(encoding="utf-8")
    if "replace_with_" not in example or "wifi_credentials.h" not in (ROOT / ".gitignore").read_text(encoding="utf-8"):
        fail("credential example/ignore contract is missing")

    print("Repository structure check: PASS")


if __name__ == "__main__":
    main()
