"""源码标注驱动：include Doxygen 注解 + 样例 caller marker。

两个子命令：
  include  : 逐篇 API 文档解析原型/参数表/架构支持，生成 include Doxygen 记录并写入头文件
  examples : 读取 featured/*.json（agent 判定的重点展示 API），精确匹配样例调用点并写入成对 marker

约定见 references/source-annotation-contract.md；默认 dry-run，--write 才落盘。
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from dataclasses import asdict
from difflib import unified_diff
from pathlib import Path
from typing import Any, Iterable, Sequence

from doxygen_annotations import apply_records_to_text, validate_record_target
from example_snippet_markers import (
    apply_insertions_to_text,
    candidate_insertions_for_documents,
)
from extract_api_yaml_metadata import (
    SignatureHeaderMapping,
    _compare_signature_header_mappings,
    _example_calls,
    _is_markdown_table_separator,
    _markdown_table_cells,
    _normalized_parameter_list,
    _parameter_name,
    _parse_prototype_declaration,
    _signatures_by_symbol,
    resolve_signature_header_mappings,
    supported_archs,
)
from prototype_id import PrototypeIdError, signature_identifier

VERIFIED_HEADER_STATUSES = {
    "hint_scope_single",
    "scope_include_single",
    "fallback_single",
}
UNSAFE_HEADER_COMPARISONS = {
    "api_not_found_in_header",
    "not_comparable",
    "parameter_name_difference",
    "parameter_type_mismatch",
    "parameter_count_mismatch",
}
PARAMETER_SECTION_RE = re.compile(r"^\s{0,3}##\s+参数说明(?:\s|[<（(]|$)")
LEVEL_TWO_HEADING_RE = re.compile(r"^\s{0,3}##(?!#)\s+")
DEFAULT_DOC_DIR = "docs/zh/api/SIMD-API/c_api"
DEFAULT_EXAMPLE_DIR = "examples/02_simd_c_api"
SKILL_ROOT = Path(__file__).resolve().parents[1]
REPO_ROOT = Path(__file__).resolve().parents[3]


def _parameter_direction(value: str) -> str | None:
    normalized = re.sub(r"\s+", "", value)
    if "输入" in normalized and "输出" in normalized:
        return "in,out"
    if "输入" in normalized and "输出" not in normalized:
        return "in"
    if "输出" in normalized and "输入" not in normalized:
        return "out"
    return None


def _parameter_documentation_tables(markdown: str) -> list[dict[str, dict[str, str]]]:
    """Read source-proven Doxygen parameter metadata from ``## 参数说明`` tables."""
    lines = markdown.splitlines()
    start = next(
        (index + 1 for index, line in enumerate(lines) if PARAMETER_SECTION_RE.match(line)),
        None,
    )
    if start is None:
        return []
    tables: list[dict[str, dict[str, str]]] = []
    for index in range(start, len(lines) - 1):
        if index > start and LEVEL_TWO_HEADING_RE.match(lines[index]):
            break
        header = _markdown_table_cells(lines[index])
        separators = _markdown_table_cells(lines[index + 1])
        if not header or len(header) != len(separators) or not _is_markdown_table_separator(separators):
            continue
        normalized_header = [re.sub(r"\s+", "", cell) for cell in header]
        name_index = next(
            (pos for pos, cell in enumerate(normalized_header) if cell in {"参数名", "参数", "名称"}),
            None,
        )
        direction_index = next(
            (pos for pos, cell in enumerate(normalized_header) if cell in {"输入/输出", "输入输出", "方向"}),
            None,
        )
        description_index = next(
            (pos for pos, cell in enumerate(normalized_header) if "描述" in cell or "说明" in cell),
            None,
        )
        if name_index is None or direction_index is None or description_index is None:
            continue
        entries: dict[str, dict[str, str]] = {}
        for row_index in range(index + 2, len(lines)):
            cells = _markdown_table_cells(lines[row_index])
            if not cells or len(cells) < len(header):
                break
            name = cells[name_index].strip()
            description = cells[description_index].strip()
            if not name or not description:
                continue
            item = {
                "direction": _parameter_direction(cells[direction_index]) or "",
                "description": description,
                "evidence": lines[row_index].strip(),
            }
            names = [part.strip() for part in re.split(r"[、,，/]", name) if part.strip()]
            for parameter_name in names or [name]:
                entries.setdefault(parameter_name, item)
        if entries:
            tables.append(entries)
    return tables


