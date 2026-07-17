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
# 1024 = small-shape / launch-overhead end; 65536 = the size the untiled wrapper died on;
# 131072 = 512KB, 2x the whole 248KB UB — proves the tiled wrapper is size-independent.
PERF_SIZES="${PERF_SIZES:-1024 65536 131072}"
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

preflight() { # returns non-zero on a FATAL problem -> abort before wasting builds
    local fatal=0
    echo "=========== PREFLIGHT ==========="
    echo "  mode=$MODE arch=$ARCH  shapes=[$SHAPES]"
    command -v cmake   >/dev/null || { echo "  !! cmake MISSING";   fatal=1; }
    command -v python3 >/dev/null || { echo "  !! python3 MISSING"; fatal=1; }
    echo "  cmake   : $(command -v cmake 2>/dev/null || echo MISSING)"
    echo "  python3 : $(command -v python3 2>/dev/null || echo MISSING)"
    if ! python3 -c 'import scipy, numpy' 2>/dev/null; then
        echo "  !! scipy/numpy MISSING — needed for the golden (pip install scipy)"; fatal=1
    else
        echo "  scipy   : $(python3 -c 'import scipy;print(scipy.__version__)' 2>/dev/null)"
    fi
    echo "  ASCEND_HOME_PATH=${ASCEND_HOME_PATH:-<unset — source set_env.sh>}"
    [ -z "${ASCEND_HOME_PATH:-}" ] && { echo "  !! set_env.sh not sourced"; fatal=1; }
    echo "  CANN    : $(cat ${ASCEND_HOME_PATH:-/nonexistent}/*/version.info 2>/dev/null | grep -m1 Version= || echo '?')"
    # npu-smi is a convenience probe only — absent inside many containers even when the NPU works.
    [ "$MODE" = "npu" ] && {
        if command -v npu-smi >/dev/null; then npu-smi info 2>&1 | head -3 | sed 's/^/     npu-smi: /'
        else echo "     npu-smi: not on PATH (not fatal — only a probe; the runtime is what matters)"; fi
    }
    for f in CMakeLists.txt data_utils.h ndtri.asc scripts/gen_data.py; do
        [ -f "$f" ] || { echo "  !! MISSING FILE: $f"; fatal=1; }
    done
    echo
    [ $fatal -ne 0 ] && { echo "PREFLIGHT FAILED — fix the !! items above. Not building."; return 1; }
    return 0
}

prof_clean() { rm -rf ./build/PROF_* ./build/*.csv 2>/dev/null; }

prof_run() { # run demo under msprof; FAIL loudly instead of letting extract read a stale CSV
    prof_clean
    local out
    if [ "$MODE" = "npu" ]; then out=$(cd build && msprof --application=./demo --output=./ 2>&1)
    else out=$(cd build && msprof op simulator --application=./demo 2>&1); fi
    if ! find ./build -name "op_summary_*.csv" 2>/dev/null | grep -q .; then
        echo "  !! msprof produced NO op_summary CSV — the kernel likely crashed. Output:"
        echo "$out" | tail -12 | sed 's/^/     /'
        return 1
    fi
    [ "$VERBOSE" = "1" ] && echo "$out" | tail -5 | sed 's/^/     /'
    return 0
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
        local out t0 t1 wall
        t0=$(date +%s.%N)
        out=$(cd build && ./demo 2>&1)
        t1=$(date +%s.%N)
        wall=$(awk "BEGIN{printf \"%.2f\", $t1-$t0}")
        [ "$VERBOSE" = "1" ] && echo "$out" | sed 's/^/     /'
        local line
        line=$(echo "$out" | grep -E "max rel err" | head -1)
        if echo "$out" | grep -q "test pass!"; then
            printf "  size=%-7s PASS   wall=%7ss   %s\n" "$n" "$wall" "${line:-}"
        else
            printf "  size=%-7s FAIL   wall=%7ss   %s\n" "$n" "$wall" "${line:-$(echo "$out" | tail -1)}"
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
        local w0 w1 wall
        w0=$(date +%s.%N)
        build 2 "$n" || { fail=1; continue; }
        (cd build && python3 ../scripts/gen_data.py "$n" >/dev/null 2>&1)
        prof_run || { echo "  size=$n  FAIL (baseline TEST_MODE=2)"; fail=1; continue; }
        local t1; t1=$(extract "aiv_time")

        build 3 "$n" || { fail=1; continue; }
        (cd build && python3 ../scripts/gen_data.py "$n" >/dev/null 2>&1)
        prof_run || { echo "  size=$n  FAIL (compute TEST_MODE=3)"; fail=1; continue; }
        local t2 ct; t2=$(extract "aiv_time"); ct=$(extract "aiv_vec_time")
        w1=$(date +%s.%N); wall=$(awk "BEGIN{printf \"%.1f\", $w1-$w0}")

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
        # per-element device compute time, so sizes are comparable: (t2-t1)/PERF_LOOPS/n
        local per_elem
        per_elem=$(python3 -c "
try:
    d=(float('$t2')-float('$t1'))/1000.0/$n*1e3   # us -> ns per element per pass
    print(f'{d:.3f}')
except Exception: print('NA')")
        printf "  size=%-7s %-6s aiv_vec=%-7s%%  wall=%5ss  %sns/elem  (t1=%s t2=%s vec=%s)\n" \
               "$n" "$verdict" "$ratio" "$wall" "$per_elem" "$t1" "$t2" "$ct"
    done
    return $fail
}

preflight || exit 1
rc=0
case "$FLAG" in
    --perf-only) run_perf || rc=1 ;;
    --perf)      run_func || rc=1; run_perf || rc=1 ;;
    *)           run_func || rc=1 ;;
esac
[ $rc -ne 0 ] && echo "  (re-run with VERBOSE=1 ./run.sh $MODE $FLAG   for full cmake/make/demo output; full log: $LOG)"
echo "=========== $([ $rc -eq 0 ] && echo 'ALL PASS' || echo 'FAILURES — see above') ==========="
exit $rc
