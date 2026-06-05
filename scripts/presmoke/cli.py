from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import time
from contextlib import contextmanager
from datetime import datetime, timezone
from pathlib import Path
from typing import Iterator, List

from .arch import detect_arch
from .case_runners import build_case_runner_cells_with_skips
from .discover import discover_examples, filter_examples
from .model import RunReport, RunResult, Suggestion
from .planner import build_cells_with_skips
from .readme_spec import parse_readme
from .report import print_console, write_json, write_markdown, write_suggestions
from .runner import run_cells_pipeline
from .scheduler import export_schedule_file, schedule_cells
from .suggest import dedupe_suggestions


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    jobs = resolve_jobs(args.jobs)
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
    cells = schedule_cells(
        cells,
        schedule=args.schedule,
        schedule_report=Path(args.schedule_report).resolve() if args.schedule_report else None,
        schedule_file=resolve_schedule_file(project_root, host_arch, modes, args.schedule, args.schedule_file),
        frontload_count=args.schedule_frontload_count,
        jobs=jobs,
    )
    if args.export_schedule:
        export_schedule_file(cells, Path(args.export_schedule).resolve())

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

    with presmoke_run_lock(project_root):
        run = run_cells_pipeline(
            cells,
            log_dir=log_dir,
            timeout=args.timeout,
            keep_artifacts=args.keep_artifacts,
            jobs=jobs,
            npu_slots=args.npu_slots,
        )
    results = run.results
    results.extend(skipped_results)
    results = sorted(results, key=lambda result: (result.example, result.mode))
    report = RunReport(host_arch, modes, started_at, now_iso(), results, suggestions, run.npu_stats)
    emit_reports(report, args, results_dir)
    return 1 if any(result.status == "FAIL" for result in results) else 0


def parse_args(argv: list[str] | None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="asc-devkit presmoke watcher v2")
    parser.add_argument("--examples-root")
    parser.add_argument("--runner-mode", choices=["readme", "case-runner"], default="readme")
    parser.add_argument("--arch")
    parser.add_argument("--modes", default="npu")
    parser.add_argument("--jobs", default="1")
    parser.add_argument("--npu-slots", type=int, default=1)
    parser.add_argument("--filter", action="append", default=[])
    parser.add_argument("--exclude", action="append", default=[])
    parser.add_argument("--timeout", type=int, default=120)
    parser.add_argument("--report-format", choices=["console", "json", "markdown", "all"], default="console")
    parser.add_argument("--results", default="presmoke_report")
    parser.add_argument("--log-dir")
    parser.add_argument("--keep-artifacts", action="store_true")
    parser.add_argument("--schedule", choices=["default", "build-desc", "frontload-build-desc", "npu-idle-min", "fixed"], default="default")
    parser.add_argument("--schedule-report")
    parser.add_argument("--schedule-file")
    parser.add_argument("--export-schedule")
    parser.add_argument("--schedule-frontload-count", type=int, default=1)
    parser.add_argument("--suggestions-only", action="store_true")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("-v", "--verbose", action="store_true")
    return parser.parse_args(argv)


def resolve_schedule_file(
    project_root: Path,
    host_arch: str,
    modes: List[str],
    schedule: str,
    explicit_schedule_file: str | None,
) -> Path | None:
    if explicit_schedule_file:
        return Path(explicit_schedule_file).resolve()
    if schedule != "fixed":
        return None
    mode = modes[0] if modes else "npu"
    return project_root / "scripts" / "presmoke" / "schedules" / f"{host_arch}_{mode}.txt"


def resolve_jobs(value: str | int, cpu_count: int | None = None) -> int:
    if isinstance(value, int):
        return max(value, 1)
    if value != "auto":
        return max(int(value), 1)

    cpus = cpu_count if cpu_count is not None else detect_cpu_count()
    if cpus >= 160:
        return 12
    if cpus >= 80:
        return 8
    if cpus >= 32:
        return 6
    return 4


def detect_cpu_count() -> int:
    try:
        result = subprocess.run(
            ["lscpu"],
            check=False,
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            text=True,
        )
    except (OSError, ValueError):
        return os.cpu_count() or 1

    for line in result.stdout.splitlines():
        if line.startswith("CPU(s):"):
            try:
                return max(int(line.split(":", 1)[1].strip()), 1)
            except ValueError:
                break
    return os.cpu_count() or 1


@contextmanager
def presmoke_run_lock(project_root: Path) -> Iterator[None]:
    lock_parent = Path(os.environ.get("PRESMOKE_LOCK_DIR", project_root / ".presmoke_locks"))
    lock_dir = lock_parent / "presmoke_run.lock"
    lock_parent.mkdir(parents=True, exist_ok=True)

    while True:
        try:
            lock_dir.mkdir()
            break
        except FileExistsError:
            pid_file = lock_dir / "pid"
            try:
                pid = int(pid_file.read_text(encoding="utf-8").strip())
            except (OSError, ValueError):
                pid = 0
            if pid and process_exists(pid):
                print(f"Waiting for existing presmoke run pid={pid}", flush=True)
                time.sleep(5)
                continue
            shutil.rmtree(lock_dir, ignore_errors=True)

    (lock_dir / "pid").write_text(str(os.getpid()), encoding="utf-8")
    try:
        yield
    finally:
        shutil.rmtree(lock_dir, ignore_errors=True)


def process_exists(pid: int) -> bool:
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    except PermissionError:
        return True
    return True


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