def _signature_parameter_names(signature: str) -> tuple[str, ...]:
    declaration = _parse_prototype_declaration(signature) or _parse_prototype_declaration(
        f"__aicore__ inline void {signature};"
    )
    if declaration is None:
        return ()
    return tuple(
        _parameter_name(parameter)
        for parameter in _normalized_parameter_list(declaration.parameters or "", keep_names=True)
    )


def _doxygen_params_from_markdown(
    signature: str,
    tables: list[dict[str, dict[str, str]]],
) -> tuple[list[dict[str, Any]], str | None]:
    names = _signature_parameter_names(signature)
    if not names:
        return [], None
    candidates = [table for table in tables if any(name in table for name in names)]
    if not candidates:
        return [], "参数说明表未完整覆盖该声明的命名参数。"
    selected = min(
        candidates,
        key=lambda table: (
            -sum(name in table for name in names),
            len(table) - sum(name in table for name in names),
            tuple(sorted(table)),
        ),
    )
    params: list[dict[str, Any]] = []
    problems: list[str] = []
    for name in names:
        item = selected.get(name)
        if item is None:
            problems.append(f"参数说明表未覆盖参数 {name}。")
            continue
        if not item["direction"]:
            problems.append(f"参数 {name} 的输入/输出方向无法映射为 in、out 或 in,out。")
            continue
        params.append(
            {
                "name": name,
                "direction": item["direction"],
                "description": item["description"],
                "evidence": [item["evidence"]],
            }
        )
    return params, "；".join(problems) if problems else None


def _iter_docs(
    root: Path, source_dirs: Sequence[str], only: Sequence[str], documents: Sequence[str]
) -> list[Path]:
    if documents:
        return sorted((root / document).resolve() for document in documents)
    docs: list[Path] = []
    for source_dir in source_dirs:
        base = (root / source_dir).resolve()
        docs.extend(sorted(base.rglob("*.md")))
    if only:
        docs = [doc for doc in docs if any(token in doc.as_posix() for token in only)]
    return sorted(set(docs))


def _load_signature_cache(run_dir: Path) -> dict[str, Any] | None:
    path = run_dir / "signatures.json"
    if not path.is_file():
        return None
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except json.JSONDecodeError:
        return None


def _signature_from_dict(payload: dict[str, Any]) -> SignatureHeaderMapping:
    return SignatureHeaderMapping(
        name=payload["name"],
        headers=tuple(payload.get("headers", ())),
        status=payload.get("status", ""),
        source=payload.get("source", ""),
        has_header_hint=bool(payload.get("has_header_hint")),
        document_parameters=payload.get("document_parameters", ""),
        canonical_id=payload.get("canonical_id", ""),
        override_id=payload.get("override_id", ""),
        macro_id=payload.get("macro_id", ""),
        scope_headers=tuple(payload.get("scope_headers", ())),
        archs=tuple(payload.get("archs", ())),
        document_signature=payload.get("document_signature", ""),
        prototype_function=payload.get("prototype_function", ""),
        prototype_document_signature=payload.get("prototype_document_signature", ""),
        placeholder_bindings=tuple(
            tuple(item) for item in payload.get("placeholder_bindings", ())
        ),
        template_parameters=tuple(payload.get("template_parameters", ())),
    )


