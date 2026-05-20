#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG_FILE="${SCRIPT_DIR}/build_and_run.log"
echo "=== 000_cube_tests build_and_run_all $(date) ===" > "$LOG_FILE"

P=0; F=0; S=0

run_test() {
    local d="$1"
    local n=$(basename "$d")
    rm -rf "$d/build" 2>/dev/null; mkdir -p "$d/build"

    if ! cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S "$d" -B "$d/build" >> "$LOG_FILE" 2>&1; then
        echo "[SKIP] $n (cmake failed)" | tee -a "$LOG_FILE"; S=$((S+1)); return
    fi
    if ! make -j -C "$d/build" >> "$LOG_FILE" 2>&1; then
        echo "[SKIP] $n (make failed)" | tee -a "$LOG_FILE"; S=$((S+1)); return
    fi
    local exe=$(find "$d/build" -maxdepth 1 -type f -executable | head -1)
    [ -z "$exe" ] && echo "[SKIP] $n (no binary)" | tee -a "$LOG_FILE" && S=$((S+1)) && return

    local run_out="$d/build/run.log"
    (cd "$d/build" && "$exe" > "$run_out" 2>&1) || true
    local csv=$(find "$d/build" -name "*.csv" ! -name "run.log" | head -1)
    [ -f "$csv" ] && cp "$d/build"/*.csv "$d/" 2>/dev/null

    if grep -q "^PASS" "$d"/*.csv 2>/dev/null; then
        echo "[PASS] $n" | tee -a "$LOG_FILE"; P=$((P+1))
    else
        echo "[FAIL] $n" | tee -a "$LOG_FILE"; F=$((F+1))
        echo "  stdout:"; cat "$run_out" 2>/dev/null
    fi
}

echo "--- compute ---" | tee -a "$LOG_FILE"
for d in "$SCRIPT_DIR"/compute/*/; do run_test "$d"; done

echo "--- datamove ---" | tee -a "$LOG_FILE"
for d in "$SCRIPT_DIR"/datamove/*/; do run_test "$d"; done

echo "=== ${P} passed, ${F} failed, ${S} skipped ===" | tee -a "$LOG_FILE"
echo "日志: ${LOG_FILE}"
