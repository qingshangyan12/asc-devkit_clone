#!/usr/bin/env bash
set -euo pipefail

CASE_REL=02_simd_c_api/00_introduction/04_reg_base_add_compute/c_api_simd_add
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../../../../_case_entry.sh"
presmoke_case_init "$CASE_REL"

case_build() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc 'cmake .. -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG')
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc 'make -j')
}

case_run() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc ./c_api_add_example)
}

case_verify() {
    mkdir -p "$BUILD_DIR"
    :
}

case_clean() {
    presmoke_default_clean
}

presmoke_case_main "$@"
