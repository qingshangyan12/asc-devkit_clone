#!/bin/bash
# ==============================================================================
# Softmax Performance Tuning - Build & Verify Script
# ==============================================================================
# Usage:
#   ./build.sh -c 0                  # Build & run Case 0
#   ./build.sh -c 6                  # Build & run Case 6
#   ./build.sh batch                 # Batch: build & verify Case 0-6
#   ./build.sh clean                 # Clean build directory
#
# Environment variables (optional):
#   ASCEND_HOME            # CANN install path
#   CMAKE_ASC_RUN_MODE     # npu / cpu / sim (default: npu)
#   CMAKE_ASC_ARCHITECTURES # dav-3510 (default)
#   BUILD_JOBS             # Parallel build jobs (default: 8)
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
CANN_SETUP="${ASCEND_HOME:-/usr/local/Ascend/cann}/set_env.sh"

ARCH="${CMAKE_ASC_ARCHITECTURES:-dav-3510}"
RUN_MODE="${CMAKE_ASC_RUN_MODE:-npu}"
NJOBS="${BUILD_JOBS:-8}"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

info()  { echo -e "${GREEN}[INFO]${NC}  $*"; }
warn()  { echo -e "${YELLOW}[WARN]${NC}  $*"; }
error() { echo -e "${RED}[ERROR]${NC} $*"; }
step()  { echo -e "${CYAN}[STEP]${NC}  $*"; }

usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Options:
  -c, --case NUM        Build & run Case 0-6 (0-6)
  batch                 Batch build & verify all versions Case 0-6
  clean                 Clean build directory
  -h, --help            Show this help

Examples:
  $0 -c 0               Build and run Case 0
  $0 -c 6               Build and run Case 6
  $0 batch              Build and verify all Case 0-6
EOF
    exit 0
}

setup_env() {
    if [[ -f "$CANN_SETUP" ]]; then
        source "$CANN_SETUP" 2>/dev/null || true
        info "CANN environment loaded"
    else
        warn "CANN set_env.sh not found at: $CANN_SETUP"
    fi
}

do_clean() {
    step "Cleaning build directory: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
    info "Clean completed"
}

do_build_and_run() {
    local ver="$1"
    local vf_mode="false"

    # Case 0-2 are MemBase/RegBase-no-fusion, disable VF auto-fusion
    if [[ "$ver" -ge 0 && "$ver" -le 2 ]]; then
        vf_mode="false"
    else
        vf_mode="true"
    fi

    step "=== Building Case ${ver} (SCENARIO_NUM=${ver}, CMAKE_VF_MODE=${vf_mode}) ==="

    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"

    cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" \
        -DCMAKE_ASC_RUN_MODE="$RUN_MODE" \
        -DCMAKE_ASC_ARCHITECTURES="$ARCH" \
        -DSCENARIO_NUM="$ver" \
        -DCMAKE_VF_MODE="$vf_mode" || {
        error "CMake configuration failed for Case ${ver}"
        return 1
    }

    cmake --build "$BUILD_DIR" -j "$NJOBS" || {
        error "Build failed for Case ${ver}"
        return 1
    }

    step "=== Running Case ${ver} ==="
    cd "$BUILD_DIR"

    local run_prefix=""
    case "$RUN_MODE" in
        sim) run_prefix="msprof op simulator --soc-version=Ascend950PR_9599 --output=./msopprof_output_sim" ;;
        npu) run_prefix="msprof" ;;
    esac

    if $run_prefix ./demo; then
        info "Case ${ver} execution passed"
    else
        error "Case ${ver} execution failed"
        cd "$SCRIPT_DIR"
        return 1
    fi
    cd "$SCRIPT_DIR"

    # Verify result
    step "=== Verifying Case ${ver} accuracy ==="
    if python3 "${SCRIPT_DIR}/scripts/verify_result.py" output/output.bin output/golden.bin; then
        info "Case ${ver} accuracy PASSED"
        return 0
    else
        error "Case ${ver} accuracy FAILED"
        return 1
    fi
}

do_batch() {
    echo ""
    echo "============================================================"
    echo "  Batch Softmax Verification: Case 0 - Case 6"
    echo "  Run mode: $RUN_MODE"
    echo "============================================================"
    echo ""

    local failed=()
    local passed=()
    local start_time=$(date +%s)

    for ver in 0 1 2 3 4 5 6; do
        if do_build_and_run "$ver"; then
            passed+=("Case ${ver}")
        else
            failed+=("Case ${ver}")
        fi
        echo ""
    done

    local end_time=$(date +%s)
    local elapsed=$((end_time - start_time))

    echo "============================================================"
    echo "  Batch completed in ${elapsed}s"
    echo "  Passed: ${#passed[@]}/7  (${passed[*]:-none})"
    if [[ ${#failed[@]} -gt 0 ]]; then
        echo -e "  ${RED}Failed: ${#failed[@]}/7  (${failed[*]})${NC}"
    fi
    echo "============================================================"

    if [[ ${#failed[@]} -gt 0 ]]; then
        return 1
    fi
    return 0
}

main() {
    setup_env

    if [[ $# -eq 0 ]]; then
        usage
    fi

    while [[ $# -gt 0 ]]; do
        case "$1" in
            -c|--case)
                do_build_and_run "$2"
                shift 2
                ;;
            batch)
                do_batch
                shift
                ;;
            clean)
                do_clean
                shift
                ;;
            -h|--help)
                usage
                ;;
            *)
                error "Unknown option: $1"
                usage
                ;;
        esac
    done
}

main "$@"
