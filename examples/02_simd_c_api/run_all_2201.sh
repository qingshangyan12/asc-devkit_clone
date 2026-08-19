#!/usr/bin/env bash
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
#
# Build and run every dav-2201-compatible SIMD C API sample. Each project has
# an isolated build directory so CMake cache entries cannot leak between cases
# or run modes.

set -uo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
RUN_MODE=npu
JOBS=${JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1)}
TIMEOUT_SECONDS=300
BUILD_ROOT="${SCRIPT_DIR}/.build_all_2201"
LOG_ROOT="${SCRIPT_DIR}/run_all_2201_logs/$(date +%Y%m%d_%H%M%S)"
LIST_ONLY=false

usage()
{
    cat <<'EOF'
Usage: ./run_all_2201.sh [options]

Build and run every dav-2201-compatible sample under 02_simd_c_api.

Options:
  --mode MODE          Run mode passed to CMake: npu (default) or sim.
  --jobs N             Parallel build jobs for each sample.
  --timeout-seconds N  Per-sample execution timeout in seconds (default: 300).
  --build-root DIR     Parent directory for isolated CMake build trees.
  --list               List all planned runs without configuring or executing.
  -h, --help           Show this help message.

Before running, source the CANN/AscendC environment that provides CMake's ASC
package. NPU mode requires a dav-2201 device. Use --mode sim for simulation.
EOF
}

while [ "$#" -gt 0 ]; do
    case "$1" in
        --mode)
            RUN_MODE=${2:-}
            shift 2
            ;;
        --jobs)
            JOBS=${2:-}
            shift 2
            ;;
        --timeout-seconds)
            TIMEOUT_SECONDS=${2:-}
            shift 2
            ;;
        --build-root)
            BUILD_ROOT=${2:-}
            shift 2
            ;;
        --list)
            LIST_ONLY=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "ERROR: unknown option: $1" >&2
            usage >&2
            exit 2
            ;;
    esac
done

if [ "$RUN_MODE" != "npu" ] && [ "$RUN_MODE" != "sim" ]; then
    echo "ERROR: --mode must be npu or sim, got: $RUN_MODE" >&2
    exit 2
fi

if ! [[ "$JOBS" =~ ^[1-9][0-9]*$ ]]; then
    echo "ERROR: --jobs must be a positive integer, got: $JOBS" >&2
    exit 2
fi

if ! [[ "$TIMEOUT_SECONDS" =~ ^[1-9][0-9]*$ ]]; then
    echo "ERROR: --timeout-seconds must be a positive integer, got: $TIMEOUT_SECONDS" >&2
    exit 2
fi

is_dav2201_compatible()
{
    case "$1" in
        00_introduction/00_quickstart/hello_world_npu|\
        00_introduction/01_add/c_api_async_add|\
        00_introduction/01_add/c_api_delicacy_async_add|\
        00_introduction/01_add/c_api_sync_add|\
        02_features/03_c_api/00_vector_compute/00_sync_add|\
        02_features/03_c_api/00_vector_compute/01_async_add|\
        02_features/03_c_api/00_vector_compute/02_c_api_delicacy_async_add)
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

mkdir -p "$LOG_ROOT"
RESULT_FILE="${LOG_ROOT}/summary.tsv"
printf 'status\tcase\tlog\n' > "$RESULT_FILE"

TOTAL=0
PASS=0
FAIL=0
SKIP=0

record_result()
{
    local status=$1
    local name=$2
    local log_file=$3

    TOTAL=$((TOTAL + 1))
    case "$status" in
        PASS) PASS=$((PASS + 1)) ;;
        SKIP) SKIP=$((SKIP + 1)) ;;
        PLANNED) ;;
        *) FAIL=$((FAIL + 1)) ;;
    esac
    printf '%-14s %s\n' "$status" "$name"
    printf '%s\t%s\t%s\n' "$status" "$name" "$log_file" >> "$RESULT_FILE"
}

