#!/usr/bin/env bash
set -euo pipefail

CASE_REL=03_simt_api/03_best_practices/00_memory_optimizations/cache_hint
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../../../../_case_entry.sh"
presmoke_case_init "$CASE_REL"

case_build() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && SCENARIO_NUM=1 soc_version=$SOC_VERSION bash -lc 'cmake -DSCENARIO_NUM=$SCENARIO_NUM .. -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG')
    (cd "$BUILD_DIR" && SCENARIO_NUM=1 soc_version=$SOC_VERSION bash -lc 'make -j')
}

case_run() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && SCENARIO_NUM=1 soc_version=$SOC_VERSION bash -lc ./data_cache_hint)
}

case_verify() {
    mkdir -p "$BUILD_DIR"
    :
}

case_clean() {
    presmoke_default_clean
}

presmoke_case_main "$@"
