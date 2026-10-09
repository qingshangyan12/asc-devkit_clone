#!/usr/bin/env python3
"""对照 gold 评分（v2）：解析答案中的文档与源码引用，计算文档必选查全率、
能力覆盖、替代组覆盖、误报与缺口正确性。

用法:
  python3 score.py --tasks testset/tasks.yaml --answers baseline/results/answers.jsonl \
      --out baseline/results/score.json

answers.jsonl 每行: {"id": "A1", "answer": "<子代理最终回复全文>"}
- 文档必选查全率（主门槛）：required 文档命中比例，路径精确匹配。
- 能力覆盖（辅助）：required 中每个 API，文档命中 或 候选行中该 API 名伴随
  include/impl 源码路径，均记覆盖。
- E 类缺口正确性仅做关键词初筛（correct/pending），由人工终审。
"""
import argparse
import json
import os
import re
import sys

try:
    import yaml
except ImportError:
    sys.exit("需要 PyYAML: pip install pyyaml")

REPO = "/mnt/workspace/asc-devkit"
SCOPE_ROOTS = [
    os.path.join(REPO, "docs/zh/api/SIMD-API/c_api/vector_datamove"),
    os.path.join(REPO, "docs/zh/api/SIMD-API/c_api/vector_compute"),
]
DOC_RE = re.compile(r"docs/zh/api/[A-Za-z0-9_/\.\-]+\.md")
SRC_RE = re.compile(r"(?:include|impl)/[A-Za-z0-9_/\.\-]+\.(?:h|hpp|c|cc|cpp)")
API_RE = re.compile(r"\basc_[a-z0-9_]+")
GAP_OK_WORDS = ("未覆盖", "不在", "范围外", "未找到", "没有找到", "无法找到", "无对应文档", "未包含")
GAP_HINT_WORDS = ("sync/", "reg_compute/")


def build_scope_files():
    files = {}
    for root in SCOPE_ROOTS:
        for dirpath, _, names in os.walk(root):
            for n in names:
                if n.endswith(".md"):
                    rel = os.path.relpath(os.path.join(dirpath, n), REPO)
                    files[n] = rel
                    files[rel] = rel
    return files


def api_name_of(doc_path):
    base = os.path.basename(doc_path)[:-3]
    return re.sub(r"_arch_\d+$", "", base)


def family_hit(doc_path, reported):
    """家族展开：required 为家族主页（无 _arch_ 后缀）时，其 _arch_2201/_arch_3510
    兄弟页命中视为等价（主页是索引页，架构页承载内容）；required 为架构页时保持严格。"""
    if doc_path in reported:
        return True
    if re.search(r"_arch_\d+\.md$", doc_path):
        return False
    d = os.path.dirname(doc_path)
    stem = os.path.basename(doc_path)[:-3]
    return any(os.path.join(d, f"{stem}_arch_{a}.md") in reported
               for a in ("2201", "3510"))


def parse_answer(answer, scope_files):
    docs = {p for p in DOC_RE.findall(answer) if p in scope_files.values()}
    for m in re.finditer(r"\b(asc_[a-z0-9_]+)\.md\b", answer):
        if m.group(0) in scope_files:
            docs.add(scope_files[m.group(0)])
    # 候选条目 = 以 "- 路径" 起始的行（兼容全角冒号与续行）
    entries = []
    cur = None
    for line in answer.splitlines():
        if re.match(r"^\s*-\s*路径\s*[:：]", line):
            cur = line
            entries.append(cur)
        elif cur is not None and line.startswith((" ", "\t", "-", "（", "(")):
            cur += " " + line.strip()
        else:
            cur = None
    src_entries = [e for e in entries if SRC_RE.search(e)]
    src_candidates = {m.group(0) for m in SRC_RE.finditer(answer)}
    src_api_names = set()
    for e in src_entries:
        src_api_names |= set(API_RE.findall(e))
    return docs, src_candidates, src_api_names


