from __future__ import annotations

import argparse
import os
from datetime import datetime, timezone
from pathlib import Path
from typing import List

from .arch import detect_arch
from .case_runners import build_case_runner_cells_with_skips
from .discover import discover_examples, filter_examples
from .model import RunReport, RunResult, Suggestion
from .planner import build_cells_with_skips
from .readme_spec import parse_readme
from .report import print_console, write_json, write_markdown, write_suggestions
from .runner import run_cell
from .pool import NpuSlotPool, run_all
from .suggest import dedupe_suggestions


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    project_root = Path(os.environ.get("PRESMOKE_PROJECT_ROOT", Path.cwd())).resolve()
    examples_root = Path(args.examples_root).resolve() if args.examples_root else project_root / "examples"
    host_arch = detect_arch(args.arch)
    if not host_arch:
        print("Error: cannot detect host arch; pass --arch or PRESMOKE_ARCH", flush=True)
        return 2

    results_dir = Path(args.results).resolve()
    log_dir = Path(args.log_dir).resolve() if args.log_dir else results_dir / "logs"
    started_at = now_iso()

    suggestions: List[Suggestion] = []
    modes = parse_modes(args.modes)
    skipped_results: List[RunResult] = []

    if args.runner_mode == "case-runner":
        cells, runner_suggestions, skipped_results = build_case_runner_cells_with_skips(project_root, host_arch, modes, args.filter, args.exclude)
        suggestions.extend(runner_suggestions)
    else:
        examples = discover_examples(examples_root)
        examples = filter_examples(examples, args.filter, args.exclude)
        specs = [parse_readme(path, examples_root) for path in examples]
        for spec in specs:
            suggestions.extend(spec.suggestions)
        cells, planner_suggestions, skipped_results = build_cells_with_skips(specs, host_arch, modes)
        suggestions.extend(planner_suggestions)
    suggestions = dedupe_suggestions(suggestions)

    if args.suggestions_only:
        report = RunReport(host_arch, modes, started_at, now_iso(), [], suggestions)
        emit_reports(report, args, results_dir)
        return 0

    if args.dry_run:
        dry_results = [
            RunResult(cell.example.rel_path, cell.arch, cell.mode, "SKIP", "dry-run", duration_s=0, source=cell.example.source)
            for cell in cells
        ]
        report = RunReport(host_arch, modes, started_at, now_iso(), dry_results + skipped_results, suggestions)
        emit_reports(report, args, results_dir)
        return 0

    if args.baseline or args.changed_only:
        print("Error: baseline and changed-only are Phase 2/3 features and are not implemented in this Phase 1 build")
        return 2

    npu_pool = NpuSlotPool(args.npu_slots)

    def _run(cell):
        return run_cell(
            cell,
            log_dir=log_dir,
            timeout=args.timeout,
            keep_artifacts=args.keep_artifacts,
            npu_gate=npu_pool.gate,
        )

    results = run_all(cells, jobs=args.jobs, npu_slots=args.npu_slots, fn=_run)
    results.extend(skipped_results)
    results = sorted(results, key=lambda result: (result.example, result.mode))
    report = RunReport(host_arch, modes, started_at, now_iso(), results, suggestions)
    emit_reports(report, args, results_dir)
    return 1 if any(result.status == "FAIL" for result in results) else 0


def parse_args(argv: list[str] | None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="asc-devkit presmoke watcher v2")
    parser.add_argument("--examples-root")
    parser.add_argument("--runner-mode", choices=["readme", "case-runner"], default="readme")
    parser.add_argument("--arch")
    parser.add_argument("--modes", default="npu")
    parser.add_argument("--jobs", type=int, default=1)
    parser.add_argument("--npu-slots", type=int, default=1)
    parser.add_argument("--filter", action="append", default=[])
    parser.add_argument("--exclude", action="append", default=[])
    parser.add_argument("--timeout", type=int, default=900)
    parser.add_argument("--report-format", choices=["console", "json", "markdown", "all"], default="console")
    parser.add_argument("--results", default="presmoke_results")
    parser.add_argument("--log-dir")
    parser.add_argument("--keep-artifacts", action="store_true")
    parser.add_argument("--suggestions-only", action="store_true")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--preflight", action="store_true")
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument("--baseline")
    parser.add_argument("--update-baseline", action="store_true")
    parser.add_argument("--allowlist")
    parser.add_argument("--changed-only", action="store_true")
    parser.add_argument("--since")
    return parser.parse_args(argv)


def parse_modes(value: str) -> List[str]:
    modes = [item.strip() for item in value.split(",") if item.strip()]
    return modes or ["npu"]


def emit_reports(report: RunReport, args: argparse.Namespace, results_dir: Path) -> None:
    results_dir.mkdir(parents=True, exist_ok=True)
    fmt = args.report_format
    if fmt in ("console", "all"):
        print_console(report)
    if fmt in ("json", "all"):
        write_json(report, results_dir / "report.json")
    if fmt in ("markdown", "all"):
        write_markdown(report, results_dir / "report.md")
    if report.suggestions:
        write_suggestions(report.suggestions, results_dir / "suggestions.md")


def now_iso() -> str:
    return datetime.now(timezone.utc).astimezone().isoformat(timespec="seconds")
