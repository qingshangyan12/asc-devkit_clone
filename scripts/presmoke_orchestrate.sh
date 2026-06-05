#!/usr/bin/env bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="${PROJECT_ROOT:-$(cd "${SCRIPT_DIR}/.." && pwd)}"
ASCEND_HOME_DIR="${ASCEND_HOME_DIR:-/usr/local/Ascend/ascend-toolkit/latest}"
ARCH="${ARCH:-dav-2201}"
MODES="${MODES:-npu}"
RUNNER_MODE="${RUNNER_MODE:-case-runner}"
SCHEDULE="${SCHEDULE:-fixed}"
SCHEDULE_FILE="${SCHEDULE_FILE:-}"
JOBS="${JOBS:-auto}"
NPU_SLOTS="${NPU_SLOTS:-1}"
TIMEOUT="${TIMEOUT:-120}"
PRIMARY_CARD="${PRIMARY_CARD:-0}"
RETRY_CARDS="${RETRY_CARDS:-}"
REPORT_FORMAT="${REPORT_FORMAT:-all}"
OUT_ROOT="${OUT_ROOT:-${PROJECT_ROOT}/presmoke_results/orchestrate_${ARCH}_$(date +%Y%m%d_%H%M%S)}"
LOCK_DIR="${LOCK_DIR:-${PROJECT_ROOT}/.presmoke_locks/presmoke_orchestrate.lock}"

mkdir -p "$OUT_ROOT"
STATUS_FILE="$OUT_ROOT/status.txt"

date_iso() {
    date '+%Y-%m-%dT%H:%M:%S%z'
}

log() {
    printf '[%s] %s\n' "$(date_iso)" "$*" | tee -a "$STATUS_FILE"
}

cleanup_lock() {
    rm -rf "$LOCK_DIR"
}

acquire_lock() {
    mkdir -p "$(dirname "$LOCK_DIR")"
    if mkdir "$LOCK_DIR" 2>/dev/null; then
        echo "$$" > "$LOCK_DIR/pid"
        trap cleanup_lock EXIT
        return
    fi

    local pid
    pid="$(cat "$LOCK_DIR/pid" 2>/dev/null || true)"
    if [[ -n "$pid" ]] && kill -0 "$pid" 2>/dev/null; then
        echo "Error: presmoke orchestrate is already running, pid=$pid lock=$LOCK_DIR" >&2
        exit 2
    fi
    rm -rf "$LOCK_DIR"
    mkdir "$LOCK_DIR"
    echo "$$" > "$LOCK_DIR/pid"
    trap cleanup_lock EXIT
}

source_cann() {
    set +u
    if [[ -f "${ASCEND_HOME_DIR}/set_env.sh" ]]; then
        # shellcheck disable=SC1090
        source "${ASCEND_HOME_DIR}/set_env.sh" 2>/dev/null || true
    elif [[ -f "/usr/local/Ascend/cann/set_env.sh" ]]; then
        # shellcheck disable=SC1091
        source "/usr/local/Ascend/cann/set_env.sh" 2>/dev/null || true
    fi
    set -u
}

run_presmoke() {
    local name="$1"
    local card="$2"
    shift 2
    local run_dir="$OUT_ROOT/$name"
    mkdir -p "$run_dir"
    log "run_start name=$name card=$card arch=$ARCH jobs=$JOBS npu_slots=$NPU_SLOTS timeout=$TIMEOUT schedule=$SCHEDULE args=$*"
    {
        echo "name=$name"
        echo "started_at=$(date_iso)"
        echo "project_root=$PROJECT_ROOT"
        echo "git=$(cd "$PROJECT_ROOT" && git rev-parse --short HEAD 2>/dev/null || true)"
        echo "arch=$ARCH"
        echo "modes=$MODES"
        echo "runner_mode=$RUNNER_MODE"
        echo "schedule=$SCHEDULE"
        echo "schedule_file=$SCHEDULE_FILE"
        echo "jobs=$JOBS"
        echo "npu_slots=$NPU_SLOTS"
        echo "timeout=$TIMEOUT"
        echo "card=$card"
        echo "extra_args=$*"
        source_cann
        echo "npu_smi_before_begin"
        npu-smi info 2>/dev/null || true
        echo "npu_smi_before_end"
    } > "$run_dir/meta.txt"

    set +e
    local start end rc
    start="$(date +%s)"
    (
        cd "$PROJECT_ROOT"
        export ASCEND_HOME_DIR
        export ASCEND_RT_VISIBLE_DEVICES="$card"
        export ASCEND_VISIBLE_DEVICES="$card"
        export ASCEND_DEVICE_ID=0
        export NPU_DEVICE_ID=0
        args=(
            --runner-mode "$RUNNER_MODE"
            --arch "$ARCH"
            --modes "$MODES"
            --jobs "$JOBS"
            --npu-slots "$NPU_SLOTS"
            --timeout "$TIMEOUT"
            --schedule "$SCHEDULE"
            --report-format "$REPORT_FORMAT"
            --results "$run_dir/results"
        )
        if [[ -n "$SCHEDULE_FILE" ]]; then
            args+=(--schedule-file "$SCHEDULE_FILE")
        fi
        bash scripts/run_presmoke2.sh "${args[@]}" "$@"
    ) > "$run_dir/stdout.log" 2> "$run_dir/stderr.log"
    rc=$?
    end="$(date +%s)"
    set -e

    {
        echo "finished_at=$(date_iso)"
        echo "rc=$rc"
        echo "elapsed_sec=$((end - start))"
        source_cann
        echo "npu_smi_after_begin"
        npu-smi info 2>/dev/null || true
        echo "npu_smi_after_end"
    } >> "$run_dir/meta.txt"
    log "run_done name=$name card=$card rc=$rc elapsed_sec=$((end - start))"
    return 0
}

