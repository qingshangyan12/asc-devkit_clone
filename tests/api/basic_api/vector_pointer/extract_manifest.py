"""Import the September VECTOR scope without changing the source workbook."""
import argparse
import hashlib
import json
from collections import Counter
from pathlib import Path

import openpyxl


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("workbook", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    workbook = openpyxl.load_workbook(args.workbook, read_only=True, data_only=True)
    entries = []
    for sheet in workbook:
        for row_number, row in enumerate(sheet.values, 1):
            if len(row) < 5 or not isinstance(row[1], int):
                continue
            entries.append(dict(sheet=sheet.title, row=row_number, name=row[0],
                                overload=row[1], declared_overloads=row[2],
                                header=row[3], signature=row[4].replace("\\n", "\n")))
    counts = Counter(entry["name"] for entry in entries)
    assert all(counts[e["name"]] == e["declared_overloads"] for e in entries)
    manifest = dict(task="September 2026 VECTOR pointer extension",
                    source=args.workbook.name,
                    sha256=hashlib.sha256(args.workbook.read_bytes()).hexdigest(),
                    api_count=len(counts), signature_count=len(entries), entries=entries)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    print(f"Imported {len(counts)} API names / {len(entries)} signatures")


if __name__ == "__main__":
    main()
