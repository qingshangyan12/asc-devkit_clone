#!/usr/bin/env bash
set -euo pipefail

CASE_REL=01_simd_cpp_api/02_features/02_framework/00_pytorch/torch_library
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../../../../../_case_entry.sh"
presmoke_case_init "$CASE_REL"

case_build() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc 'cmake .. -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG')
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc 'make -j')
}

case_run() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc 'python3 ../add_custom_test.py')
}

case_verify() {
    mkdir -p "$BUILD_DIR"
    :
}

case_clean() {
    presmoke_default_clean
}

presmoke_case_main "$@"
