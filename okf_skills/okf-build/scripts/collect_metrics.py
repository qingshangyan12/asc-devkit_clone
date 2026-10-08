#!/usr/bin/env python3
"""从 opencode 会话存储采集单任务的效率指标与隔离审计数据（v2 白名单口径）。

用法:
  python3 collect_metrics.py --start-ms <epoch_ms> --end-ms <epoch_ms> \
      --task-id A1 --out baseline/results/A1.metrics.json

定位方式: session 表中 agent='explore' 且 time_created 落在 [start-ms, end-ms] 的子会话。
白名单（v2）: 计分语料两个文档目录 ∪ include ∪ impl ∪ /tmp；
白名单外访问（含 examples/、okf-skills/、docs 其他目录）记违规。
输出: 指标 JSON（含 token、会话级耗时、工具调用统计、文档/源码读取量、违规清单）。
"""
import argparse
import re
import json
import os
import sqlite3
import sys

DB_PATH = os.path.expanduser("~/.local/share/opencode/opencode.db")
REPO = "/mnt/workspace/asc-devkit"
DOC_SCOPE = (
    "docs/zh/api/SIMD-API/c_api/vector_datamove",
    "docs/zh/api/SIMD-API/c_api/vector_compute",
)
SOURCE_SCOPE = ("include", "impl")
ALLOWED_PREFIXES = DOC_SCOPE + SOURCE_SCOPE
HARD_FORBIDDEN = ("examples/", "okf-skills")


def connect_ro():
    return sqlite3.connect(f"file:{DB_PATH}?mode=ro", uri=True)


def find_sessions(cur, start_ms, end_ms):
    return cur.execute(
        "SELECT id, model, time_created, time_updated, tokens_input, tokens_output, "
        "tokens_reasoning, tokens_cache_read, tokens_cache_write, cost "
        "FROM session WHERE agent='explore' AND time_created >= ? AND time_created <= ? "
        "ORDER BY time_created",
        (start_ms, end_ms),
    ).fetchall()


def rel_of(target):
    if not isinstance(target, str):
        return None
    if target.startswith(REPO + "/"):
        return target[len(REPO) + 1:]
    if target.startswith("/"):
        return target
    return target  # 相对路径或模式串


def classify_path(rel):
    """返回 (类别, 白名单内?)。类别: doc / source / tmp / outside。"""
    if rel is None:
        return ("none", True)
    if rel.startswith(DOC_SCOPE):
        return ("doc", True)
    if rel.startswith(SOURCE_SCOPE):
        return ("source", True)
    if rel.startswith("/tmp") or rel.startswith("tmp/"):
        return ("tmp", True)
    if not rel.startswith("/") and "/" not in rel and "." not in rel:
        return ("pattern", True)  # glob 模式串等无路径目标
    return ("outside", False)


def classify_target(path, pattern):
    """path+pattern 联合判定：任一以白名单前缀开头即合规（glob/grep 场景）。"""
    for p in (pattern or "", path or ""):
        rel = rel_of(p) if p else None
        if rel is None:
            continue
        if rel.startswith(DOC_SCOPE):
            return "doc", True
        if rel.startswith(SOURCE_SCOPE):
            return "source", True
        if rel.startswith("/tmp") or rel.startswith("tmp/"):
            return "tmp", True
    rel = rel_of(path) if path else None
    if rel is None or (not rel.startswith("/") and "/" not in rel and "." not in rel
                       and not (pattern or "").strip("/").startswith(tuple(ALLOWED_PREFIXES))):
        return "pattern", True
    return "outside", False


def audit_parts(cur, session_id):
    rows = cur.execute(
        "SELECT data FROM part WHERE session_id=? AND json_extract(data,'$.type')='tool'",
        (session_id,),
    ).fetchall()
    tools, violations = {}, []
    doc_reads, source_reads = [], []

    def check_output_leakage(output):
        for p in re.findall(r"/mnt/workspace/asc-devkit/[^\s:,\"]+", str(output)):
            rel = p[len(REPO) + 1:]
            kind, ok = classify_path(rel)
            if not ok:
                return rel
        return None

    for (raw,) in rows:
        d = json.loads(raw)
        tool = d.get("tool", "?")
        tools[tool] = tools.get(tool, 0) + 1
        inp = (d.get("state") or {}).get("input") or {}
        blob = json.dumps(inp, ensure_ascii=False)

        hard = next((m for m in HARD_FORBIDDEN if m in blob), None)
        if hard:
            violations.append({"tool": tool, "severity": "leakage",
                               "detail": f"输入指向 {hard}", "input": blob[:300]})
            continue

        path = inp.get("filePath") or inp.get("path") or inp.get("workdir")
        pattern = inp.get("pattern") or ""
        kind, ok = classify_target(path, pattern)
        if not ok:
            out = (d.get("state") or {}).get("output") or ""
            leak = check_output_leakage(out)
            severity = "leakage" if leak else "scan_breach"
            violations.append({"tool": tool, "severity": severity,
                               "detail": (f"{path} {pattern}".strip()
                                          + (f"；输出含范围外路径 {leak}" if leak else "；输出无范围外内容")),
                               "input": blob[:300]})
            continue
        if tool == "read" and kind == "doc" and path.endswith(".md"):
            full = path if os.path.isabs(path) else os.path.join(REPO, path)
            doc_reads.append({"path": rel_of(path),
                              "bytes": os.path.getsize(full) if os.path.exists(full) else 0})
        elif tool == "read" and kind == "source":
            full = path if os.path.isabs(path) else os.path.join(REPO, path)
            source_reads.append({"path": rel_of(path),
                                 "bytes": os.path.getsize(full) if os.path.exists(full) else 0})
    return tools, doc_reads, source_reads, violations


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--start-ms", type=int, required=True)
    ap.add_argument("--end-ms", type=int, required=True)
    ap.add_argument("--task-id", required=True)
    ap.add_argument("--out", required=True)
    args = ap.parse_args()

    con = connect_ro()
    cur = con.cursor()
    sessions = find_sessions(cur, args.start_ms, args.end_ms)
    if len(sessions) != 1:
        print(f"ERROR: expected 1 explore session in window, got {len(sessions)}",
              file=sys.stderr)
        for s in sessions:
            print("  candidate:", s[0], file=sys.stderr)
        sys.exit(2)
    s = sessions[0]
    (sid, model, tc, tu, ti, to, tr, tcr, tcw, cost) = s
    tools, doc_reads, source_reads, violations = audit_parts(cur, sid)
    metrics = {
        "task_id": args.task_id,
        "session_id": sid,
        "model": model,
        "duration_s": round((tu - tc) / 1000.0, 1),
        "tokens_input": ti,
        "tokens_output": to,
        "tokens_reasoning": tr,
        "tokens_total": (ti or 0) + (to or 0) + (tr or 0),
        "tokens_cache_read": tcr,
        "tokens_cache_write": tcw,
        "cost": cost,
        "tool_calls": tools,
        "docs_read_in_scope": len(doc_reads),
        "bytes_read_docs": sum(r["bytes"] for r in doc_reads),
        "source_files_read": len(source_reads),
        "bytes_read_source": sum(r["bytes"] for r in source_reads),
        "isolation_violations": violations,
        "isolation_ok": not violations,
    }
    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w", encoding="utf-8") as f:
        json.dump(metrics, f, ensure_ascii=False, indent=2)
    print(json.dumps({k: metrics[k] for k in
                      ("task_id", "duration_s", "tokens_total",
                       "docs_read_in_scope", "source_files_read", "isolation_ok")},
                     ensure_ascii=False))


if __name__ == "__main__":
    main()
