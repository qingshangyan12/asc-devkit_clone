#!/bin/bash
# Run boundary value (inf/-inf/nan) tests for all 17 quantization scenarios,
# testing both saturation (CTRL[48]=0) and non-saturation (CTRL[48]=1) modes.
# Usage: bash scripts/test_boundary.sh [build_dir]

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
EXAMPLE_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="${1:-${EXAMPLE_DIR}/build_boundary}"

cd "$EXAMPLE_DIR"

PASS=0
FAIL=0
TOTAL=0
FAILED_SCENARIOS=()

for SAT_MODE in 1 0; do
    MODE_NAME="saturation"
    if [ "$SAT_MODE" -eq 0 ]; then
        MODE_NAME="non-saturation"
    fi

    for SCENARIO_NUM in $(seq 1 17); do
        TOTAL=$((TOTAL + 1))
        echo "=============================="
        echo "Scenario $SCENARIO_NUM ($MODE_NAME mode)"
        echo "=============================="

        # Generate boundary value inputs with matching saturation mode
        python3 scripts/gen_data.py -scenarioNum=$SCENARIO_NUM --boundary --saturation=$SAT_MODE

        # Rebuild for this scenario and saturation mode
        rm -rf "$BUILD_DIR"
        mkdir -p "$BUILD_DIR"
        cmake -DSCENARIO_NUM=$SCENARIO_NUM \
              -DSATURATION_MODE=$SAT_MODE \
              -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
              -B "$BUILD_DIR" -S "$EXAMPLE_DIR" 2>&1 | tail -1
        cmake --build "$BUILD_DIR" -j 2>&1 | tail -1

        # Run kernel
        (cd "$BUILD_DIR" && ./demo)

        # Verify
        if python3 scripts/verify_result.py -scenarioNum=$SCENARIO_NUM \
            "$BUILD_DIR/output/output.bin" ./output/golden.bin; then
            echo "[PASS] Scenario $SCENARIO_NUM ($MODE_NAME)"
            PASS=$((PASS + 1))
        else
            echo "[FAIL] Scenario $SCENARIO_NUM ($MODE_NAME)"
            FAIL=$((FAIL + 1))
            FAILED_SCENARIOS+=("$SCENARIO_NUM($MODE_NAME)")
        fi
    done
done

echo ""
echo "=============================="
echo "Boundary Test Summary"
echo "=============================="
echo "PASS: $PASS / $TOTAL"
echo "FAIL: $FAIL / $TOTAL"
if [ $FAIL -gt 0 ]; then
    echo "Failed: ${FAILED_SCENARIOS[*]}"
fi
