from __future__ import annotations

import json
from dataclasses import asdict
from pathlib import Path
from typing import Iterable, List

from .model import RunReport, RunResult, Suggestion


def write_json(report: RunReport, path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    payload = asdict(report)
    payload["summary"] = summarize(report.results)
    if report.npu_stats:
        payload["npu_stats"]["utilization"] = report.npu_stats.utilization
    path.write_text(json.dumps(payload, ensure_ascii=False, indent=2), encoding="utf-8")


def write_markdown(report: RunReport, path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    summary = summarize(report.results)
    lines = [
        "# Presmoke Report",
        "",
        f"- host_arch: `{report.host_arch}`",
        f"- modes: `{','.join(report.modes)}`",
        f"- started_at: `{report.started_at}`",
        f"- finished_at: `{report.finished_at}`",
        f"- pass/fail/skip: `{summary['PASS']}/{summary['FAIL']}/{summary['SKIP']}`",
    ]
    if report.parallel_config:
        lines.extend(
            [
                f"- resolved_jobs: `{report.parallel_config.get('jobs', '')}`",
                f"- resolved_npu_slots: `{report.parallel_config.get('npu_slots', '')}`",
                f"- resolved_cpu_run_slots: `{report.parallel_config.get('cpu_run_slots', '')}`",
                f"- resolved_make_jobs: `{report.parallel_config.get('make_jobs', '')}`",
            ]
        )
    if report.npu_stats:
        metric_prefix = "cpu_run_queue" if report.npu_stats.queue_model == "pipeline-cpu" else "npu"
        lines.extend(
            [
                f"- {metric_prefix}_slots: `{report.npu_stats.slots}`",
                f"- {metric_prefix}_model: `{report.npu_stats.queue_model}`",
                f"- {metric_prefix}_busy_s: `{report.npu_stats.busy_s:.3f}`",
                f"- {metric_prefix}_idle_s: `{report.npu_stats.idle_s:.3f}`",
                f"- {metric_prefix}_utilization: `{report.npu_stats.utilization:.3f}`",
                f"- {metric_prefix}_max_slot_idle_s: `{report.npu_stats.max_slot_idle_s:.3f}`",
            ]
        )
    lines.extend(
        [
            "",
            "| example | arch | mode | status | rc | duration_s | npu_wait_s | reason | steps | log | stage_logs |",
            "|---|---|---|---:|---:|---:|---:|---|---|---|---|",
        ]
    )
    for result in sorted(report.results, key=lambda r: (r.example, r.mode)):
        npu_wait = sum(step.wait_s for step in result.steps)
        steps = "<br>".join(
            f"{_md_escape(step.kind)}:{step.duration_s:.1f}s"
            f"{'/npu' if step.npu_slot else ''}"
            f"{'/wait=' + format(step.wait_s, '.1f') + 's' if step.wait_s else ''}"
            f"{'/rc=' + str(step.rc) if step.rc not in (0, None) else ''}"
            for step in result.steps
        )
        lines.append(
            f"| {result.example} | {result.arch} | {result.mode} | {result.status} | "
            f"{'' if result.rc is None else result.rc} | {result.duration_s:.1f} | {npu_wait:.1f} | "
            f"{result.reason} | {steps} | {result.log_file} | {'<br>'.join(result.stage_log_files)} |"
        )
    if report.suggestions:
        lines.extend(["", "## Suggestions", "", "| example | severity | category | message | hint |", "|---|---|---|---|---|"])
        for item in report.suggestions:
            lines.append(f"| {item.example} | {item.severity} | {item.category} | {item.message} | {item.hint} |")
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_suggestions(suggestions: Iterable[Suggestion], path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    lines = ["# Presmoke Suggestions", "", "| example | severity | category | message | hint |", "|---|---|---|---|---|"]
    for item in suggestions:
        lines.append(f"| {item.example} | {item.severity} | {item.category} | {item.message} | {item.hint} |")
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def print_console(report: RunReport) -> None:
    summary = summarize(report.results)
    print(f"Presmoke v2: arch={report.host_arch} modes={','.join(report.modes)}")
    print(f"PASS={summary['PASS']} FAIL={summary['FAIL']} SKIP={summary['SKIP']}")
    if report.parallel_config:
        print(
            "PARALLEL "
            f"jobs={report.parallel_config.get('jobs', '')} "
            f"npu_slots={report.parallel_config.get('npu_slots', '')} "
            f"cpu_run_slots={report.parallel_config.get('cpu_run_slots', '')} "
            f"make_jobs={report.parallel_config.get('make_jobs', '')}"
        )
    if report.npu_stats:
        metric_label = "CPU_RUN_QUEUE" if report.npu_stats.queue_model == "pipeline-cpu" else "NPU"
        print(
            f"{metric_label} "
            f"slots={report.npu_stats.slots} "
            f"model={report.npu_stats.queue_model} "
            f"busy_s={report.npu_stats.busy_s:.3f} "
            f"idle_s={report.npu_stats.idle_s:.3f} "
            f"utilization={report.npu_stats.utilization:.3f}"
        )
    failures = [r for r in report.results if r.status == "FAIL"]
    if failures:
        print("Failed cells:")
        for result in sorted(failures, key=lambda r: (r.example, r.mode)):
            print(f"  {result.example} [{result.mode}] rc={result.rc} reason={result.reason} step={result.failing_step}")
    if report.suggestions:
        print(f"Suggestions: {len(report.suggestions)} written to suggestions.md")


def summarize(results: Iterable[RunResult]) -> dict[str, int]:
    summary = {"PASS": 0, "FAIL": 0, "SKIP": 0}
    for result in results:
        summary[result.status] = summary.get(result.status, 0) + 1
    return summary


def _md_escape(value: str) -> str:
    return value.replace("|", "\\|")