def gap_assess(answer, keywords):
    m = re.search(r"##\s*文档?范围内未覆盖\s*\n(.*)", answer, re.S)
    sec = m.group(1) if m else ""
    if not re.sub(r"[\s（()）、。无—-]", "", sec):
        return "pending"
    if not any(k in answer for k in keywords):
        return "pending"
    hint = any(h in answer for h in GAP_HINT_WORDS) or "include/" in answer or "impl/" in answer
    return "correct_with_hint" if hint else "correct"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--tasks", required=True)
    ap.add_argument("--answers", required=True)
    ap.add_argument("--out", required=True)
    args = ap.parse_args()

    with open(args.tasks, encoding="utf-8") as f:
        spec = yaml.safe_load(f)
    nav = set(spec["acceptable_navigation"])
    scope_files = build_scope_files()
    tasks = {t["id"]: t for t in spec["tasks"]}

    answers = {}
    with open(args.answers, encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if line:
                d = json.loads(line)
                answers[d["id"]] = d["answer"]

    results = []
    for tid, t in tasks.items():
        ans = answers.get(tid, "")
        docs, src_candidates, src_api_names = parse_answer(ans, scope_files)
        rec = {"id": tid, "category": t["category"],
               "reported_docs": sorted(docs),
               "reported_source": sorted(src_candidates)}
        required = set(t.get("required") or [])
        groups = [set(g) for g in (t.get("alternative_groups") or [])]
        optional = set(t.get("optional") or [])

        if required:
            hit = {d for d in required if family_hit(d, docs)}
            rec["required_recall"] = round(len(hit) / len(required), 3)
            rec["required_missing"] = sorted(required - hit)
            cap_hit = {d for d in required
                       if family_hit(d, docs) or api_name_of(d) in src_api_names}
            rec["capability_coverage"] = round(len(cap_hit) / len(required), 3)
        if groups:
            def group_ok(g):
                for m in g:
                    if family_hit(m, docs) or api_name_of(m) in src_api_names:
                        return True
                return False
            covered = [i for i, g in enumerate(groups) if group_ok(g)]
            rec["alt_group_coverage"] = round(len(covered) / len(groups), 3)
            rec["alt_groups_all_members"] = all(
                len([m for m in g if family_hit(m, docs)
                     or api_name_of(m) in src_api_names]) == len(g) for g in groups)

        if t.get("gap_expected"):
            rec["gap_assessment"] = gap_assess(ans, t.get("gap_keywords") or [])
            rec["evidence_refs"] = sorted(docs | src_candidates)
            rec["false_positives"] = []
            results.append(rec)
            continue

        acceptable = required | optional | nav
        for g in groups:
            acceptable |= g
        rec["false_positives"] = sorted(docs - acceptable)
        eg = t.get("expected_gap") or []
        if eg:
            m = re.search(r"##\s*文档?范围内未覆盖\s*\n(.*)", ans, re.S)
            sec = m.group(1) if m else ""
            body = re.sub(r"[\s（()）、。：:—\-]", "", sec)
            if not body or body.startswith("无"):
                rec["gap_section"] = "no_gap_claimed"
            elif any(k in sec for k in ("同步", "sync", "初始化", "init",
                                        "寄存器", "reg", "loadalign", "范围外")):
                rec["gap_section"] = "noted"
            else:
                rec["gap_section"] = "other_manual_review"
        results.append(rec)

    def mean(xs):
        xs = [x for x in xs if x is not None]
        return round(sum(xs) / len(xs), 3) if xs else None

    summary = {
        "per_task": results,
        "summary": {
            "tasks": len(results),
            "mean_required_recall": mean([r.get("required_recall") for r in results]),
            "mean_capability_coverage": mean(
                [r.get("capability_coverage") for r in results]),
            "tasks_full_recall": sum(
                1 for r in results if r.get("required_recall") == 1.0),
            "tasks_with_false_positives": sum(
                1 for r in results if r["false_positives"]),
            "gap_correct": sum(1 for r in results
                               if str(r.get("gap_assessment", "")).startswith("correct")),
            "gap_pending_manual": [r["id"] for r in results
                                   if r.get("gap_assessment") == "pending"],
        },
    }
    os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w", encoding="utf-8") as f:
        json.dump(summary, f, ensure_ascii=False, indent=2)
    print(json.dumps(summary["summary"], ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()
