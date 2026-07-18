#!/usr/bin/env python3
"""Hardware-independent publication contracts; no sensor or network emulation."""
from __future__ import annotations

from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class SourceContractsTest(unittest.TestCase):
    def read(self, path: str) -> str:
        return (ROOT / path).read_text(encoding="utf-8")

    def test_no_default_wifi_credentials_or_hotspot(self) -> None:
        main = self.read("src/main.cpp")
        example = self.read("src/wifi_credentials.example.h")
        self.assertIn('__has_include("wifi_credentials.h")', main)
        self.assertIn('#define BABY_MONITOR_WIFI_SSID ""', main)
        self.assertIn("replace_with_2_4ghz_wifi_ssid", example)
        self.assertNotIn("WiFi.softAP", main + self.read("src/wifi_server.cpp"))

    def test_http_routes_are_local_demonstration_routes(self) -> None:
        server = self.read("src/wifi_server.cpp")
        self.assertIn('"/api/status"', server)
        self.assertIn('"/api/mute"', server)
        self.assertIn('"scope"] = "demonstration_only"', server)
        self.assertIn("local HTTP is disabled", server)
        self.assertNotIn('"status"] = "ok"', server)

    def test_invalid_readings_are_not_reported_as_reference(self) -> None:
        posture = self.read("src/posture.cpp")
        environment = self.read("src/environment.cpp")
        self.assertIn("DISTANCE_UNKNOWN", posture)
        self.assertIn("ENV_UNKNOWN", environment)
        self.assertIn("isnan(_temperature)", environment)

    def test_neutral_signal_vocabulary_replaces_safety_claims_in_firmware(self) -> None:
        firmware = "\n".join(self.read(path) for path in (
            "src/main.cpp", "src/posture.cpp", "src/environment.cpp", "src/audio_feedback.cpp", "src/wifi_server.cpp",
        ))
        for disallowed in ("POSTURE_DANGER", "哭声", "趴睡", "安全状态", "Baby Monitor Status"):
            self.assertNotIn(disallowed, firmware)
        for expected in ("demonstration", "SIGNAL_HIGH_THRESHOLD", "DISTANCE_NEAR_THRESHOLD"):
            self.assertIn(expected, firmware)

    def test_publication_docs_keep_strict_non_safety_boundary(self) -> None:
        readme = self.read("README.md")
        security = self.read("SECURITY.md")
        verification = self.read("docs/VERIFICATION.md")
        self.assertIn("不得将它用于婴儿看护", readme)
        self.assertIn("严格不适用场景", security)
        self.assertIn("不是婴儿安全监护", readme)


if __name__ == "__main__":
    unittest.main()