def collect_signatures(
    root: Path,
    source_dirs: Sequence[str],
    run_dir: Path,
    only: Sequence[str],
    documents: Sequence[str],
    *,
    refresh: bool,
) -> tuple[dict[str, tuple[SignatureHeaderMapping, ...]], list[dict[str, Any]], dict[str, list[dict[str, Any]]]]:
    partial = bool(only or documents)
    cached = None if refresh else _load_signature_cache(run_dir)
    by_symbol: dict[str, list[dict[str, Any]]] = {}
    if cached and not partial:
        by_symbol = cached.get("by_symbol", {})
        records_by_header = cached.get("records_by_header", {})
        issues = cached.get("issues", [])
        grouped = {
            symbol: tuple(_signature_from_dict(item) for item in items)
            for symbol, items in by_symbol.items()
        }
        return grouped, issues, records_by_header

    issues: list[dict[str, Any]] = []
    records_by_header: dict[str, list[dict[str, Any]]] = {}
    signatures_by_symbol: dict[str, list[SignatureHeaderMapping]] = {}
    for doc in _iter_docs(root, source_dirs, only, documents):
        rel = doc.relative_to(root).as_posix()
        markdown = doc.read_text(encoding="utf-8", errors="replace")
        try:
            mappings, _candidates = resolve_signature_header_mappings(doc, markdown, root)
        except (ValueError, PrototypeIdError) as exc:
            issues.append({"document": rel, "issue": f"signature mapping failed: {exc}"})
            continue
        document_archs = tuple(supported_archs(markdown))
        tables = _parameter_documentation_tables(markdown)
        comparisons = _compare_signature_header_mappings(markdown, mappings, root)
        comparison_statuses: dict[str, set[str]] = {}
        for comparison in comparisons:
            comparison_statuses.setdefault(comparison.document_parameters, set()).add(
                comparison.status
            )
        for mapping in mappings:
            signatures_by_symbol.setdefault(mapping.name, []).append(mapping)
            record = _build_include_record(
                rel, mapping, document_archs, tables, comparison_statuses, issues
            )
            if record is None:
                continue
            records_by_header.setdefault(record["header"], []).append(record)
    grouped = {symbol: tuple(items) for symbol, items in signatures_by_symbol.items()}
    if not partial:
        payload = {
            "schema_version": 1,
            "by_symbol": {
                symbol: [asdict(item) for item in items]
                for symbol, items in grouped.items()
            },
            "records_by_header": records_by_header,
            "issues": issues,
        }
        run_dir.mkdir(parents=True, exist_ok=True)
        (run_dir / "signatures.json").write_text(
            json.dumps(payload, ensure_ascii=False, indent=1) + "\n", encoding="utf-8"
        )
    return grouped, issues, records_by_header


def _build_include_record(
    rel: str,
    mapping: SignatureHeaderMapping,
    document_archs: Sequence[str],
    tables: list[dict[str, dict[str, str]]],
    comparison_statuses: dict[str, set[str]],
    issues: list[dict[str, Any]],
) -> dict[str, Any] | None:
    headers = [
        header.lstrip("/")
        for header in mapping.headers
        if header.lstrip("/").startswith("include/c_api/")
    ]
    if mapping.macro_id:
        if len(headers) != 1:
            return None
        return {
            "source_document": rel,
            "header": headers[0].lstrip("/"),
            "function": mapping.canonical_id,
            "parameters": mapping.document_signature or mapping.canonical_id,
            "macro_id": mapping.macro_id,
            "archs": [],
            "template_parameters": [],
            "doxygen": {},
        }
    if not mapping.override_id or mapping.status not in VERIFIED_HEADER_STATUSES:
        return None
    if len(headers) != 1:
        return None
    unsafe = comparison_statuses.get(mapping.document_parameters, set()) & UNSAFE_HEADER_COMPARISONS
    if unsafe:
        issues.append(
            {
                "document": rel,
                "issue": f"{mapping.name} 的文档原型与头文件比对未通过（{sorted(unsafe)}），跳过 include 注解",
            }
        )
        return None
    try:
        key, identifier = signature_identifier(
            mapping.canonical_id, source=rel, template_parameters=tuple(mapping.template_parameters)
        )
    except PrototypeIdError as exc:
        issues.append({"document": rel, "issue": f"{mapping.name} override_id 生成失败：{exc}"})
        return None
    if key != "override_id":
        return None
    archs = sorted(dict.fromkeys(mapping.archs or document_archs))
    params, problem = _doxygen_params_from_markdown(
        mapping.document_signature or mapping.canonical_id, tables
    )
    if problem:
        issues.append({"document": rel, "issue": f"{mapping.name} 的 Doxygen 参数未生成：{problem}"})
    return {
        "source_document": rel,
        "header": headers[0].lstrip("/"),
        "function": mapping.canonical_id,
        "parameters": mapping.document_signature or mapping.canonical_id,
        "override_id": mapping.override_id,
        "archs": archs,
        "template_parameters": list(mapping.template_parameters),
        "doxygen": {"params": params} if params else {},
    }


def _bounded_diff(before: str, after: str, limit: int = 120) -> list[str]:
    diff = list(
        unified_diff(
            before.splitlines(),
            after.splitlines(),
            lineterm="",
            n=1,
        )
    )
    return diff[:limit]


