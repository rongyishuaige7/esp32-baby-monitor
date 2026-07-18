#!/usr/bin/env python3
"""Reject credentials, generated files, binary payloads, and unsafe claims."""
from __future__ import annotations

from pathlib import Path
import re

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
FORBIDDEN_PARTS = {".pio", ".idea", ".vscode", ".vs", "__pycache__"}
FORBIDDEN_NAMES = {"wifi_credentials.h", ".env", ".env.local"}
FORBIDDEN_SUFFIXES = {".bin", ".elf", ".map", ".zip", ".tar", ".gz", ".apk", ".aab", ".mp4", ".mov", ".avi", ".mkv", ".pcap"}
SUSPICIOUS = (
    re.compile(r'''(?i)\b(?:wifi_)?(?:ssid|password|passwd|token|api[_-]?key|secret)\s*[:=]\s*["'](?!replace_with_)[^"']{4,}["']'''),
    re.compile(r'''(?i)\b(?:sk|api)[_-]?(?:key|token)\b\s*[:=]\s*["'][A-Za-z0-9_\-]{12,}["']'''),
)
FORBIDDEN_MARKETING = (
    "婴儿安全监护系统",
    "睡眠安全系统",
    "防猝死",
    "医疗设备",
    "哭声报警",
    "趴睡判定",
)
PUBLIC_MEDIA_DIRS = {"assets", "hardware"}
PUBLIC_IMAGE_SUFFIXES = {".jpg", ".jpeg", ".png"}
PUBLIC_PDF_SUFFIXES = {".pdf"}


def fail(message: str) -> None:
    raise SystemExit(f"FAIL: {message}")


def main() -> None:
    checked = 0
    for path in ROOT.rglob("*"):
        if not path.is_file() or ".git" in path.parts:
            continue
        rel = path.relative_to(ROOT).as_posix()
        if any(part in FORBIDDEN_PARTS for part in path.relative_to(ROOT).parts):
            fail(f"generated/editor path is tracked: {rel}")
        if path.name in FORBIDDEN_NAMES or path.suffix.lower() in FORBIDDEN_SUFFIXES:
            fail(f"forbidden local or binary file is tracked: {rel}")
        if path.stat().st_size > 2 * 1024 * 1024:
            fail(f"file exceeds 2 MiB publication limit: {rel}")
        suffix = path.suffix.lower()
        is_reviewed_media = rel.startswith("assets/") or rel.startswith("hardware/eda/")
        if is_reviewed_media and suffix in PUBLIC_IMAGE_SUFFIXES:
            try:
                with Image.open(path) as image:
                    image.verify()
                with Image.open(path) as image:
                    if len(image.getexif()) != 0:
                        fail(f"public image contains EXIF metadata: {rel}")
                    allowed_info = {"jfif", "jfif_version", "jfif_unit", "jfif_density", "progressive", "progression", "transparency"}
                    if set(image.info) - allowed_info:
                        fail(f"public image contains unexpected metadata: {rel}")
            except OSError as exc:
                fail(f"invalid public image {rel}: {exc}")
            checked += 1
            continue
        if is_reviewed_media and suffix in PUBLIC_PDF_SUFFIXES:
            if not path.read_bytes().startswith(b"%PDF-"):
                fail(f"invalid public PDF: {rel}")
            checked += 1
            continue
        try:
            text = path.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            fail(f"unexpected binary file is tracked: {rel}")
        for pattern in SUSPICIOUS:
            if pattern.search(text):
                fail(f"possible credential in {rel}")
        if rel not in {"README.md", "SECURITY.md", "HARDWARE.md", "docs/SOURCE_PROVENANCE.md", "docs/PROJECT_STATUS.md", "docs/VERIFICATION.md", "docs/PROTOCOL.md", "docs/HARDWARE_LAB_CARD.md", "scripts/secret_scan.py", "tests/test_source_contracts.py"}:
            for claim in FORBIDDEN_MARKETING:
                if claim in text:
                    fail(f"unsafe unsupported claim in {rel}: {claim}")
        checked += 1
    print(f"Secret/publication scan: PASS ({checked} text files checked)")


if __name__ == "__main__":
    main()
