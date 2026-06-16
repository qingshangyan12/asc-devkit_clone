#!/bin/bash
# Bank conflict benchmark for dav-3510
# Usage:
#   SCENARIO_NUM=1 bash test.sh          # single case with profiling
#   bash test.sh                     # all 11 cases, accuracy only
#   bash test.sh profile             # all 11 cases with msprof profiling

set -e

WORKDIR=$(cd "$(dirname "$0")" && pwd)
RUN_PROFILE=false
[[ "$1" == "profile" ]] && RUN_PROFILE=true

if [[ -n "$SCENARIO_NUM" ]]; then
    CASES=("$SCENARIO_NUM")
else
    CASES=(1 2 3 4 5 6 7 8 9 10 11 12 13 14)
fi

echo "=========================================="
echo " Bank Conflict Benchmark  for dav-3510"
echo " Cases:  ${CASES[*]}"
echo " Profile: $RUN_PROFILE"
echo "=========================================="

SUMMARY_FILE="$WORKDIR/build/perf_summary.csv"
mkdir -p "$(dirname "$SUMMARY_FILE")"
printf "Case,Task_dura_us,aiv_time_us,aiv_vec_us,aiv_vec_rat,aiv_sca_us,aiv_sca_rat,mte2_rat,mte3_rat,Result\n" > "$SUMMARY_FILE"

for case in "${CASES[@]}"; do
    echo ""
    echo "--- Case $case ---"

    # Clean previous profiling output (generated in WORKDIR by msprof op)
    rm -rf "$WORKDIR/OPPROF_"* 2>/dev/null || true

    cmake -S "$WORKDIR" -B "$WORKDIR/build" \
        -DCMAKE_ASC_RUN_MODE=npu \
        -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
        -DCASE_NUM=$case -Wno-dev 2>&1 | grep "SCENARIO_NUM\|Build files\|Built"
    make -C "$WORKDIR/build" -j 2>&1 | grep "Built"

    rm -rf "$WORKDIR/input" "$WORKDIR/output"

    # Extract REPEAT_TIMES from source to keep golden in sync
    REPEAT=$(grep -oP 'REPEAT_TIMES_FOR_INSTR_EXCUTE\s*=\s*\K\d+' "$WORKDIR/bank_conflict.asc" 2>/dev/null || echo 1)
    python3 "$WORKDIR/scripts/gen_data.py" --case $case --repeat $REPEAT

    "$WORKDIR/build/demo" 2>&1 | grep -E "error ratio|test pass" || true

    VERIFY_RESULT="FAIL"
    python3 "$WORKDIR/scripts/verify_result.py" \
        "$WORKDIR/output/output_z.bin" "$WORKDIR/output/golden.bin" > /dev/null 2>&1 && VERIFY_RESULT="PASS"
    echo "Result: $VERIFY_RESULT"

    if $RUN_PROFILE; then
        echo "Profiling case $case ..."
        msprof op "$WORKDIR/build/demo" > /dev/null 2>&1
    fi

    # Collect metrics from OPPROF_* directories in WORKDIR
    TASK_DUR="N/A"; AIV_TIME="N/A"
    VEC_US="N/A"; VEC_R="N/A"; SCA_US="N/A"; SCA_R="N/A"
    M2_R="N/A"; M3_R="N/A"
    PROF_DIR=$(ls -d "$WORKDIR/OPPROF_"* 2>/dev/null | head -1 || true)

    if [[ -d "$PROF_DIR" ]]; then
        PIPE_CSV="$PROF_DIR/PipeUtilization.csv"
        BASIC_CSV="$PROF_DIR/OpBasicInfo.csv"

        if [[ -f "$PIPE_CSV" ]]; then
            # 0-idx: 21=aiv_time, 24=aiv_vec_time, 25=aiv_vec_ratio, 26=aiv_sca_time, 27=aiv_sca_ratio
            #        29=aiv_mte2_ratio, 32=aiv_mte3_ratio
            IFS=',' read -ra A < <(tail -1 "$PIPE_CSV")
            AIV_TIME="${A[21]}"
            VEC_US="${A[24]}"
            VEC_R="${A[25]}"
            SCA_US="${A[26]}"
            SCA_R="${A[27]}"
            M2_R="${A[29]}"
            M3_R="${A[32]}"
        fi

        if [[ -f "$BASIC_CSV" ]]; then
            TASK_DUR=$(awk -F',' 'NR>1 {print $3+0}' "$BASIC_CSV" 2>/dev/null)
        fi
    fi

    printf "%d,%s,%s,%s,%s,%s,%s,%s,%s,%s\n" \
        "$case" "$TASK_DUR" "$AIV_TIME" "$VEC_US" "$VEC_R" "$SCA_US" "$SCA_R" "$M2_R" "$M3_R" "$VERIFY_RESULT" >> "$SUMMARY_FILE"
done

echo ""
echo "=========================================="
echo " Performance Summary"
echo "=========================================="
python3 -c "
import csv, sys
with open('$SUMMARY_FILE') as f:
    r = csv.reader(f)
    for row in r:
        print('  ' + '  |  '.join(row))
"
echo ""
echo "Detailed results saved to: $SUMMARY_FILE"
