#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
mode="${1:-npu}"
case "$mode" in
    npu) ;;
    sim)
        sim_lib="${SIMULATOR_LIB:-${ASCEND_HOME_PATH:-/usr/local/Ascend/cann}/tools/simulator/Ascend950PR_9599/lib}"
        test -f "$sim_lib/libnpu_drv.so" || { echo "Missing simulator: $sim_lib" >&2; exit 1; }
        export LD_LIBRARY_PATH="$sim_lib:${LD_LIBRARY_PATH:-}"
        ;;
    *) echo "Usage: bash run.sh [npu|sim]" >&2; exit 2 ;;
esac
repo_root="$(cd ../../../../.. && pwd)"
cmake -S . -B build -DASC_DEVKIT_SOURCE="${ASC_DEVKIT_SOURCE:-$repo_root}" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build -j "${BUILD_JOBS:-4}"
timeout "${RUN_TIMEOUT:-120}" ./build/demo "$PWD/acl.json" 2>&1 | tee run.log
python3 check_output.py run.log