def run_include(args: argparse.Namespace) -> int:
    root = Path(args.devkit_root).resolve()
    run_dir = Path(args.run_dir).resolve()
    grouped, issues, records_by_header = collect_signatures(
        root, args.source_dir, run_dir, args.only, args.document, refresh=args.refresh
    )
    if args.list_mappings:
        for symbol in sorted(grouped):
            print(symbol, len(grouped[symbol]))
        return 0
    report: dict[str, Any] = {
        "mode": "write" if args.write else "dry-run",
        "documents_with_records": len({r["source_document"] for records in records_by_header.values() for r in records}),
        "headers": [],
        "issues": issues,
    }
    total_records = 0
    changed = 0
    for header in sorted(records_by_header):
        records = records_by_header[header]
        total_records += len(records)
        path = root / header
        entry: dict[str, Any] = {"header": header, "records": len(records)}
        if not path.is_file():
            entry["status"] = "header-missing"
            report["headers"].append(entry)
            continue
        text = path.read_text(encoding="utf-8")
        valid: list[dict[str, Any]] = []
        for record in records:
            try:
                validate_record_target(text, record)
            except Exception as exc:  # noqa: BLE001 - report and drop invalid target
                entry.setdefault("skipped", []).append(
                    {"override_id": record.get("override_id") or record.get("macro_id"), "reason": str(exc)}
                )
                continue
            valid.append(record)
        try:
            new_text, applied = apply_records_to_text(text, valid)
        except Exception as exc:  # noqa: BLE001 - one bad header must not abort the run
            entry["status"] = f"apply-failed: {exc}"
            report["headers"].append(entry)
            continue
        entry["applied"] = applied
        if new_text == text:
            entry["status"] = "unchanged"
        else:
            changed += 1
            entry["status"] = "updated" if args.write else "would-update"
            entry["added_lines"] = sum(
                1 for line in _bounded_diff(text, new_text, limit=100000) if line.startswith("+") and not line.startswith("+++")
            )
            entry["diff"] = _bounded_diff(text, new_text)
            if args.write:
                path.write_text(new_text, encoding="utf-8")
        report["headers"].append(entry)
    report["total_records"] = total_records
    report["changed_headers"] = changed
    report["unresolved_count"] = sum(1 for item in issues if "跳过 include" in item.get("issue", "") or "未生成" in item.get("issue", ""))
    run_dir.mkdir(parents=True, exist_ok=True)
    (run_dir / "include-report.json").write_text(
        json.dumps(report, ensure_ascii=False, indent=1) + "\n", encoding="utf-8"
    )
    _write_include_markdown(run_dir / "include-report.md", report)
    print(
        f"[include] {'wrote' if args.write else 'dry-run'}: "
        f"records={total_records} headers={len(records_by_header)} changed={changed} issues={len(issues)}"
    )
    if args.write:
        for header in report["headers"]:
            if header.get("diff"):
                header.pop("diff")
    return 0


