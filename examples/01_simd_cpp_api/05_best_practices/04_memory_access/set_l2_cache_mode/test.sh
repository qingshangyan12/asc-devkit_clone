ASC_ARCH=dav-2201
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
mkdir -p "$SCRIPT_DIR/profiling_result"

for SCENARIO_NUM in $(seq 1 9); do
    echo ""
    echo "========================================="
    echo "Running scenario $SCENARIO_NUM with arch $ASC_ARCH"
    echo "========================================="

    cd "$SCRIPT_DIR" || exit 1

    # ---- cmake ----
    mkdir -p build
    cd build || { echo "[ERROR] Failed to enter build directory"; continue; }
    cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=$ASC_ARCH .. 2>&1
    if [ $? -ne 0 ]; then
        echo "[WARN] CMake failed for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    # ---- make ----
    make -j 2>&1
    if [ $? -ne 0 ]; then
        echo "[WARN] make failed for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    # ---- gen data ----
    python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM 2>&1
    if [ $? -ne 0 ]; then
        echo "[WARN] gen_data.py failed for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    # ---- profiling ----
    PROF_OUTPUT=$(msprof --ai-core=on --aic-metrics=L2Cache ./demo 2>&1)
    if [ $? -ne 0 ]; then
        echo "[WARN] msprof failed for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    PROF_DIR=$(echo "$PROF_OUTPUT" | grep -i "Data is saved in" | awk -F' in ' '{print $NF}' | tr -d '[:space:]')

    if [ -z "$PROF_DIR" ]; then
        echo "[WARN] Failed to get profiling result directory for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    echo "Profiling results: $PROF_DIR"

    # ---- copy op_summary CSV to profiling_result with suffix ----
    SUMMARY_CSV=$(ls "$PROF_DIR/mindstudio_profiler_output/op_summary_"*.csv 2>/dev/null | head -n 1)

    if [ -z "$SUMMARY_CSV" ]; then
        echo "[WARN] op_summary_*.csv not found in $PROF_DIR/mindstudio_profiler_output"
    else
        cp "$SUMMARY_CSV" "$SCRIPT_DIR/profiling_result/op_summary_${SCENARIO_NUM}_${ASC_ARCH}.csv"
        echo "  -> Copied op_summary_*.csv  -> op_summary_${SCENARIO_NUM}_${ASC_ARCH}.csv"
    fi

    cd "$SCRIPT_DIR"
done

echo ""
echo "========================================="
echo "All scenarios completed."
echo "Results saved in: $SCRIPT_DIR/profiling_result/"
echo "========================================="
ls -la "$SCRIPT_DIR/profiling_result/"
