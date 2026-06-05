#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="${PROJECT_ROOT:-/data/zgx/asc-devkit}"
ASCEND_HOME_DIR="${ASCEND_HOME_DIR:-/usr/local/Ascend/cann-9.1.0}"
ARCH="${ARCH:-dav-2201}"
MODES="${MODES:-npu}"
JOBS="${JOBS:-4}"
NPU_SLOTS="${NPU_SLOTS:-1}"
TIMEOUT="${TIMEOUT:-120}"
PRIMARY_CARD="${PRIMARY_CARD:-1}"
RETRY_CARDS="${RETRY_CARDS:-2 3 4 5 6 7 0}"
WAIT_PID_FILE="${WAIT_PID_FILE:-/tmp/run_presmoke2_full_exclude_000102.pid}"
OUT_ROOT="${OUT_ROOT:-/tmp/presmoke2_overnight_$(date +%Y%m%d_%H%M%S)}"

mkdir -p "$OUT_ROOT"
STATUS_FILE="$OUT_ROOT/status.txt"

log() {
    printf '[%s] %s\n' "$(date -Is)" "$*" | tee -a "$STATUS_FILE"
}

source_cann() {
    set +u
    # shellcheck disable=SC1091
    source "$ASCEND_HOME_DIR/set_env.sh" 2>/dev/null || true
    set -u
}

wait_for_existing_run() {
    if [[ ! -f "$WAIT_PID_FILE" ]]; then
        return
    fi
    local pid
    pid="$(cat "$WAIT_PID_FILE" 2>/dev/null || true)"
    if [[ -z "$pid" ]] || ! kill -0 "$pid" 2>/dev/null; then
        return
    fi
    log "waiting_for_existing_run pid=$pid"
    while kill -0 "$pid" 2>/dev/null; do
        sleep 60
    done
    log "existing_run_finished pid=$pid"
}

run_presmoke() {
    local name="$1"
    local card="$2"
    shift 2
    local run_dir="$OUT_ROOT/$name"
    mkdir -p "$run_dir"
    log "run_start name=$name card=$card jobs=$JOBS npu_slots=$NPU_SLOTS timeout=$TIMEOUT args=$*"
    {
        echo "name=$name"
        echo "started_at=$(date -Is)"
        echo "project_root=$PROJECT_ROOT"
        echo "git=$(cd "$PROJECT_ROOT" && git rev-parse --short HEAD 2>/dev/null || true)"
        echo "arch=$ARCH"
        echo "modes=$MODES"
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
        bash scripts/run_presmoke2.sh \
            --arch "$ARCH" --modes "$MODES" --jobs "$JOBS" --npu-slots "$NPU_SLOTS" --timeout "$TIMEOUT" \
            --report-format all --results "$run_dir/results" "$@"
    ) > "$run_dir/stdout.log" 2> "$run_dir/stderr.log"
    rc=$?
    end="$(date +%s)"
    set -e

    {
        echo "finished_at=$(date -Is)"
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

json_value() {
    local report="$1"
    local expr="$2"
    python3 - "$report" "$expr" <<'PY'
import json
import sys

path, expr = sys.argv[1], sys.argv[2]
data = json.load(open(path, encoding="utf-8"))
if expr == "timeout_examples":
    for result in data.get("results", []):
        if result.get("status") == "FAIL" and result.get("reason") == "timeout":
            print(result.get("example", ""))
elif expr == "fail_examples":
    for result in data.get("results", []):
        if result.get("status") == "FAIL":
            print(result.get("example", ""))
PY
}

retry_timeouts() {
    local full_report="$OUT_ROOT/full_card${PRIMARY_CARD}/results/report.json"
    [[ -f "$full_report" ]] || return
    mapfile -t timeout_examples < <(json_value "$full_report" timeout_examples)
    if [[ "${#timeout_examples[@]}" -eq 0 ]]; then
        log "retry_skip no_timeouts"
        return
    fi
    log "retry_timeouts count=${#timeout_examples[@]}"
    local example card safe_name retry_report status
    for example in "${timeout_examples[@]}"; do
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
import json
import os
import pathlib
import re
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
                k, v = line.split("=", 1)
                meta[k] = v
    return meta

runs = []
for report in sorted(root.glob("*/results/report.json")):
    run_dir = report.parents[1]
    data = read_json(report)
    if not data:
        continue
    meta = read_meta(run_dir / "meta.txt")
    runs.append((run_dir.name, run_dir, meta, data))

primary = None
for item in runs:
    if item[0].startswith("full_card"):
        primary = item
        break

retry_by_example = {}
for name, run_dir, meta, data in runs:
    if not name.startswith("retry_"):
        continue
    results = data.get("results", [])
    if results:
        retry_by_example.setdefault(results[0].get("example", ""), []).append((name, meta, results[0]))

timings_path = root / "ALL_CASE_TIMINGS.tsv"
failures_path = root / "FAILURES.tsv"
with timings_path.open("w", encoding="utf-8", newline="") as f:
    writer = csv.writer(f, delimiter="\t")
    writer.writerow(["run", "example", "arch", "mode", "status", "rc", "duration_s", "npu_wait_s", "reason", "failing_step", "steps", "log_file"])
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
                result.get("log_file", ""),
            ])

with failures_path.open("w", encoding="utf-8", newline="") as f:
    writer = csv.writer(f, delimiter="\t")
    writer.writerow(["example", "first_status", "first_reason", "first_rc", "first_duration_s", "first_failing_step", "retry_results"])
    if primary:
        _, _, _, data = primary
        for result in data.get("results", []):
            if result.get("status") == "PASS":
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

lines = ["# Presmoke Overnight Report", ""]
lines.append(f"- root: `{root}`")
lines.append(f"- generated_at: `{os.popen('date -Is').read().strip()}`")
lines.append("")
if primary:
    name, run_dir, meta, data = primary
    summary = data.get("summary", {})
    results = data.get("results", [])
    npu_stats = data.get("npu_stats") or {}
    lines.extend([
        "## Primary Full Run",
        "",
        f"- run: `{name}`",
        f"- card: `{meta.get('card', '')}`",
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
            f"- npu_max_slot_idle_s: `{float(npu_stats.get('max_slot_idle_s') or 0):.3f}`",
        ])
    lines.append("")
    failures = [r for r in results if r.get("status") != "PASS"]
    lines.append("## Failures")
    lines.append("")
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
    lines.append("Primary full run report was not found.")

(root / "FINAL_REPORT.md").write_text("\n".join(lines) + "\n", encoding="utf-8")
PY
    log "final_report_written $OUT_ROOT/FINAL_REPORT.md"
}

main() {
    log "nightly_start out_root=$OUT_ROOT"
    wait_for_existing_run
    run_presmoke "full_card${PRIMARY_CARD}" "$PRIMARY_CARD"
    retry_timeouts
    write_final_report
    log "nightly_done out_root=$OUT_ROOT"
}

main "$@"
