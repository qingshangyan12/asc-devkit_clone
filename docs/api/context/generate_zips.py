"""Generate filtered download zips after Sphinx build.

Produces:
  _build/html/download_all.zip   — all source .md/.rst files
  _build/html/download_950.zip   — only 950-supported .md/.rst files
"""

import json
import urllib.parse
import zipfile
from pathlib import Path


def load_950_supported_sources(json_path: Path, src_dir: Path) -> set[str]:
    """Load version_filter.json, return the set of source filenames (.md/.rst) supported by 950."""
    with open(json_path, "r", encoding="utf-8") as f:
        data = json.load(f)

    supported: set[str] = set()
    skipped = 0
    for key, is_supported in data.items():
        if is_supported:
            html_name = urllib.parse.unquote(key)
            stem = html_name.rsplit(".", 1)[0]
            found = None
            for ext in (".md", ".rst"):
                candidate = stem + ext
                if (src_dir / candidate).exists():
                    found = candidate
                    break
            if found:
                supported.add(found)
            else:
                skipped += 1
    if skipped:
        print(f"[zip] skipped {skipped} JSON keys (no matching .md/.rst source)")
    return supported


def build_src_zip(
    src_dir: Path,
    out_path: Path,
    include_files: set[str] | None,
) -> None:
    """Create a zip archive of source .md/.rst files.

    Args:
        src_dir: Source directory containing .md/.rst files.
        out_path: Destination zip file path.
        include_files: Set of source filenames to include, or None for all.
    """
    out_path.parent.mkdir(parents=True, exist_ok=True)

    with zipfile.ZipFile(out_path, "w", zipfile.ZIP_DEFLATED) as zf:
        for src_file in sorted(src_dir.glob("*")):
            if src_file.suffix not in (".md", ".rst"):
                continue
            if include_files is not None and src_file.name not in include_files:
                continue
            zf.write(src_file, src_file.name)


def main():
    context_dir = Path(__file__).resolve().parent
    html_dir = context_dir / "_build" / "html"
    json_path = context_dir / "_static" / "version_filter.json"

    if not html_dir.is_dir():
        print(f"[zip] Build directory not found: {html_dir}")
        return

    # ── 全量 zip ──
    all_zip = html_dir / "download_all.zip"
    print("[zip] Building download_all.zip (all .md/.rst files) ...")
    build_src_zip(context_dir, all_zip, include_files=None)
    all_size_mb = all_zip.stat().st_size / 1024 / 1024 if all_zip.exists() else 0
    print(f"[zip] download_all.zip  ({all_size_mb:.1f} MB)")

    # ── 950 zip ──
    supported = load_950_supported_sources(json_path, context_dir)
    print(f"[zip] 950-supported source files: {len(supported)}")

    zip_950 = html_dir / "download_950.zip"
    print("[zip] Building download_950.zip (950 .md/.rst only) ...")
    build_src_zip(context_dir, zip_950, include_files=supported)
    size_mb = zip_950.stat().st_size / 1024 / 1024 if zip_950.exists() else 0
    print(f"[zip] download_950.zip  ({size_mb:.1f} MB)")

    print("[zip] Done.")


if __name__ == "__main__":
    main()
