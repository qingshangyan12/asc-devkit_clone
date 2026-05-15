import json
import re
import yaml
from pathlib import Path

SCRIPTS_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPTS_DIR.parent


def parse_readme_toc():
    readme = PROJECT_ROOT / "README.md"
    with open(readme, "r", encoding="utf-8") as f:
        lines = f.readlines()

    root_entries = []
    stack = [(0, None)]
    link_re = re.compile(r"^(\s*)-\s*\[(.+?)\]\((.+?)\)")

    for line in lines:
        m = link_re.match(line)
        if not m:
            continue

        indent_str, text, link = m.groups()

        if any(link.lower().endswith(ext) for ext in (".png", ".jpg", ".gif", ".svg", ".pdf")):
            continue

        ref = re.sub(r"\.md$", "", link)
        indent = len(indent_str)
        level = indent // 4

        entry = {"file": ref, "_text": text}

        while len(stack) > 1 and stack[-1][0] >= level:
            stack.pop()

        parent_level, parent = stack[-1]

        if level == 0 or parent is None:
            root_entries.append(entry)
        else:
            parent.setdefault("entries", []).append(entry)

        stack.append((level, entry))

    return root_entries


def add_numbering(entries, prefix):
    for i, entry in enumerate(entries, 1):
        num = ".".join(str(p) for p in prefix) + "." + str(i) if prefix else str(i)
        entry["title"] = num + " " + entry["_text"]

        if entry.get("entries"):
            add_numbering(entry["entries"], prefix + (i,))


def remove_temp_keys(entries):
    for entry in entries:
        entry.pop("_text", None)
        if "entries" in entry:
            remove_temp_keys(entry["entries"])


def show_structure(entries, indent=0):
    prefix = "  " * indent
    for e in entries:
        f = e.get("file", "(no file)")
        t = e.get("title", "")
        subs = len(e.get("entries", []))
        label = f"{prefix}{f[:70]:70s}"
        if t:
            label += f"  [{t[:50]}]"
        print(f"{label}  sub={subs}")
        if subs:
            show_structure(e["entries"], indent + 1)


def count_entries(entries):
    n = 0
    for e in entries:
        n += 1
        if "entries" in e:
            n += count_entries(e["entries"])
    return n


def main():
    toc_entries = parse_readme_toc()

    for entry in toc_entries:
        entry["title"] = entry["_text"]
        if entry.get("entries"):
            add_numbering(entry["entries"], ())

    remove_temp_keys(toc_entries)

    toc = {"root": "index", "entries": toc_entries}

    total = count_entries(toc_entries)
    print(f"Total entries: {total}")
    print(f"Top-level: {len(toc_entries)}")
    show_structure(toc_entries)
    print(f"  ...")

    toc_path = SCRIPTS_DIR / "_toc.yml"
    with open(toc_path, "w", encoding="utf-8") as f:
        yaml.dump(toc, f, allow_unicode=True, default_flow_style=False, sort_keys=False, width=200)

    _unescape_yaml(toc_path)

    print(f"\nWritten to: {toc_path}")

    json_data = [_compact(e) for e in toc_entries]
    json_str = json.dumps(json_data, ensure_ascii=False, separators=(',', ':'))

    json_path = PROJECT_ROOT / "_static" / "toc_data.json"
    json_path.write_text(json_str, encoding="utf-8")
    print(f"Written to: {json_path} ({json_path.stat().st_size / 1024:.0f} KB)")

    js_path = PROJECT_ROOT / "_static" / "js"
    js_path.mkdir(parents=True, exist_ok=True)
    js_path = js_path / "toc-data.js"
    js_path.write_text("var TOC_DATA_EMBEDDED = " + json_str + ";", encoding="utf-8")
    print(f"Written to: {js_path} ({js_path.stat().st_size / 1024:.0f} KB)")


def _compact(entry):
    obj = {"t": entry.get("title", ""), "f": entry.get("file", "")}
    if "entries" in entry:
        obj["e"] = [_compact(e) for e in entry["entries"]]
    return obj


def _unescape_yaml(path):
    """Strip PyYAML's unnecessary backslash-escapes from a YAML file.

    PyYAML 6.0 escapes _, (, ), [, ] etc. in large nested documents,
    producing title values with spurious backslash characters before
    underscores, parentheses, and brackets. These are not real YAML escape
    sequences and cause literal backslashes to appear in the sidebar
    when sphinx_external_toc parses the file.
    """
    import re as _re

    content = path.read_text(encoding="utf-8")
    # Characters PyYAML wrongly escapes: _ ( ) [ ] { } < > # & * ! | %
    # We NEVER unescape \\  \n  \t  \r  etc.
    cleaned = _re.sub(r"\\(" + "[]_[(){}])", r"\1", content)
    path.write_text(cleaned, encoding="utf-8")


if __name__ == "__main__":
    main()
