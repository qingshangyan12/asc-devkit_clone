#!/usr/bin/env bash
set -euo pipefail

CASE_REL=01_simd_cpp_api/05_compatibility_guide/matmul_s4
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [[ -n "${PRESMOKE_PROJECT_ROOT:-}" ]]; then
    COMMON_SH="$PRESMOKE_PROJECT_ROOT/scripts/presmoke/case_common.sh"
else
    SEARCH_DIR="$SCRIPT_DIR"
    COMMON_SH=""
    while [[ "$SEARCH_DIR" != "/" ]]; do
        if [[ -f "$SEARCH_DIR/scripts/presmoke/case_common.sh" ]]; then
            COMMON_SH="$SEARCH_DIR/scripts/presmoke/case_common.sh"
            break
        fi
        SEARCH_DIR="$(dirname "$SEARCH_DIR")"
    done
fi
if [[ -z "$COMMON_SH" || ! -f "$COMMON_SH" ]]; then
    echo "Cannot locate scripts/presmoke/case_common.sh; set PRESMOKE_PROJECT_ROOT" >&2
    exit 2
fi
source "$COMMON_SH"
CASE_DIR="$(presmoke_case_root "$CASE_REL")"
MODE="$(presmoke_mode)"
ARCH="$(presmoke_arch)"
SOC_VERSION="$(presmoke_soc_version "$ARCH")"
RUN_MODE_ARG=""
if [[ "$MODE" != "npu" ]]; then
    RUN_MODE_ARG="-DCMAKE_ASC_RUN_MODE=$MODE"
fi
export ARCH MODE SOC_VERSION RUN_MODE_ARG
BUILD_DIR="$(presmoke_build_dir "$CASE_DIR")"
ACTION="${1:-all}"

run_in_build_dir() {
    (cd "$BUILD_DIR" && "$@")
}

build() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc 'cmake .. -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG')
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc 'make -j')
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc 'python3 ../scripts/gen_data.py')
}

run() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc ./demo)
}

verify() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc 'python3 ../scripts/verify_result.py output/output.bin output/golden.bin')
}

clean() {
    rm -rf "$BUILD_DIR"
}

case "$ACTION" in
    build) build ;;
    run) run ;;
    verify) verify ;;
    clean) clean ;;
    all) build; run; verify ;;
    *) echo "Usage: $0 {build|run|verify|clean|all}" >&2; exit 2 ;;
esac
