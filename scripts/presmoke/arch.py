from __future__ import annotations

import os
import re
import subprocess
from typing import Optional


def detect_arch(explicit: Optional[str] = None) -> Optional[str]:
    if explicit:
        return explicit
    env_arch = os.environ.get("PRESMOKE_ARCH")
    if env_arch:
        return env_arch
    try:
        proc = subprocess.run(["npu-smi", "info"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, timeout=5)
    except (OSError, subprocess.TimeoutExpired):
        return None
    text = proc.stdout.lower()
    if "910b" in text or "910" in text or "a2" in text:
        return "dav-2201"
    if "950" in text or "a3" in text:
        return "dav-3510"
    found = re.search(r"dav-\d+", text)
    if found:
        return found.group(0)
    return None
