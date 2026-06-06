from __future__ import annotations

import argparse
import csv
import datetime
import json
from pathlib import Path
from typing import Any


def read_json(path: Path) -> dict[str, Any] | None:
    if not path.exists():
        return None
    with path.open(encoding="utf-8") as handle:
        return json.load(handle)


def read_meta(path: Path) -> dict[str, str]:
    meta: dict[str, str] = {}
    if path.exists():
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            if "=" in line:
                key, value = line.split("=", 1)
                meta[key] = value
    return meta


def timeout_examples(report_path: Path) -> list[str]:
    data = read_json(report_path)
    if not data:
        return []
    return [
        result.get("example", "")
        for result in data.get("results", [])
        if result.get("status") == "FAIL" and result.get("reason") == "timeout"
    ]


def retry_status(report_path: Path) -> str:
    data = read_json(report_path)
    if not data:
        return ""
    results = data.get("results", [])
    return results[0].get("status", "") if results else ""


def print_summary(root: Path) -> None:
    primary = load_primary_run(root)
    effective_rc = (root / "effective_rc.txt").read_text(encoding="utf-8").strip() if (root / "effective_rc.txt").exists() else "1"
    print("========================================")
    print("Presmoke Summary:")
    print(f"  Output: {root}")
    print(f"  Final Report: {root / 'FINAL_REPORT.md'}")
    print(f"  Timings: {root / 'ALL_CASE_TIMINGS.tsv'}")
    print(f"  Failures: {root / 'FAILURES.tsv'}")
    print(f"  Effective RC: {effective_rc}")
    if not primary:
        print("  Primary run: missing")
        print("========================================")
        return

    name, _, meta, data = primary
    summary = data.get("summary", {})
    results = data.get("results", [])
    npu_stats = data.get("npu_stats") or {}
    elapsed = meta.get("elapsed_sec", "")
    print(f"  Primary run: {name}")
    print(f"  Arch/Card: {meta.get('arch', '')}/{meta.get('card', '')}")
    print(f"  Mode/Schedule: {meta.get('modes', '')}/{meta.get('schedule', '')}")
    print(f"  Jobs/NPU slots: {meta.get('jobs', '')}/{meta.get('npu_slots', '')}")
    print(f"  Elapsed: {elapsed}s")
    print(
        "  Cases: "
        f"total={len(results)} "
        f"pass={summary.get('PASS', 0)} "
        f"fail={summary.get('FAIL', 0)} "
        f"skip={summary.get('SKIP', 0)}"
    )
    if npu_stats:
        metric = "run_queue" if npu_stats.get("queue_model") == "pipeline-cpu" else "npu"
        print(
            f"  {metric}: "
            f"busy={float(npu_stats.get('busy_s') or 0):.3f}s "
            f"idle={float(npu_stats.get('idle_s') or 0):.3f}s "
            f"util={float(npu_stats.get('utilization') or 0):.3f}%"
        )
    failures = [result for result in results if result.get("status") == "FAIL"]
    if failures:
        print("  Failed cases:")
        for result in failures[:20]:
            print(
                "    "
                f"{result.get('example', '')} "
                f"reason={result.get('reason', '')} "
                f"rc={result.get('rc', '')} "
                f"duration={float(result.get('duration_s') or 0):.1f}s"
            )
        if len(failures) > 20:
            print(f"    ... {len(failures) - 20} more")
    print("========================================")


def load_primary_run(root: Path) -> tuple[str, Path, dict[str, str], dict[str, Any]] | None:
    for report in sorted(root.glob("*/results/report.json")):
        run_dir = report.parents[1]
        if not run_dir.name.startswith("full_card"):
            continue
        data = read_json(report)
        if data:
            return run_dir.name, run_dir, read_meta(run_dir / "meta.txt"), data
    return None


