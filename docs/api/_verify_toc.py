import yaml
from pathlib import Path

BASE = Path(__file__).resolve().parent
data = yaml.safe_load(open(BASE / "_toc.yml.new", encoding="utf-8"))
entries = data["entries"]

for e in entries:
    title = e.get("title", "")
    file = e.get("file", "")
    has_entries = bool(e.get("entries"))
    print(f"[{title}] file={file} has_entries={has_entries}")
    if has_entries:
        for sub in e["entries"][:5]:
            stitle = sub.get("title", "")
            sfile = sub.get("file", "")
            shentries = bool(sub.get("entries"))
            print(f"  [{stitle}] file={sfile} has_entries={shentries}")
        if len(e["entries"]) > 5:
            print(f"  ... ({len(e['entries'])} total)")
