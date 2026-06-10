#!/usr/bin/env python3
"""Query the api-doc-mapping.json index.

Usage:
  python query.py --function Add
  python query.py --header kernel_operator_vec_reduce_intf
  python query.py --tier simt_api
  python query.py --unreferenced
  python query.py --stats
"""

import argparse
import json
import sys
from pathlib import Path

MAPPING_PATH = Path(__file__).resolve().parents[1] / "references" / "api-doc-mapping.json"


def _load():
    with open(MAPPING_PATH) as f:
        return json.load(f)


def _format_entry(r, show_tier=False):
    conf_label = f"[{r['confidence']}]"
    lines = [f"  {r['name']}  {conf_label}"]
    if show_tier:
        lines.append(f"    tier:     {r['tier']}")
    lines.append(f"    header:   {r['header']}")
    lines.append(f"    doc:      {r['doc_path']}")
    return "\n".join(lines)


def cmd_function(data, name):
    name_lower = name.lower()
    matches = [r for r in data["functions"] if r["name"].lower() == name_lower]

    if not matches:
        # Check unmapped
        unmapped = [r for r in data["unmapped_docs"] if r.get("name", "").lower() == name_lower]
        if unmapped:
            print(f"Not in main index (unmapped):")
            for r in unmapped:
                print(f"  {r['name']} [{r['confidence']}]")
                print(f"    tier:     {r['tier']}")
                print(f"    doc:      {r['doc_path']}")
                if r.get("header"):
                    print(f"    candidates: {r['header']}")
        else:
            print(f"No entry found for '{name}'.")
            print("The mapping data may be stale — try regenerating or fallback search.")
        return

    print(f"Found {len(matches)} entries for '{name}':\n")
    for r in sorted(matches, key=lambda r: (r["confidence"] != "HIGH", r["tier"])):
        print(_format_entry(r, show_tier=True))
        print()


def cmd_header(data, fragment):
    matches = [r for r in data["functions"] if fragment in r.get("header", "")]
    if not matches:
        # Check unreferenced
        unref = [h for h in data["unreferenced_headers"] if fragment in h["header"]]
        if unref:
            print(f"Header '{fragment}' has no documented functions:")
            for h in unref:
                reason = h.get("reason") or "no doc coverage"
                print(f"  {h['header']} ({h['tier']}) — {reason}")
        else:
            print(f"No functions found for header matching '{fragment}'.")
        return

    print(f"Found {len(matches)} documented functions in header matching '{fragment}':\n")
    funcs = sorted(set(r["name"] for r in matches))
    print(f"  Functions: {', '.join(funcs)}")
    print()

    # Show tier breakdown
    tiers = sorted(set(r["tier"] for r in matches))
    for t in tiers:
        t_funcs = sorted(set(r["name"] for r in matches if r["tier"] == t))
        t_confs = set(r["confidence"] for r in matches if r["tier"] == t)
        print(f"  [{t}] confidence: {', '.join(sorted(t_confs))}")
        for f in t_funcs:
            r = [r for r in matches if r["name"] == f and r["tier"] == t][0]
            print(f"    {f} → {r['doc_path']}")
    print()


def cmd_tier(data, tier):
    matches = [r for r in data["functions"] if r["tier"] == tier]
    if not matches:
        print(f"No entries for tier '{tier}'.")
        return

    funcs = sorted(set(r["name"] for r in matches))
    high = sum(1 for r in matches if r["confidence"] == "HIGH")
    med = sum(1 for r in matches if r["confidence"] == "MEDIUM")
    print(f"Tier '{tier}': {len(funcs)} unique functions ({high} HIGH, {med} MEDIUM)\n")
    for f in funcs:
        entries = [r for r in matches if r["name"] == f]
        for r in entries:
            print(_format_entry(r))
        print()


def cmd_unreferenced(data, tier=None):
    headers = data["unreferenced_headers"]
    if tier:
        headers = [h for h in headers if h["tier"] == tier]

    print(f"Unreferenced headers (no doc coverage): {len(headers)}\n")
    by_tier = {}
    for h in headers:
        by_tier.setdefault(h["tier"], []).append(h)

    for t in sorted(by_tier):
        print(f"  [{t}] ({len(by_tier[t])} headers)")
        for h in sorted(by_tier[t], key=lambda x: x["header"]):
            reason = f" — {h['reason']}" if h.get("reason") else ""
            print(f"    {h['header']}{reason}")
        print()


def cmd_stats(data):
    meta = data["metadata"]
    print(f"Generated: {meta['generated_at']}")
    print(f"Total function mappings: {meta['total_functions']}")
    print(f"  HIGH:   {meta['high_confidence']}")
    print(f"  MEDIUM: {meta['medium_confidence']}")
    print(f"Unreferenced headers:    {meta['unreferenced_headers']}")
    print(f"Unmapped docs:           {meta['unmapped_docs']}")
    print()

    # Per-tier breakdown
    tiers = {}
    for r in data["functions"]:
        t = r["tier"]
        tiers.setdefault(t, {"total": 0, "HIGH": 0, "MEDIUM": 0})
        tiers[t]["total"] += 1
        tiers[t][r["confidence"]] += 1

    print("Per tier:")
    for t in sorted(tiers):
        info = tiers[t]
        print(f"  {t:12s}  {info['total']:4d} total  ({info['HIGH']:3d} HIGH, {info['MEDIUM']:3d} MEDIUM)")


def main():
    parser = argparse.ArgumentParser(description="Query asc-api-doc-mapper index")
    parser.add_argument("--function", "-f", help="Find by function name (case-insensitive)")
    parser.add_argument("--header", "-H", help="Find by header filename (partial match)")
    parser.add_argument("--tier", "-t", help="List all functions in a tier")
    parser.add_argument("--unreferenced", "-u", action="store_true", help="List headers without doc coverage")
    parser.add_argument("--unreferenced-tier", help="Filter unreferenced by tier")
    parser.add_argument("--stats", "-s", action="store_true", help="Show summary statistics")
    args = parser.parse_args()

    if not MAPPING_PATH.exists():
        print(f"Error: mapping data not found at {MAPPING_PATH}")
        print("Run: python .agent/skills/asc-api-doc-mapper/scripts/regenerate.py")
        sys.exit(1)

    data = _load()

    if args.function:
        cmd_function(data, args.function)
    elif args.header:
        cmd_header(data, args.header)
    elif args.tier:
        cmd_tier(data, args.tier)
    elif args.unreferenced or args.unreferenced_tier:
        cmd_unreferenced(data, args.unreferenced_tier)
    elif args.stats:
        cmd_stats(data)
    else:
        parser.print_help()


if __name__ == "__main__":
    main()
