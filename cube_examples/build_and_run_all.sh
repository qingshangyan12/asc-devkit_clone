#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LOG_FILE="${SCRIPT_DIR}/build_and_run.log"
echo "=== build_and_run_all $(date) ===" > "$LOG_FILE"

if [ -f "${HOME}/Ascend/ascend-toolkit/set_env.sh" ]; then
    source "${HOME}/Ascend/ascend-toolkit/set_env.sh"
fi
P=0; F=0; S=0
for d in "$SCRIPT_DIR"/*/; do
    n=$(basename "$d")
    a=$(grep "npu-arch" "$d/CMakeLists.txt" 2>/dev/null | grep -oP 'dav-\d+')
    [ -z "$a" ] && a="dav-3510"
    # 尝试 cann-9.0.0-beta.2, 回退到 cann-9.1.0-beta.1
    sl="/home/jinhu/Ascend/cann-9.0.0-beta.2/x86_64-linux/simulator/${a}/lib"
    [ ! -d "$sl" ] && sl="/home/jinhu/Ascend/cann-9.1.0-beta.1/x86_64-linux/simulator/${a}/lib"

    rm -rf "$d/build" 2>/dev/null; mkdir -p "$d/build"
    if ! cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=${a} -S "$d" -B "$d/build" >> "$LOG_FILE" 2>&1; then
        echo "[SKIP] $n (cmake failed)" | tee -a "$LOG_FILE"; S=$((S+1)); continue
    fi
    if ! make -j -C "$d/build" >> "$LOG_FILE" 2>&1; then
        echo "[SKIP] $n (make failed)" | tee -a "$LOG_FILE"; S=$((S+1)); continue
    fi
    exe=$(find "$d/build" -maxdepth 1 -type f -executable | head -1)
    [ -z "$exe" ] && echo "[SKIP] $n (no binary)" | tee -a "$LOG_FILE" && S=$((S+1)) && continue

    run_out="$d/build/run.log"
    (cd "$d/build" && LD_LIBRARY_PATH="${sl}:${LD_LIBRARY_PATH}" "$exe" > "$run_out" 2>&1)
    csv="$d/build/${n}.csv"; [ -f "$csv" ] && cp "$csv" "$d/" 2>/dev/null

    if grep -q "^PASS" "$d/${n}.csv" 2>/dev/null; then
        echo "[PASS] $n" | tee -a "$LOG_FILE"; P=$((P+1))
    else
        echo "[FAIL] $n" | tee -a "$LOG_FILE"; F=$((F+1))
        echo "  stdout:"; cat "$run_out" 2>/dev/null
        echo "  csv: $(cat "$d/${n}.csv" 2>/dev/null)"
    fi
done
echo "=== ${P} passed, ${F} failed, ${S} skipped ===" | tee -a "$LOG_FILE"
echo "日志: ${LOG_FILE}"