timeout_examples() {
    local report="$1"
    [[ -f "$report" ]] || return
    python3 - "$report" <<'PY'
import json
import sys

data = json.load(open(sys.argv[1], encoding="utf-8"))
for result in data.get("results", []):
    if result.get("status") == "FAIL" and result.get("reason") == "timeout":
        print(result.get("example", ""))
PY
}

retry_timeouts() {
    local full_report="$OUT_ROOT/full_card${PRIMARY_CARD}/results/report.json"
    [[ -f "$full_report" ]] || return
    local examples=()
    while IFS= read -r example; do
        examples+=("$example")
    done < <(timeout_examples "$full_report")
    if [[ "${#examples[@]}" -eq 0 ]]; then
        log "retry_skip no_timeouts"
        return
    fi
    if [[ -z "$RETRY_CARDS" ]]; then
        log "retry_skip retry_cards_empty count=${#examples[@]}"
        return
    fi

    log "retry_timeouts count=${#examples[@]}"
    local example card safe_name retry_report status
    for example in "${examples[@]}"; do
        [[ -n "$example" ]] || continue
        safe_name="${example//\//__}"
        for card in $RETRY_CARDS; do
            log "retry_start example=$example card=$card"
            run_presmoke "retry_${safe_name}_card${card}" "$card" --filter "$example"
            retry_report="$OUT_ROOT/retry_${safe_name}_card${card}/results/report.json"
            status="$(python3 - "$retry_report" <<'PY'
import json
import sys

data = json.load(open(sys.argv[1], encoding="utf-8"))
results = data.get("results", [])
print(results[0].get("status", "") if results else "")
PY
)"
            log "retry_done example=$example card=$card status=$status"
            [[ "$status" == "PASS" ]] && break
        done
    done
}

