#!/usr/bin/env python3
"""Validate Agent-authored API descriptions and write only the description field."""

from __future__ import annotations

import argparse
from datetime import datetime, timezone
from hashlib import sha256
import json
from pathlib import Path
import re
import sys
from typing import Any


SCHEMA_VERSION = 1
RUN_ID_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._-]*$")
API_PREFIX = Path("docs/zh/api/SIMD-API")
MAX_DESCRIPTION_LENGTH = 180
MIN_DESCRIPTION_LENGTH = 8


def now_utc() -> str:
    return datetime.now(timezone.utc).replace(microsecond=0).isoformat()


def digest(path: Path) -> str:
    return sha256(path.read_bytes()).hexdigest()


def read_json(path: Path) -> dict[str, Any]:
    payload = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(payload, dict):
        raise ValueError(f"JSON 根节点必须是对象：{path}")
    return payload


def write_json(path: Path, payload: dict[str, Any]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(payload, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")


def strip_front_matter(text: str) -> str:
    if text.startswith("---\n"):
        end = text.find("\n---\n", 4)
        if end >= 0:
            return text[end + len("\n---\n") :]
    return text


def front_matter_bounds(text: str) -> tuple[int, int]:
    if not text.startswith("---\n"):
        raise ValueError("目标 Markdown 缺少 front matter，不能只写入 description")
    end = text.find("\n---\n", 4)
    if end < 0:
        raise ValueError("目标 Markdown 的 front matter 结束标记无效")
    return 4, end + 1


def first_heading(body: str, fallback: str) -> str:
    match = re.search(r"(?m)^#\s+(.+?)\s*$", body)
    return match.group(1).strip() if match else fallback


def resolve_document(root: Path, value: str) -> tuple[str, Path]:
    candidate = Path(value)
    if candidate.is_absolute():
        raise ValueError("document 必须是 DevKit 根目录相对路径")
    normalized = Path(*candidate.parts)
    if normalized.suffix != ".md" or normalized.is_absolute() or ".." in normalized.parts:
        raise ValueError("document 必须是 docs/zh/api/SIMD-API 下的 Markdown 路径")
    try:
        normalized.relative_to(API_PREFIX)
    except ValueError as error:
        raise ValueError("document 必须位于 docs/zh/api/SIMD-API 下") from error
    path = (root / normalized).resolve()
    try:
        path.relative_to(root.resolve())
    except ValueError as error:
        raise ValueError("document 超出 DevKit 根目录") from error
    if not path.is_file():
        raise FileNotFoundError(f"目标 Markdown 不存在：{normalized.as_posix()}")
    return normalized.as_posix(), path


def load_manifest(run_dir: Path) -> dict[str, Any]:
    manifest = read_json(run_dir / "run-manifest.json")
    if manifest.get("schema_version") != SCHEMA_VERSION or manifest.get("purpose") != "okf_api_description_run":
        raise ValueError("run-manifest.json 不是有效的 description 运行清单")
    if manifest.get("run_dir") != str(run_dir.resolve()):
        raise ValueError("run-dir 与运行清单不一致")
    return manifest


def task_from_manifest(manifest: dict[str, Any]) -> tuple[Path, str, str]:
    root_value = manifest.get("devkit_root")
    path_value = manifest.get("document_path")
    expected = manifest.get("document_sha256")
    if not isinstance(root_value, str) or not isinstance(path_value, str) or not isinstance(expected, str):
        raise ValueError("运行清单缺少目标文档信息")
    path_name, document = resolve_document(Path(root_value), path_value)
    return document, path_name, expected


def validate_result(result: dict[str, Any], manifest: dict[str, Any]) -> list[str]:
    errors: list[str] = []
    document, document_path, expected_digest = task_from_manifest(manifest)
    original = document.read_text(encoding="utf-8", errors="replace")
    body = strip_front_matter(original)
    title = first_heading(body, document.stem)
    if digest(document) != expected_digest:
        errors.append("目标文档在 prepare 后已变化")
    if result.get("schema_version") != SCHEMA_VERSION:
        errors.append("schema_version 必须为 1")
    if result.get("purpose") != "okf_api_description_result":
        errors.append("purpose 必须为 okf_api_description_result")
    if result.get("run_id") != manifest.get("run_id"):
        errors.append("run_id 与运行清单不一致")
    if result.get("document_path") != document_path:
        errors.append("document_path 与运行清单不一致")
    if result.get("document_sha256") != expected_digest:
        errors.append("document_sha256 与运行清单不一致")
    status = result.get("status")
    if status not in {"ready", "unresolved"}:
        errors.append("status 只能为 ready 或 unresolved")
    confidence = result.get("confidence")
    if not isinstance(confidence, (int, float)) or isinstance(confidence, bool) or not 0 <= confidence <= 1:
        errors.append("confidence 必须是 0 到 1 的数值")
    description = result.get("description")
    evidence = result.get("evidence")
    if not isinstance(description, str):
        errors.append("description 必须是字符串")
        description = ""
    if not isinstance(evidence, list):
        errors.append("evidence 必须是数组")
        evidence = []
    if status == "ready":
        if "\n" in description or "\r" in description:
            errors.append("ready description 必须是单段文本")
        if not MIN_DESCRIPTION_LENGTH <= len(description.strip()) <= MAX_DESCRIPTION_LENGTH:
            errors.append(f"ready description 长度必须在 {MIN_DESCRIPTION_LENGTH} 到 {MAX_DESCRIPTION_LENGTH} 之间")
        if description.strip() in {title, document.stem}:
            errors.append("ready description 不能仅重复标题或 API 名称")
        if any(marker in description for marker in ("#", "```", "[", "]", "|")):
            errors.append("ready description 不能包含 Markdown 结构")
        if not 1 <= len(evidence) <= 3:
            errors.append("ready result 必须提供一至三条 evidence")
        if "reason" in result and result["reason"]:
            errors.append("ready result 不应包含 unresolved reason")
    elif status == "unresolved":
        if description:
            errors.append("unresolved description 必须为空")
        if evidence:
            errors.append("unresolved evidence 必须为空")
        if not isinstance(result.get("reason"), str) or not result["reason"].strip():
            errors.append("unresolved result 必须说明 reason")
    for index, item in enumerate(evidence):
        prefix = f"evidence[{index}]"
        if not isinstance(item, dict) or set(item).difference({"excerpt"}):
            errors.append(f"{prefix} 只能包含 excerpt")
            continue
        excerpt = item.get("excerpt")
        if not isinstance(excerpt, str) or not excerpt.strip():
            errors.append(f"{prefix}.excerpt 必须是非空字符串")
        elif excerpt not in body:
            errors.append(f"{prefix}.excerpt 未逐字出现在目标 Markdown 正文")
    return errors


def write_validation(run_dir: Path, result_path: Path, errors: list[str]) -> None:
    write_json(
        run_dir / "validation.json",
        {
            "schema_version": SCHEMA_VERSION,
            "purpose": "okf_api_description_validation",
            "validated_at": now_utc(),
            "result": str(result_path.resolve()),
            "valid": not errors,
            "errors": errors,
        },
    )


def replace_description(original: str, description: str) -> str:
    start, end = front_matter_bounds(original)
    front_matter = original[start:end]
    body = original[end + len("---\n") :]
    lines = front_matter.splitlines(keepends=True)
    description_lines = [index for index, line in enumerate(lines) if re.match(r"^description:\s*", line)]
    encoded = json.dumps(description, ensure_ascii=False)
    replacement = f"description: {encoded}\n"
    if len(description_lines) > 1:
        raise ValueError("front matter 包含多个顶层 description，拒绝写入")
    if description_lines:
        index = description_lines[0]
        value = lines[index].split(":", 1)[1].strip()
        if value in {"|", ">", "|-", ">-"}:
            raise ValueError("不支持块形式 description，拒绝写入")
        lines[index] = replacement
    else:
        insert_at = next((index + 1 for index, line in enumerate(lines) if re.match(r"^resource:\s*", line)), None)
        if insert_at is None:
            insert_at = next((index + 1 for index, line in enumerate(lines) if re.match(r"^title:\s*", line)), len(lines))
        lines.insert(insert_at, replacement)
    return "---\n" + "".join(lines) + "---\n" + body


def cmd_prepare(args: argparse.Namespace) -> None:
    run_dir = Path(args.run_dir).resolve()
    if run_dir.exists() and any(run_dir.iterdir()):
        raise ValueError(f"run-dir 必须不存在或为空：{run_dir}")
    if RUN_ID_RE.fullmatch(args.run_id) is None:
        raise ValueError("run-id 只能使用字母、数字、点、下划线和连字符")
    root = Path(args.devkit_root).resolve()
    document_path, document = resolve_document(root, args.document)
    original = document.read_text(encoding="utf-8", errors="replace")
    front_matter_bounds(original)
    source_digest = digest(document)
    manifest = {
        "schema_version": SCHEMA_VERSION,
        "purpose": "okf_api_description_run",
        "run_id": args.run_id,
        "run_dir": str(run_dir),
        "created_at": now_utc(),
        "devkit_root": str(root),
        "document_path": document_path,
        "document_sha256": source_digest,
    }
    task = {
        "schema_version": SCHEMA_VERSION,
        "purpose": "okf_api_description_task",
        "run_id": args.run_id,
        "devkit_root": str(root),
        "document_path": document_path,
        "document_sha256": source_digest,
        "instructions": [
            "阅读指定 Markdown 的正文，并独立撰写中文 description。",
            "只把目标 Markdown 正文中的逐字摘录作为 evidence。",
            "没有可靠结论时输出 unresolved，不猜测。",
        ],
        "result_path": "description-result.json",
    }
    run_dir.mkdir(parents=True, exist_ok=True)
    write_json(run_dir / "run-manifest.json", manifest)
    write_json(run_dir / "task.json", task)
    print(json.dumps({"run_dir": str(run_dir), "task": str(run_dir / "task.json")}, ensure_ascii=False))


def cmd_validate(args: argparse.Namespace) -> None:
    run_dir = Path(args.run_dir).resolve()
    manifest = load_manifest(run_dir)
    result_path = Path(args.result)
    result = read_json(result_path)
    errors = validate_result(result, manifest)
    write_validation(run_dir, result_path, errors)
    if errors:
        raise ValueError("description 工件校验失败：" + "；".join(errors))
    print(json.dumps({"valid": True, "validation": str(run_dir / "validation.json")}, ensure_ascii=False))


def cmd_write(args: argparse.Namespace) -> None:
    run_dir = Path(args.run_dir).resolve()
    manifest = load_manifest(run_dir)
    result_path = Path(args.result)
    result = read_json(result_path)
    errors = validate_result(result, manifest)
    write_validation(run_dir, result_path, errors)
    if errors:
        raise ValueError("description 工件校验失败：" + "；".join(errors))
    if result["status"] == "unresolved" and not args.allow_unresolved:
        raise ValueError("unresolved 结果默认不能写入；仅在明确要求置空时使用 --allow-unresolved")
    document, document_path, expected_digest = task_from_manifest(manifest)
    if digest(document) != expected_digest:
        raise ValueError("目标文档在 prepare 后已变化")
    original = document.read_text(encoding="utf-8", errors="replace")
    updated = replace_description(original, result["description"])
    plan = {
        "schema_version": SCHEMA_VERSION,
        "purpose": "okf_api_description_write_plan",
        "document_path": document_path,
        "before_sha256": expected_digest,
        "after_sha256": sha256(updated.encode("utf-8")).hexdigest(),
        "description": result["description"],
        "will_write": bool(args.write),
    }
    if args.write:
        dry_run_plan = dict(plan)
        dry_run_plan["will_write"] = False
        plan_path = run_dir / "write-plan.json"
        if not plan_path.is_file() or read_json(plan_path) != dry_run_plan:
            raise ValueError("实际写入前必须完成同一输入的 dry-run write 计划")
    write_json(run_dir / "write-plan.json", plan)
    if args.write:
        document.write_text(updated, encoding="utf-8")
    print(json.dumps({"write_plan": str(run_dir / "write-plan.json"), "written": bool(args.write)}, ensure_ascii=False))


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    prepare = subparsers.add_parser("prepare", help="固定单个 API 的 description 任务")
    prepare.add_argument("--devkit-root", required=True)
    prepare.add_argument("--document", required=True)
    prepare.add_argument("--run-id", required=True)
    prepare.add_argument("--run-dir", required=True)
    prepare.set_defaults(handler=cmd_prepare)
    validate = subparsers.add_parser("validate", help="校验 Agent 生成的 description 工件")
    validate.add_argument("--run-dir", required=True)
    validate.add_argument("--result", required=True)
    validate.set_defaults(handler=cmd_validate)
    write = subparsers.add_parser("write", help="生成计划或写入 description")
    write.add_argument("--run-dir", required=True)
    write.add_argument("--result", required=True)
    write.add_argument("--allow-unresolved", action="store_true")
    write.add_argument("--write", action="store_true")
    write.set_defaults(handler=cmd_write)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    try:
        args.handler(args)
    except (FileNotFoundError, ValueError, json.JSONDecodeError) as error:
        print(f"错误：{error}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
