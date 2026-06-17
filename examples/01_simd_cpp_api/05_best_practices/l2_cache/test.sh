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
        echo "[WARNING] CMake failed for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    # ---- make ----
    make -j 2>&1
    if [ $? -ne 0 ]; then
        echo "[WARNING] make failed for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    # ---- gen data ----
    python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM 2>&1
    if [ $? -ne 0 ]; then
        echo "[WARNING] gen_data.py failed for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    # ---- run demo ----
    ./demo 2>&1
    if [ $? -ne 0 ]; then
        echo "[WARNING] demo failed for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    # ---- profiling ----
    PROF_OUTPUT=$(msprof op ./demo 2>&1)
    PROF_DIR=$(echo "$PROF_OUTPUT" | grep "Profiling results saved in" | awk '{print $NF}')

    if [ -z "$PROF_DIR" ]; then
        echo "[WARNING] Failed to get profiling result directory (msprof may have failed) for scenario $SCENARIO_NUM, skip to next."
        cd "$SCRIPT_DIR"
        continue
    fi

    echo "Profiling results: $PROF_DIR"

    # ---- copy CSVs to profiling_result with suffix ----
    if [ -f "$PROF_DIR/L2Cache.csv" ]; then
        cp "$PROF_DIR/L2Cache.csv" "$SCRIPT_DIR/profiling_result/L2Cache_${SCENARIO_NUM}_${ASC_ARCH}.csv"
        echo "  -> Copied L2Cache.csv  -> L2Cache_${SCENARIO_NUM}_${ASC_ARCH}.csv"
    else
        echo "[WARNING] L2Cache.csv not found in $PROF_DIR"
    fi

    if [ -f "$PROF_DIR/PipeUtilization.csv" ]; then
        cp "$PROF_DIR/PipeUtilization.csv" "$SCRIPT_DIR/profiling_result/PipeUtilization_${SCENARIO_NUM}_${ASC_ARCH}.csv"
        echo "  -> Copied PipeUtilization.csv  -> PipeUtilization_${SCENARIO_NUM}_${ASC_ARCH}.csv"
    else
        echo "[WARNING] PipeUtilization.csv not found in $PROF_DIR"
    fi

    cd "$SCRIPT_DIR"
done

echo ""
echo "========================================="
echo "All scenarios completed."
echo "Results saved in: $SCRIPT_DIR/profiling_result/"
echo "========================================="
ls -la "$SCRIPT_DIR/profiling_result/"