def write_final_report(root: Path) -> None:
    runs: list[tuple[str, Path, dict[str, str], dict[str, Any]]] = []
    for report in sorted(root.glob("*/results/report.json")):
        run_dir = report.parents[1]
        data = read_json(report)
        if data:
            runs.append((run_dir.name, run_dir, read_meta(run_dir / "meta.txt"), data))

    primary = next((item for item in runs if item[0].startswith("full_card")), None)
    retry_by_example: dict[str, list[tuple[str, dict[str, str], dict[str, Any]]]] = {}
    for name, _, meta, data in runs:
        if not name.startswith("retry_"):
            continue
        results = data.get("results", [])
        if results:
            retry_by_example.setdefault(results[0].get("example", ""), []).append((name, meta, results[0]))

    timings_path = root / "ALL_CASE_TIMINGS.tsv"
    failures_path = root / "FAILURES.tsv"
    effective_rc_path = root / "effective_rc.txt"

    with timings_path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle, delimiter="\t")
        writer.writerow(
            [
                "run",
                "example",
                "arch",
                "mode",
                "status",
                "rc",
                "duration_s",
                "npu_wait_s",
                "reason",
                "failing_step",
                "steps",
                "commands",
                "log_file",
                "stage_log_files",
            ]
        )
        if primary:
            name, _, _, data = primary
            for result in data.get("results", []):
                steps = result.get("steps", [])
                npu_wait = sum(float(step.get("wait_s") or 0) for step in steps)
                step_text = "; ".join(format_step(step) for step in steps)
                commands = " && ".join(str(step.get("command", "")).replace("\n", " ") for step in steps)
                writer.writerow(
                    [
                        name,
                        result.get("example", ""),
                        result.get("arch", ""),
                        result.get("mode", ""),
                        result.get("status", ""),
                        result.get("rc", ""),
                        f"{float(result.get('duration_s') or 0):.3f}",
                        f"{npu_wait:.3f}",
                        result.get("reason", ""),
                        result.get("failing_step", ""),
                        step_text,
                        commands,
                        result.get("log_file", ""),
                        ";".join(result.get("stage_log_files", [])),
                    ]
                )

    with failures_path.open("w", encoding="utf-8", newline="") as handle:
        writer = csv.writer(handle, delimiter="\t")
        writer.writerow(["example", "first_status", "first_reason", "first_rc", "first_duration_s", "first_failing_step", "retry_results"])
        if primary:
            _, _, _, data = primary
            for result in data.get("results", []):
                if result.get("status") != "FAIL":
                    continue
                retries = [
                    f"{retry_name}:card={meta.get('card','')}:status={retry_result.get('status','')}:"
                    f"reason={retry_result.get('reason','')}:rc={retry_result.get('rc','')}:"
                    f"duration={float(retry_result.get('duration_s') or 0):.1f}"
                    for retry_name, meta, retry_result in retry_by_example.get(result.get("example", ""), [])
                ]
                writer.writerow(
                    [
                        result.get("example", ""),
                        result.get("status", ""),
                        result.get("reason", ""),
                        result.get("rc", ""),
                        f"{float(result.get('duration_s') or 0):.3f}",
                        result.get("failing_step", ""),
                        " | ".join(retries),
                    ]
                )

    effective_failures: list[dict[str, Any]] = []
    missing_primary = primary is None
    if primary:
        _, _, _, data = primary
        for result in data.get("results", []):
            if result.get("status") != "FAIL":
                continue
            retries = retry_by_example.get(result.get("example", ""), [])
            retry_passed = any(retry_result.get("status") == "PASS" for _, _, retry_result in retries)
            if result.get("reason") == "timeout" and retry_passed:
                continue
            effective_failures.append(result)
    effective_rc_path.write_text("1\n" if missing_primary or effective_failures else "0\n", encoding="utf-8")

    (root / "FINAL_REPORT.md").write_text(
        "\n".join(render_markdown(root, primary, retry_by_example, effective_failures, timings_path, failures_path)) + "\n",
        encoding="utf-8",
    )


def format_step(step: dict[str, Any]) -> str:
    text = f"{step.get('kind')}:{float(step.get('duration_s') or 0):.3f}s"
    if step.get("npu_slot"):
        text += "/npu"
    if step.get("wait_s"):
        text += f"/wait={float(step.get('wait_s') or 0):.3f}s"
    if step.get("rc") not in (0, None):
        text += f"/rc={step.get('rc')}"
    return text


