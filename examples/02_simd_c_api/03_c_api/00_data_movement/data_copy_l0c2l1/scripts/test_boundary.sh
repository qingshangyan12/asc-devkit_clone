#!/bin/bash
# Run boundary value (inf/-inf/nan) tests for all 17 quantization scenarios,
# testing both saturation (CTRL[48]=0) and non-saturation (CTRL[48]=1) modes.
# Usage: bash scripts/test_boundary.sh [build_dir]

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
EXAMPLE_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="${1:-${EXAMPLE_DIR}/build_boundary}"

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
        # Scenarios 11, 13: int4b_t is NOT in the official data type support
        #   matrix for asc_copy_l0c2l1. Additionally, asc_mmad_s4 is not
        #   implemented for dav-3510. These scenarios are permanently skipped.
        # Scenarios 12: bfloat16_t/int32_t IS officially supported, but CANN
        #   9.2.0 lacks the overload (ASC_HAS_L0C2L1_BF16_INT32).
        # Scenarios 14: fp8_e4m3fn_t/float IS officially supported, but CANN
        #   9.2.0 lacks the overload (ASC_HAS_L0C2L1_FP8).
        # Scenarios 15, 16: hifloat8_t/float IS officially supported, but CANN
        #   9.2.0 lacks the overload (ASC_HAS_L0C2L1_HIF8).
        # Toggle the feature macros in data_copy_l0c2l1.asc when upgrading CANN.
        case "$SCENARIO_NUM" in
            11|13)
                echo "=============================="
                echo "Scenario $SCENARIO_NUM ($MODE_NAME mode)"
                echo "=============================="
                echo "[SKIP] Scenario $SCENARIO_NUM ($MODE_NAME) — int4b_t not in official support matrix + ASC_HAS_MMAD_S4_3510=0"
                continue
                ;;
            12)
                echo "=============================="
                echo "Scenario $SCENARIO_NUM ($MODE_NAME mode)"
                echo "=============================="
                echo "[SKIP] Scenario $SCENARIO_NUM ($MODE_NAME) — ASC_HAS_L0C2L1_BF16_INT32=0"
                continue
                ;;
            14)
                echo "=============================="
                echo "Scenario $SCENARIO_NUM ($MODE_NAME mode)"
                echo "=============================="
                echo "[SKIP] Scenario $SCENARIO_NUM ($MODE_NAME) — ASC_HAS_L0C2L1_FP8=0"
                continue
                ;;
            15|16)
                echo "=============================="
                echo "Scenario $SCENARIO_NUM ($MODE_NAME mode)"
                echo "=============================="
                echo "[SKIP] Scenario $SCENARIO_NUM ($MODE_NAME) — ASC_HAS_L0C2L1_HIF8=0"
                continue
                ;;
        esac
        TOTAL=$((TOTAL + 1))
        echo "=============================="
        echo "Scenario $SCENARIO_NUM ($MODE_NAME mode)"
        echo "=============================="

        rm -rf "$BUILD_DIR"
        mkdir -p "$BUILD_DIR"
        cmake -DSCENARIO_NUM=$SCENARIO_NUM \
              -DSATURATION_MODE=$SAT_MODE \
              -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
              -B "$BUILD_DIR" -S "$EXAMPLE_DIR" 2>&1 | tail -1
        cmake --build "$BUILD_DIR" -j 2>&1
        if [ ! -f "$BUILD_DIR/demo" ]; then
            echo "[BUILD FAIL] Scenario $SCENARIO_NUM ($MODE_NAME)"
            FAIL=$((FAIL + 1))
            FAILED_SCENARIOS+=("$SCENARIO_NUM($MODE_NAME)")
            continue
        fi

        (cd "$BUILD_DIR" && python3 "$EXAMPLE_DIR/scripts/gen_data.py" \
            -scenarioNum=$SCENARIO_NUM --boundary --saturation=$SAT_MODE)

        (cd "$BUILD_DIR" && ./demo)

        if python3 "$EXAMPLE_DIR/scripts/verify_result.py" -scenarioNum=$SCENARIO_NUM \
            "$BUILD_DIR/output/output.bin" "$BUILD_DIR/output/golden.bin"; then
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
