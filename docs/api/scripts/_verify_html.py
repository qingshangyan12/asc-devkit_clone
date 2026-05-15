"""Post-build HTML validation: check for common rendering issues after Sphinx build.

Checks:
  1. Residual .md links (verify generate_zips.py post-processing)
  2. Broken image references (<img src="..."> pointing to missing files)
  3. Empty <section> elements (may indicate MyST parsing failures)
  4. Backslash escape chars in visible text
"""

import re
import sys
from pathlib import Path

SRC_ROOT = Path(__file__).resolve().parent.parent

IMG_RE = re.compile(r'<img[^>]+src="([^"]+)"')
RESIDUAL_MD_RE = re.compile(r'href="[^"]+\.md(?:#[^"]*)?"')
SECTION_RE = re.compile(r"<section[^>]*>\s*</section>")
BAD_ESCAPE_RE = re.compile(r">[^<]*\\[_()[\]{}][^<]*<")


def find_build_output() -> Path | None:
    for d in ("_build/html", "_build/dirhtml"):
        p = SRC_ROOT / d
        if p.is_dir():
            return p
    return None


def check_residual_md_links(html_dir: Path) -> int:
    """Return count of files still containing .md links."""
    files_with_md = []
    for f in html_dir.rglob("*.html"):
        text = f.read_text(encoding="utf-8", errors="replace")
        if RESIDUAL_MD_RE.search(text):
            files_with_md.append(str(f.relative_to(html_dir)))
    if files_with_md:
        print(f"[verify] {len(files_with_md)} file(s) still contain .md links:")
        for p in files_with_md[:20]:
            print(f"         {p}")
        if len(files_with_md) > 20:
            print(f"         ... and {len(files_with_md) - 20} more")
    return len(files_with_md)


def check_broken_images(html_dir: Path) -> int:
    """Check <img src="..."> refer to existing files under html_dir."""
    broken = []
    for f in html_dir.rglob("*.html"):
        text = f.read_text(encoding="utf-8", errors="replace")
        for m in IMG_RE.finditer(text):
            src = m.group(1)
            if src.startswith("http:") or src.startswith("https:") or src.startswith("data:"):
                continue
            img_path = (f.parent / src).resolve()
            if not img_path.exists():
                broken.append((str(f.relative_to(html_dir)), src))
    if broken:
        print(f"[verify] {len(broken)} broken image reference(s):")
        for fil, src in broken[:15]:
            print(f"         {fil}  ->  {src}")
        if len(broken) > 15:
            print(f"         ... and {len(broken) - 15} more")
    return len(broken)


def check_empty_sections(html_dir: Path) -> int:
    """Count <section> elements with only whitespace."""
    empty = 0
    for f in html_dir.rglob("*.html"):
        text = f.read_text(encoding="utf-8", errors="replace")
        count = len(SECTION_RE.findall(text))
        if count:
            empty += count
    if empty:
        print(f"[verify] {empty} empty <section> element(s) found")
    return empty


def check_backslash_escapes(html_dir: Path) -> int:
    """Check for visible backslash escapes that shouldn't appear."""
    hits = 0
    for f in html_dir.rglob("*.html"):
        text = f.read_text(encoding="utf-8", errors="replace")
        count = len(BAD_ESCAPE_RE.findall(text))
        if count:
            hits += count
    if hits:
        print(f"[verify] {hits} backslash escape(s) in visible text")
    return hits


def main():
    html_dir = find_build_output()
    if not html_dir:
        print("[verify] No build output found (_build/html or _build/dirhtml)")
        sys.exit(1)

    issues = 0
    issues += check_residual_md_links(html_dir)
    issues += check_broken_images(html_dir)
    issues += check_empty_sections(html_dir)
    issues += check_backslash_escapes(html_dir)

    if issues:
        print(f"\n[verify] {issues} total issue(s) — review above")
    else:
        print("[verify] All checks passed.")
    sys.exit(0 if issues == 0 else 1)


if __name__ == "__main__":
    main()
