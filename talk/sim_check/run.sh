#!/bin/bash
# Our standalone ndtri test. Self-contained — does NOT need the cann-ops-competitions repo.
#
#   ./run.sh sim              functional sweep on the simulator
#   ./run.sh npu              functional sweep on silicon
#   ./run.sh npu --perf       functional sweep + aiv_vec perf sweep (silicon)
#   ./run.sh npu --perf-only  just the perf sweep
#   ./run.sh sim --perf       perf on the simulator (msprof op simulator; INDICATIVE ONLY, not acceptance)
#
# Differences from the official harness run.sh (on purpose):
#   - sweeps all shapes in one invocation instead of one size per call
#   - does NOT hard-block perf on non-npu (the official one exits; its sim-msprof branch is dead code)
#   - prints a summary table you can paste into the self-test report
set -u

MODE="${1:-sim}"
FLAG="${2:-}"
ARCH="dav-3510"
SHAPES="${SHAPES:-1 32 1023 2048}"
PERF_SIZES="${PERF_SIZES:-1024 4096 8192 16384 32768 65536}"
THRESHOLD=90

case "$MODE" in
  sim|npu|cpu) ;;
  *) echo "usage: $0 <sim|npu|cpu> [--perf|--perf-only]"; exit 1;;
esac

build() { # $1=test_mode  $2=size
    mkdir -p build && cd build
    cmake -DCMAKE_ASC_RUN_MODE="$MODE" -DCMAKE_ASC_ARCHITECTURES="$ARCH" \
          -DTEST_MODE="$1" -DSIZE="$2" .. >/dev/null 2>&1 || { echo "cmake FAILED"; cd ..; return 1; }
    make -j >/dev/null 2>&1 || { echo "build FAILED"; cd ..; return 1; }
    cd ..
}

extract() { # $1=field   -> newest op_summary_*.csv under build/
    local csv
    csv=$(find ./build -type f -name "op_summary_*.csv" -printf '%T@ %p\n' 2>/dev/null | sort -rn | head -1 | cut -d' ' -f2-)
    [ -z "$csv" ] && { echo "NA"; return 1; }
    python3 - "$csv" "$1" <<'PY'
import csv, sys
path, field = sys.argv[1], sys.argv[2].lower()
with open(path, newline='', encoding='utf-8') as f:
    for row in csv.DictReader(f):
        for k, v in row.items():
            if k and k.strip().lower().startswith(field):
                try: print(float(v))
                except ValueError: print(v.strip())
                sys.exit(0)
print("NA")
PY
}

run_func() {
    echo "=========== FUNCTIONAL ($MODE) ==========="
    local fail=0
    for n in $SHAPES; do
        build 1 "$n" || { fail=1; continue; }
        (cd build && python3 ../scripts/gen_data.py "$n" >/dev/null 2>&1)
        local out
        out=$(cd build && ./demo 2>&1)
        local line
        line=$(echo "$out" | grep -E "max rel err" | head -1)
        if echo "$out" | grep -q "test pass!"; then
            printf "  size=%-6s PASS   %s\n" "$n" "${line:-}"
        else
            printf "  size=%-6s FAIL   %s\n" "$n" "${line:-$(echo "$out" | tail -1)}"
            fail=1
        fi
        # surface the Ln inf/nan hardware flag if the device raises it (open question F16)
        echo "$out" | grep -q "vec_err_idata_inf_nan" && echo "         note: RV_VLN inf/nan flag raised"
    done
    return $fail
}

run_perf() {
    echo "=========== PERF ($MODE)  aiv_vec = vec_time / (aiv_t3 - aiv_t2), need >= ${THRESHOLD}% ==========="
    [ "$MODE" != "npu" ] && echo "  WARNING: $MODE perf is INDICATIVE ONLY — acceptance requires -r npu on silicon."
    local fail=0
    for n in $PERF_SIZES; do
        build 2 "$n" || { fail=1; continue; }
        (cd build && python3 ../scripts/gen_data.py "$n" >/dev/null 2>&1)
        if [ "$MODE" = "npu" ]; then (cd build && msprof --application=./demo --output=./ >/dev/null 2>&1)
        else (cd build && msprof op simulator --application=./demo >/dev/null 2>&1); fi
        local t1; t1=$(extract "aiv_time")

        build 3 "$n" || { fail=1; continue; }
        (cd build && python3 ../scripts/gen_data.py "$n" >/dev/null 2>&1)
        if [ "$MODE" = "npu" ]; then (cd build && msprof --application=./demo --output=./ >/dev/null 2>&1)
        else (cd build && msprof op simulator --application=./demo >/dev/null 2>&1); fi
        local t2 ct; t2=$(extract "aiv_time"); ct=$(extract "aiv_vec_time")

        local ratio
        ratio=$(python3 -c "
try:
    t1,t2,c=float('$t1'),float('$t2'),float('$ct'); d=t2-t1
    print('NaN' if d<=0 else round(c/d*100,2))
except Exception: print('NaN')")
        local verdict="PASS"
        [ "$ratio" = "NaN" ] && verdict="NODATA"
        [ "$ratio" != "NaN" ] && [ "$(python3 -c "print(1 if float('$ratio')<$THRESHOLD else 0)")" = "1" ] && verdict="FAIL"
        [ "$verdict" != "PASS" ] && fail=1
        printf "  size=%-6s %-6s aiv_vec=%-7s%%  (t1=%s t2=%s vec=%s)\n" "$n" "$verdict" "$ratio" "$t1" "$t2" "$ct"
    done
    return $fail
}

rc=0
case "$FLAG" in
    --perf-only) run_perf || rc=1 ;;
    --perf)      run_func || rc=1; run_perf || rc=1 ;;
    *)           run_func || rc=1 ;;
esac
echo "=========== $([ $rc -eq 0 ] && echo 'ALL PASS' || echo 'FAILURES — see above') ==========="
exit $rc
