#!/usr/bin/env bash
set -euo pipefail

CASE_REL=01_simd_cpp_api/04_best_practices/04_memory_access_practices/data_copy
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/../../../../_case_entry.sh"
presmoke_case_init "$CASE_REL"

case_build() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && ASC_ARCH=$ARCH COPY_DST=UB SCENARIO_NUM=1 soc_version=$SOC_VERSION bash -lc 'cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCOPY_DST=$COPY_DST .. -DCMAKE_ASC_ARCHITECTURES="$ARCH" $RUN_MODE_ARG')
    (cd "$BUILD_DIR" && ASC_ARCH=$ARCH COPY_DST=UB SCENARIO_NUM=1 soc_version=$SOC_VERSION bash -lc 'make -j')
    (cd "$BUILD_DIR" && ASC_ARCH=$ARCH COPY_DST=UB SCENARIO_NUM=1 soc_version=$SOC_VERSION bash -lc 'python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM -copyDst $COPY_DST -arch $ASC_ARCH')
}

case_run() {
    mkdir -p "$BUILD_DIR"
    (cd "$BUILD_DIR" && ASC_ARCH=$ARCH COPY_DST=UB SCENARIO_NUM=1 soc_version=$SOC_VERSION bash -lc ./demo)
}

case_verify() {
    mkdir -p "$BUILD_DIR"
    :
}

case_clean() {
    presmoke_default_clean
}

presmoke_case_main "$@"