write_final_report() {
    python3 - "$OUT_ROOT" <<'PY'
import csv
import datetime
import json
import os
import pathlib
import sys

root = pathlib.Path(sys.argv[1])

def read_json(path):
    if not path.exists():
        return None
    with path.open(encoding="utf-8") as f:
        return json.load(f)

def read_meta(path):
    meta = {}
    if path.exists():
        for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
            if "=" in line:
                key, value = line.split("=", 1)
                meta[key] = value
    return meta

runs = []
for report in sorted(root.glob("*/results/report.json")):
    run_dir = report.parents[1]
    data = read_json(report)
    if data:
        runs.append((run_dir.name, run_dir, read_meta(run_dir / "meta.txt"), data))

primary = next((item for item in runs if item[0].startswith("full_card")), None)
retry_by_example = {}
for name, _, meta, data in runs:
    if not name.startswith("retry_"):
        continue
    results = data.get("results", [])
    if results:
        retry_by_example.setdefault(results[0].get("example", ""), []).append((name, meta, results[0]))

timings_path = root / "ALL_CASE_TIMINGS.tsv"
failures_path = root / "FAILURES.tsv"
effective_rc_path = root / "effective_rc.txt"
with timings_path.open("w", encoding="utf-8", newline="") as f:
    writer = csv.writer(f, delimiter="\t")
    writer.writerow(["run", "example", "arch", "mode", "status", "rc", "duration_s", "npu_wait_s", "reason", "failing_step", "steps", "commands", "log_file"])
    if primary:
        name, _, _, data = primary
        for result in data.get("results", []):
            steps = result.get("steps", [])
            npu_wait = sum(float(step.get("wait_s") or 0) for step in steps)
            step_text = "; ".join(
                f"{step.get('kind')}:{float(step.get('duration_s') or 0):.3f}s"
                f"{'/npu' if step.get('npu_slot') else ''}"
                f"{'/wait=' + format(float(step.get('wait_s') or 0), '.3f') + 's' if step.get('wait_s') else ''}"
                f"{'/rc=' + str(step.get('rc')) if step.get('rc') not in (0, None) else ''}"
                for step in steps
            )
            commands = " && ".join(str(step.get("command", "")).replace("\n", " ") for step in steps)
            writer.writerow([
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
            ])

with failures_path.open("w", encoding="utf-8", newline="") as f:
    writer = csv.writer(f, delimiter="\t")
    writer.writerow(["example", "first_status", "first_reason", "first_rc", "first_duration_s", "first_failing_step", "retry_results"])
    if primary:
        _, _, _, data = primary
        for result in data.get("results", []):
            if result.get("status") != "FAIL":
                continue
            retries = []
            for retry_name, meta, retry_result in retry_by_example.get(result.get("example", ""), []):
                retries.append(
                    f"{retry_name}:card={meta.get('card','')}:status={retry_result.get('status','')}:"
                    f"reason={retry_result.get('reason','')}:rc={retry_result.get('rc','')}:"
                    f"duration={float(retry_result.get('duration_s') or 0):.1f}"
                )
            writer.writerow([
                result.get("example", ""),
                result.get("status", ""),
                result.get("reason", ""),
                result.get("rc", ""),
                f"{float(result.get('duration_s') or 0):.3f}",
                result.get("failing_step", ""),
                " | ".join(retries),
            ])

effective_failures = []
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

lines = ["# Presmoke Orchestrate Report", ""]
lines.append(f"- root: `{root}`")
lines.append(f"- generated_at: `{datetime.datetime.now().astimezone().isoformat(timespec='seconds')}`")
if primary:
    name, run_dir, meta, data = primary
    summary = data.get("summary", {})
    npu_stats = data.get("npu_stats") or {}
    results = data.get("results", [])
    lines.extend([
        "",
        "## Primary Full Run",
        "",
        f"- run: `{name}`",
        f"- card: `{meta.get('card', '')}`",
        f"- arch: `{meta.get('arch', '')}`",
        f"- schedule: `{meta.get('schedule', '')}`",
        f"- jobs/npu_slots: `{meta.get('jobs', '')}/{meta.get('npu_slots', '')}`",
        f"- timeout: `{meta.get('timeout', '')}`",
        f"- elapsed_sec: `{meta.get('elapsed_sec', '')}`",
        f"- planned_results: `{len(results)}`",
        f"- pass/fail/skip: `{summary.get('PASS', 0)}/{summary.get('FAIL', 0)}/{summary.get('SKIP', 0)}`",
    ])
    if npu_stats:
        lines.extend([
            f"- npu_busy_s: `{float(npu_stats.get('busy_s') or 0):.3f}`",
            f"- npu_idle_s: `{float(npu_stats.get('idle_s') or 0):.3f}`",
            f"- npu_utilization: `{float(npu_stats.get('utilization') or 0):.3f}`",
        ])
    failures = [r for r in results if r.get("status") == "FAIL"]
    skips = [r for r in results if r.get("status") == "SKIP"]
    lines.extend(["", "## Failures", ""])
    if failures:
        lines.append("| example | status | reason | rc | duration_s | failing_step | retry |")
        lines.append("|---|---:|---|---:|---:|---|---|")
        for result in failures:
            retry_text = []
            for retry_name, retry_meta, retry_result in retry_by_example.get(result.get("example", ""), []):
                retry_text.append(
                    f"{retry_name} card={retry_meta.get('card','')} {retry_result.get('status','')}"
                    f" {retry_result.get('reason','')} rc={retry_result.get('rc','')}"
                )
            failing_step = str(result.get("failing_step", "")).replace("|", "\\|")
            lines.append(
                f"| {result.get('example','')} | {result.get('status','')} | {result.get('reason','')} | "
                f"{result.get('rc','')} | {float(result.get('duration_s') or 0):.1f} | "
                f"{failing_step} | {'<br>'.join(retry_text)} |"
            )
    else:
        lines.append("No failures.")
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
    if effective_failures:
        lines.extend(["", "## Effective Status", "", f"`FAIL`: `{len(effective_failures)}` unrecovered failed cases."])
    else:
        lines.extend(["", "## Effective Status", "", "`PASS`: no unrecovered failed cases."])
    lines.extend([
        "",
        "## Artifacts",
        "",
        f"- all case timings: `{timings_path}`",
        f"- failures tsv: `{failures_path}`",
        f"- primary JSON: `{run_dir / 'results' / 'report.json'}`",
        f"- primary Markdown: `{run_dir / 'results' / 'report.md'}`",
    ])
else:
    lines.extend(["", "Primary full run report was not found.", "", "## Effective Status", "", "`FAIL`: primary report missing."])

(root / "FINAL_REPORT.md").write_text("\n".join(lines) + "\n", encoding="utf-8")
PY
    log "final_report_written $OUT_ROOT/FINAL_REPORT.md"
}

main() {
    acquire_lock
    log "orchestrate_start out_root=$OUT_ROOT"
    run_presmoke "full_card${PRIMARY_CARD}" "$PRIMARY_CARD" "$@"
    retry_timeouts
    write_final_report
    local effective_rc
    effective_rc="$(cat "$OUT_ROOT/effective_rc.txt" 2>/dev/null || echo 1)"
    log "orchestrate_done out_root=$OUT_ROOT effective_rc=$effective_rc"
    exit "$effective_rc"
}

main "$@"
