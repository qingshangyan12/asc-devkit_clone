#!/usr/bin/env bash
set -euo pipefail

CASE_REL=01_simd_cpp_api/02_features/00_compilation/parallel_ops_package
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../../../../_case_entry.sh"
presmoke_case_init "$CASE_REL"

case_build() {
    mkdir -p "$BUILD_DIR"
    (cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc 'cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG')
    (cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc 'cmake --build build -j')
}

case_run() {
    mkdir -p "$BUILD_DIR"
    (cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc './build/add_custom/custom_opp_*.run')
    (cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc './build/leaky_relu_custom/custom_opp_*.run')
}

case_verify() {
    mkdir -p "$BUILD_DIR"
    :
}

case_clean() {
    presmoke_default_clean
}

presmoke_case_main "$@"
