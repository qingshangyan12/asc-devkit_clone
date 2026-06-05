#!/usr/bin/env bash
set -euo pipefail

CASE_REL=01_simd_cpp_api/02_features/04_aicpu/tiling_sink_programming
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../../../../_case_entry.sh"
presmoke_case_init "$CASE_REL"

case_build() {
    mkdir -p "$BUILD_DIR"
    presmoke_ensure_custom_op_package
}

case_run() {
    mkdir -p "$BUILD_DIR"
    (cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc 'python3 test_add_custom_tiling_sink.py')
}

case_verify() {
    mkdir -p "$BUILD_DIR"
    :
}

case_clean() {
    presmoke_default_clean
}

presmoke_case_main "$@"