find_executable()
{
    local build_dir=$1
    local candidate

    for candidate in "$build_dir/demo" "$build_dir/c_api_add_example" \
        "$build_dir/demo.exe" "$build_dir/c_api_add_example.exe"; do
        if [ -x "$candidate" ]; then
            printf '%s' "$candidate"
            return 0
        fi
    done

    candidate=$(find "$build_dir" -maxdepth 1 -type f -perm -u+x \
        ! -name 'CMakeCache.txt' ! -name '*.cmake' -print -quit 2>/dev/null)
    if [ -n "$candidate" ]; then
        printf '%s' "$candidate"
        return 0
    fi
    return 1
}

run_case()
{
    local rel_dir=$1
    local build_dir
    local log_file
    local executable
    local data_generator

    build_dir="${BUILD_ROOT}/${RUN_MODE}/${rel_dir//\//__}"
    log_file="${LOG_ROOT}/${rel_dir//\//__}.log"

    if ! cmake -S "${SCRIPT_DIR}/${rel_dir}" -B "$build_dir" \
        -DCMAKE_ASC_ARCHITECTURES=dav-2201 \
        -DCMAKE_ASC_RUN_MODE="$RUN_MODE" > "$log_file" 2>&1; then
        record_result CONFIGURE_FAIL "$rel_dir" "$log_file"
        return
    fi
    if ! cmake --build "$build_dir" --parallel "$JOBS" >> "$log_file" 2>&1; then
        record_result BUILD_FAIL "$rel_dir" "$log_file"
        return
    fi
    if ! executable=$(find_executable "$build_dir"); then
        echo 'ERROR: no executable was produced in the build directory.' >> "$log_file"
        record_result EXECUTABLE_MISSING "$rel_dir" "$log_file"
        return
    fi

    data_generator="${SCRIPT_DIR}/${rel_dir}/scripts/gen_data.py"
    if [ -f "$data_generator" ]; then
        if ! command -v python3 > /dev/null 2>&1; then
            echo 'ERROR: python3 is required to generate sample data.' >> "$log_file"
            record_result DATA_GEN_FAIL "$rel_dir" "$log_file"
            return
        fi
        {
            echo
            echo "===== Generating data for ${rel_dir} ====="
            (
                cd "$build_dir" &&
                python3 "$data_generator"
            )
        } >> "$log_file" 2>&1
        if [ "$?" -ne 0 ]; then
            record_result DATA_GEN_FAIL "$rel_dir" "$log_file"
            return
        fi
    fi

    {
        echo
        echo "===== Running ${executable} ====="
        (
            cd "$build_dir" &&
            if command -v timeout > /dev/null 2>&1; then
                timeout --preserve-status "$TIMEOUT_SECONDS" "$executable"
            else
                "$executable"
            fi
        )
    } >> "$log_file" 2>&1
    if [ "$?" -eq 0 ]; then
        record_result PASS "$rel_dir" "$log_file"
    else
        record_result RUN_FAIL "$rel_dir" "$log_file"
    fi
}

mapfile -t PROJECT_DIRS < <(find "$SCRIPT_DIR" -type f -name CMakeLists.txt -exec dirname {} \; | sort)

if [ "${#PROJECT_DIRS[@]}" -eq 0 ]; then
    echo "ERROR: no CMakeLists.txt found under ${SCRIPT_DIR}" >&2
    exit 2
fi

if [ "$LIST_ONLY" = false ] && ! command -v cmake > /dev/null 2>&1; then
    echo 'ERROR: cmake is not available. Source the CANN/AscendC environment first.' >&2
    exit 2
fi

echo "SIMD C API dav-2201 run"
echo "Mode: ${RUN_MODE}"
echo "Build root: ${BUILD_ROOT}"
echo "Logs: ${LOG_ROOT}"
echo
printf '%-14s %s\n' STATUS CASE

for project_dir in "${PROJECT_DIRS[@]}"; do
    rel_dir=${project_dir#"${SCRIPT_DIR}/"}
    if ! is_dav2201_compatible "$rel_dir"; then
        record_result SKIP "${rel_dir} (dav-3510 only)" '-'
        continue
    fi

    if [ "$LIST_ONLY" = true ]; then
        record_result PLANNED "$rel_dir" '-'
    else
        run_case "$rel_dir"
    fi
done

echo
echo "Summary: total=${TOTAL}, pass=${PASS}, fail=${FAIL}, skip=${SKIP}"
echo "Result file: ${RESULT_FILE}"

if [ "$FAIL" -ne 0 ]; then
    exit 1
fi
