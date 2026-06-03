#!/usr/bin/env bash
set -euo pipefail

CASE_REL=03_simt_api/02_features/06_compile/01_separate_compile
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
    (cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc 'bisheng -dc gather_compute.asc -o gather_compute.o --npu-arch=dav-3510 --enable-simt')
    (cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc 'bisheng -dc gather_kernel.asc -o gather_kernel.o --npu-arch=dav-3510 --enable-simt')
    (cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc 'bisheng -c main.cpp -o main.o -I${ASCEND_HOME_PATH}/include')
    (cd "$CASE_DIR" && soc_version=$SOC_VERSION bash -lc 'bisheng gather_compute.o gather_kernel.o main.o -o demo')
}

run() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && soc_version=$SOC_VERSION bash -lc ./demo)
}

verify() {
    mkdir -p "$BUILD_DIR"
    :
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