def _write_include_markdown(path: Path, report: dict[str, Any]) -> None:
    lines = [
        "# include 注解报告",
        "",
        f"- 模式：{report['mode']}",
        f"- 记录数：{report['total_records']}，涉及头文件：{len(report['headers'])}，变更头文件：{report['changed_headers']}",
        "",
        "| 头文件 | 记录 | 状态 | 新增行 |",
        "| --- | --- | --- | --- |",
    ]
    for entry in report["headers"]:
        lines.append(
            f"| `{entry['header']}` | {entry['records']} | {entry.get('status', '')} | {entry.get('added_lines', 0)} |"
        )
    if report["issues"]:
        lines.extend(["", "## 问题", ""])
        for item in report["issues"][:200]:
            lines.append(f"- {item.get('document', '')}: {item.get('issue', '')}")
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def run_examples(args: argparse.Namespace) -> int:
    root = Path(args.devkit_root).resolve()
    run_dir = Path(args.run_dir).resolve()
    grouped, issues, _ = collect_signatures(
        root, [DEFAULT_DOC_DIR], run_dir, args.only, args.document, refresh=args.refresh
    )
    featured_entries: list[dict[str, Any]] = []
    for featured_path in sorted(Path(args.featured_dir).resolve().glob("*.json")):
        payload = json.loads(featured_path.read_text(encoding="utf-8"))
        entries = payload if isinstance(payload, list) else payload.get("examples", [])
        featured_entries.extend(entries)
    if not featured_entries:
        print("no featured entries found", file=sys.stderr)
        return 2

    report: dict[str, Any] = {"mode": "write" if args.write else "dry-run", "examples": [], "issues": issues}
    total_insertions = 0
    for entry in sorted(featured_entries, key=lambda item: item.get("example", "")):
        example = entry.get("example", "")
        apis = [api for api in entry.get("apis", []) if api]
        project = root / example
        info: dict[str, Any] = {"example": example, "apis": apis}
        if not project.is_dir():
            info["status"] = "example-missing"
            report["examples"].append(info)
            continue
        unknown = [api for api in apis if api not in grouped]
        if unknown:
            info["unknown_apis"] = unknown
        symbols = {api: grouped[api] for api in apis if api in grouped}
        if not symbols:
            info["status"] = "no-mapped-apis"
            report["examples"].append(info)
            continue
        calls, call_issues = _example_calls(project, root, symbols)
        info["call_issues"] = [asdict(item) for item in call_issues]
        confirmed = [
            call
            for call in calls
            if call.overload_status == "exact"
            and isinstance(call.canonical_id, str)
            and isinstance(call.file, str)
            and call.is_statement
            and call.inferred_types is not None
            and all(isinstance(value, str) and value for value in call.inferred_types)
        ]
        info["calls_exact"] = len(confirmed)
        if not confirmed:
            info["status"] = "no-exact-calls"
            report["examples"].append(info)
            continue
        record = {"path": example, "confidence": "authoritative", "issues": (), "calls": [asdict(call) for call in confirmed]}
        pseudo_api = f"featured:{example}"
        insertions, skips = candidate_insertions_for_documents(((pseudo_api, [record]),), root)
        info["skips"] = [
            {"file": str(skip.file_path.relative_to(root)) if skip.file_path else "", "marker_id": skip.marker_id, "reason": skip.reason}
            for skip in skips
        ]
        by_file: dict[Path, list[Any]] = {}
        for insertion in insertions:
            by_file.setdefault(insertion.file_path, []).append(insertion)
        info["files"] = []
        for file_path in sorted(by_file):
            rel_file = file_path.relative_to(root).as_posix()
            text = file_path.read_text(encoding="utf-8", errors="replace")
            new_text = apply_insertions_to_text(text, by_file[file_path])
            file_info: dict[str, Any] = {"file": rel_file, "insertions": len(by_file[file_path])}
            if new_text != text:
                total_insertions += len(by_file[file_path])
                file_info["status"] = "updated" if args.write else "would-update"
                file_info["diff"] = _bounded_diff(text, new_text)
                if args.write:
                    file_path.write_text(new_text, encoding="utf-8")
            else:
                file_info["status"] = "unchanged"
            info["files"].append(file_info)
        info["status"] = "done"
        report["examples"].append(info)
    run_dir.mkdir(parents=True, exist_ok=True)
    (run_dir / "examples-report.json").write_text(
        json.dumps(report, ensure_ascii=False, indent=1) + "\n", encoding="utf-8"
    )
    print(
        f"[examples] {'wrote' if args.write else 'dry-run'}: "
        f"examples={len(report['examples'])} insertions={total_insertions} issues={len(issues)}"
    )
    return 0


def parser() -> argparse.ArgumentParser:
    p = argparse.ArgumentParser(description="源码标注驱动（include Doxygen + 样例 marker）")
    sub = p.add_subparsers(dest="command", required=True)

    common = argparse.ArgumentParser(add_help=False)
    common.add_argument("--devkit-root", default=str(REPO_ROOT))
    common.add_argument("--run-dir", default=str(SKILL_ROOT / "tmp" / "source-annotate"))
    common.add_argument("--only", action="append", default=[], help="只处理路径含该子串的文档（可多次）")
    common.add_argument("--document", action="append", default=[], help="精确处理指定文档（仓内相对路径，可多次）")
    common.add_argument("--refresh", action="store_true", help="忽略 signatures.json 缓存")
    common.add_argument("--write", action="store_true", help="落盘；默认 dry-run")

    inc = sub.add_parser("include", parents=[common], help="生成并写入 include Doxygen 注解")
    inc.add_argument("--source-dir", action="append", default=[], help="文档目录（默认 c_api）")
    inc.add_argument("--list-mappings", action="store_true", help="仅按 symbol 打印映射统计")

    exa = sub.add_parser("examples", parents=[common], help="读取 featured 名单并写入样例 marker")
    exa.add_argument("--featured-dir", default=str(SKILL_ROOT / "tmp" / "source-annotate" / "featured"))
    exa.add_argument("--source-dir", action="append", default=[], help="样例目录（默认 examples/02_simd_c_api）")
    return p


def main(argv: Sequence[str] | None = None) -> int:
    args = parser().parse_args(argv)
    if args.command == "include":
        if not args.source_dir:
            args.source_dir = [DEFAULT_DOC_DIR]
        return run_include(args)
    if not args.source_dir:
        args.source_dir = [DEFAULT_EXAMPLE_DIR]
    return run_examples(args)


if __name__ == "__main__":
    raise SystemExit(main())
