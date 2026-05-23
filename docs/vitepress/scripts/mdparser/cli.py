import argparse
import os
import sys
from pathlib import Path

from .parser import parse_file, parse_string, render_html


def main():
    parser = argparse.ArgumentParser(
        prog="mdparser",
        description="High-performance Markdown-to-HTML converter based on cmark-gfm (C).",
    )
    parser.add_argument("input", nargs="*", help="Markdown file(s) or directory to parse")
    parser.add_argument("-o", "--output", help="Output file (single mode) or output directory (batch mode)")
    parser.add_argument("--no-gfm", action="store_true", help="Disable GitHub Flavored Markdown extensions")
    parser.add_argument("--stdin", action="store_true", help="Force read from stdin")
    parser.add_argument("-s", "--style", action="store_true", help="Embed GitHub-style CSS in output")
    parser.add_argument("-r", "--recursive", action="store_true", help="Recursively scan directories for .md files")

    args = parser.parse_args()

    md_files = _collect_files(args.input, args.recursive)

    if not md_files and not args.stdin:
        parser.print_help()
        sys.exit(1)

    if args.stdin:
        content = sys.stdin.buffer.read().decode("utf-8")
        html_body = parse_string(content, gfm=not args.no_gfm)
        html = _build_html(html_body, styled=args.style)
        if args.output:
            Path(args.output).write_text(html, encoding="utf-8")
        else:
            sys.stdout.write(html)
        return

    if len(md_files) == 1 and not (args.output and os.path.isdir(args.output)):
        html_body = parse_file(md_files[0], gfm=not args.no_gfm)
        html = _build_html(html_body, styled=args.style)
        if args.output:
            Path(args.output).write_text(html, encoding="utf-8")
        else:
            sys.stdout.write(html)
        return

    out_dir = Path(args.output) if args.output else Path.cwd()
    out_dir.mkdir(parents=True, exist_ok=True)

    common_root = _find_common_root(md_files)
    for src in md_files:
        html_body = parse_file(src, gfm=not args.no_gfm)
        html = _build_html(html_body, styled=args.style)
        rel = Path(src).relative_to(common_root).with_suffix(".html")
        dst = out_dir / rel
        dst.parent.mkdir(parents=True, exist_ok=True)
        dst.write_text(html, encoding="utf-8")
        print(f"  {src} -> {dst}")


def _find_common_root(paths):
    if not paths:
        return Path(".")
    root = Path(paths[0]).parent
    for p in paths[1:]:
        pp = Path(p).parent
        while root not in pp.parents and root != pp:
            root = root.parent
    return root


def _collect_files(paths, recursive):
    files = []
    for p in paths:
        pp = Path(p)
        if pp.is_dir():
            pattern = "**/*.md" if recursive else "*.md"
            for f in sorted(pp.glob(pattern)):
                if f.is_file():
                    files.append(str(f))
        elif pp.is_file() and pp.suffix == ".md":
            files.append(str(p))
    return files


def _build_html(body, styled=False):
    if not styled:
        return body
    return render_html(body)