def render_markdown(
    root: Path,
    primary: tuple[str, Path, dict[str, str], dict[str, Any]] | None,
    retry_by_example: dict[str, list[tuple[str, dict[str, str], dict[str, Any]]]],
    effective_failures: list[dict[str, Any]],
    timings_path: Path,
    failures_path: Path,
) -> list[str]:
    lines = ["# Presmoke Report", ""]
    lines.append(f"- root: `{root}`")
    lines.append(f"- generated_at: `{datetime.datetime.now().astimezone().isoformat(timespec='seconds')}`")
    if not primary:
        lines.extend(["", "Primary full run report was not found.", "", "## Effective Status", "", "`FAIL`: primary report missing."])
        return lines

    name, run_dir, meta, data = primary
    summary = data.get("summary", {})
    npu_stats = data.get("npu_stats") or {}
    parallel_config = data.get("parallel_config") or {}
    results = data.get("results", [])
    lines.extend(
        [
            "",
            "## Primary Full Run",
            "",
            f"- run: `{name}`",
            f"- card: `{meta.get('card', '')}`",
            f"- arch: `{meta.get('arch', '')}`",
            f"- schedule: `{meta.get('schedule', '')}`",
            f"- jobs/npu_slots: `{meta.get('jobs', '')}/{meta.get('npu_slots', '')}`",
            f"- cpu_run_slots/make_jobs: `{meta.get('cpu_run_slots', '')}/{meta.get('make_jobs', '')}`",
            f"- resolved jobs/npu_slots: `{parallel_config.get('jobs', '')}/{parallel_config.get('npu_slots', '')}`",
            f"- resolved cpu_run_slots/make_jobs: `{parallel_config.get('cpu_run_slots', '')}/{parallel_config.get('make_jobs', '')}`",
            f"- resolved cpu_run_timeout: `{parallel_config.get('cpu_run_timeout', '')}`",
            f"- timeout: `{meta.get('timeout', '')}`",
            f"- cpu_run_timeout: `{meta.get('cpu_run_timeout', '')}`",
            f"- elapsed_sec: `{meta.get('elapsed_sec', '')}`",
            f"- planned_results: `{len(results)}`",
            f"- pass/fail/skip: `{summary.get('PASS', 0)}/{summary.get('FAIL', 0)}/{summary.get('SKIP', 0)}`",
        ]
    )
    if npu_stats:
        metric_prefix = "run_queue" if npu_stats.get("queue_model") == "pipeline-cpu" else "npu"
        lines.extend(
            [
                f"- {metric_prefix}_busy_s: `{float(npu_stats.get('busy_s') or 0):.3f}`",
                f"- {metric_prefix}_idle_s: `{float(npu_stats.get('idle_s') or 0):.3f}`",
                f"- {metric_prefix}_utilization: `{float(npu_stats.get('utilization') or 0):.3f}`",
            ]
        )

    append_failures(lines, results, retry_by_example)
    append_skips(lines, results)
    if effective_failures:
        lines.extend(["", "## Effective Status", "", f"`FAIL`: `{len(effective_failures)}` unrecovered failed cases."])
    else:
        lines.extend(["", "## Effective Status", "", "`PASS`: no unrecovered failed cases."])
    lines.extend(
        [
            "",
            "## Artifacts",
            "",
            f"- all case timings: `{timings_path}`",
            f"- failures tsv: `{failures_path}`",
            f"- primary JSON: `{run_dir / 'results' / 'report.json'}`",
            f"- primary Markdown: `{run_dir / 'results' / 'report.md'}`",
            f"- per-case logs: `{run_dir / 'results' / 'logs'}`",
            f"- per-stage logs: `{run_dir / 'results' / 'logs' / 'stages'}`",
        ]
    )
    return lines


def append_failures(
    lines: list[str],
    results: list[dict[str, Any]],
    retry_by_example: dict[str, list[tuple[str, dict[str, str], dict[str, Any]]]],
) -> None:
    failures = [result for result in results if result.get("status") == "FAIL"]
    lines.extend(["", "## Failures", ""])
    if not failures:
        lines.append("No failures.")
        return
    lines.append("| example | status | reason | rc | duration_s | failing_step | retry |")
    lines.append("|---|---:|---|---:|---:|---|---|")
    for result in failures:
        retry_text = [
            f"{retry_name} card={retry_meta.get('card','')} {retry_result.get('status','')}"
            f" {retry_result.get('reason','')} rc={retry_result.get('rc','')}"
            for retry_name, retry_meta, retry_result in retry_by_example.get(result.get("example", ""), [])
        ]
        failing_step = str(result.get("failing_step", "")).replace("|", "\\|")
        lines.append(
            f"| {result.get('example','')} | {result.get('status','')} | {result.get('reason','')} | "
            f"{result.get('rc','')} | {float(result.get('duration_s') or 0):.1f} | "
            f"{failing_step} | {'<br>'.join(retry_text)} |"
        )


def append_skips(lines: list[str], results: list[dict[str, Any]]) -> None:
    skips = [result for result in results if result.get("status") == "SKIP"]
    lines.extend(["", "## Skips", ""])
    dry_run_skips = [result for result in skips if result.get("reason") == "dry-run"]
    real_skips = [result for result in skips if result.get("reason") != "dry-run"]
    if dry_run_skips:
        lines.append(f"- dry-run planned cases: `{len(dry_run_skips)}`; details are in `ALL_CASE_TIMINGS.tsv`.")
    if real_skips:
        lines.append("")
        lines.append("| example | reason |")
        lines.append("|---|---|")
        for result in real_skips:
            lines.append(f"| {result.get('example','')} | {result.get('reason','')} |")
    elif not dry_run_skips:
        lines.append("No skips.")


def main() -> int:
    parser = argparse.ArgumentParser(description="Presmoke orchestration report helper")
    subparsers = parser.add_subparsers(dest="command", required=True)
    timeout_parser = subparsers.add_parser("timeout-examples")
    timeout_parser.add_argument("report", type=Path)
    status_parser = subparsers.add_parser("retry-status")
    status_parser.add_argument("report", type=Path)
    report_parser = subparsers.add_parser("write-final-report")
    report_parser.add_argument("root", type=Path)
    summary_parser = subparsers.add_parser("print-summary")
    summary_parser.add_argument("root", type=Path)
    args = parser.parse_args()

    if args.command == "timeout-examples":
        for example in timeout_examples(args.report):
            print(example)
    elif args.command == "retry-status":
        print(retry_status(args.report))
    elif args.command == "write-final-report":
        write_final_report(args.root)
    elif args.command == "print-summary":
        print_summary(args.root)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
