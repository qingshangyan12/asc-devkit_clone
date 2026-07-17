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

VERBOSE="${VERBOSE:-0}"
LOG=$PWD/build.log

build() { # $1=test_mode  $2=size
    mkdir -p build && cd build
    echo "  [build] cmake -DCMAKE_ASC_RUN_MODE=$MODE -DCMAKE_ASC_ARCHITECTURES=$ARCH -DTEST_MODE=$1 -DSIZE=$2 .."
    if [ "$VERBOSE" = "1" ]; then
        cmake -DCMAKE_ASC_RUN_MODE="$MODE" -DCMAKE_ASC_ARCHITECTURES="$ARCH" \
              -DTEST_MODE="$1" -DSIZE="$2" .. 2>&1 | tee "$LOG"
        local rc=${PIPESTATUS[0]}
        [ $rc -ne 0 ] && { echo "  !! cmake FAILED (rc=$rc)"; cd ..; return 1; }
        make -j 2>&1 | tee -a "$LOG"
        rc=${PIPESTATUS[0]}
        [ $rc -ne 0 ] && { echo "  !! make FAILED (rc=$rc)"; cd ..; return 1; }
    else
        # quiet, but ALWAYS surface errors on failure — never swallow them
        if ! cmake -DCMAKE_ASC_RUN_MODE="$MODE" -DCMAKE_ASC_ARCHITECTURES="$ARCH" \
                   -DTEST_MODE="$1" -DSIZE="$2" .. >"$LOG" 2>&1; then
            echo "  !! cmake FAILED — last 30 lines (full log: $LOG):"
            tail -30 "$LOG" | sed 's/^/     /'
            cd ..; return 1
        fi
        if ! make -j >"$LOG" 2>&1; then
            echo "  !! make FAILED — errors (full log: $LOG):"
            grep -E "error:|Error|undefined|No such file" "$LOG" | head -15 | sed 's/^/     /'
            [ -s "$LOG" ] || echo "     (log empty)"
            cd ..; return 1
        fi
    fi
    echo "  [build] ok -> $(cd "$OLDPWD/build" 2>/dev/null || true; ls -la demo 2>/dev/null | awk '{print $5" bytes"}')"
    cd ..
}

preflight() {
    echo "=========== PREFLIGHT ==========="
    echo "  mode=$MODE arch=$ARCH  shapes=[$SHAPES]"
    echo "  cmake   : $(command -v cmake || echo 'MISSING')"
    echo "  python3 : $(command -v python3 || echo 'MISSING')"
    echo "  scipy   : $(python3 -c 'import scipy;print(scipy.__version__)' 2>&1 | head -1)"
    echo "  ASCEND_HOME_PATH=${ASCEND_HOME_PATH:-<unset — did you source set_env.sh?>}"
    echo "  CANN    : $(cat ${ASCEND_HOME_PATH:-/nonexistent}/*/version.info 2>/dev/null | grep -m1 Version= || echo '? (ASCEND_HOME_PATH unset or layout differs)')"
    [ "$MODE" = "npu" ] && { echo "  npu-smi : $(command -v npu-smi || echo 'MISSING')"; npu-smi info 2>&1 | head -3 | sed 's/^/     /'; }
    for f in CMakeLists.txt data_utils.h ndtri.asc scripts/gen_data.py; do
        [ -f "$f" ] || echo "  !! MISSING FILE: $f"
    done
    echo
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
        if ! (cd build && python3 ../scripts/gen_data.py "$n" >"$LOG.gen" 2>&1); then
            echo "  size=$n  !! gen_data FAILED:"; tail -12 "$LOG.gen" | sed 's/^/     /'; fail=1; continue
        fi
        [ "$VERBOSE" = "1" ] && grep gen_data "$LOG.gen" | sed 's/^/     /'
        local out
        out=$(cd build && ./demo 2>&1)
        [ "$VERBOSE" = "1" ] && echo "$out" | sed 's/^/     /'
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

preflight
rc=0
case "$FLAG" in
    --perf-only) run_perf || rc=1 ;;
    --perf)      run_func || rc=1; run_perf || rc=1 ;;
    *)           run_func || rc=1 ;;
esac
[ $rc -ne 0 ] && echo "  (re-run with VERBOSE=1 ./run.sh $MODE $FLAG   for full cmake/make/demo output; full log: $LOG)"
echo "=========== $([ $rc -eq 0 ] && echo 'ALL PASS' || echo 'FAILURES — see above') ==========="
exit $rc
