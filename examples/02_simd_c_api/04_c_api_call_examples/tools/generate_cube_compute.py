#!/usr/bin/env python3

"""Validate the hand-written DAV3510 Cube compute examples.

Cube compute APIs need distinct matrix layouts and observable numerical scenes, so
they cannot be produced safely by the generic prototype-to-marker generator used
for the initial scaffold.
"""

import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "cube_compute"
EXPECTED = {
    "asc_enable_fp8",
    "asc_enable_hf32",
    "asc_enable_hf32_trans",
    "asc_enable_hif8",
    "asc_mmad",
    "asc_mmad_mx",
    "asc_set_fp32_mode",
    "asc_set_hf32_round_mode",
    "asc_set_l0c2gm_config",
    "asc_set_l0c2gm_nz2nd",
    "asc_set_mmad_direction_m",
    "asc_set_mmad_direction_n",
}


def validate(doc_dir: Path) -> None:
    missing_docs = sorted(name for name in EXPECTED if not (doc_dir / f"{name}.md").is_file())
    if missing_docs:
        raise SystemExit(f"Missing C API documents: {', '.join(missing_docs)}")

    actual = {path.stem for path in OUT.glob("*.asc")}
    if actual != EXPECTED:
        missing = sorted(EXPECTED - actual)
        extra = sorted(actual - EXPECTED)
        raise SystemExit(f"Cube compute example mismatch; missing={missing}, extra={extra}")

    for name in sorted(EXPECTED):
        source = (OUT / f"{name}.asc").read_text(encoding="utf-8")
        first_line = source.splitlines()[0]
        if "bisheng" not in first_line or "--npu-arch=dav-3510" not in first_line:
            raise SystemExit(f"{name}: first line must contain the direct DAV3510 build/run command")
        if f"{name}(" not in source:
            raise SystemExit(f"{name}: target API call not found")
        if 'common/sample_host.h' in source:
            raise SystemExit(f"{name}: shared host helper is not allowed")
        if "aclrtSynchronizeDevice" not in source:
            raise SystemExit(f"{name}: runnable Host launch is missing")

    print(f"Validated {len(EXPECTED)} functional Cube compute examples in {OUT}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit(f"Usage: {sys.argv[0]} <C-API/cube_compute directory>")
    validate(Path(sys.argv[1]).resolve())
