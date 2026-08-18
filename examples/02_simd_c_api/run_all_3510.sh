#!/usr/bin/env bash
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
#
# Build and run every dav-3510-compatible SIMD C API sample. The script keeps
# each project/scenario in an isolated build directory so CMake cache entries
# cannot leak between samples or run modes.

set -uo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
RUN_MODE=npu
JOBS=${JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1)}
TIMEOUT_SECONDS=300
BUILD_ROOT="${SCRIPT_DIR}/.build_all_3510"
LOG_ROOT="${SCRIPT_DIR}/run_all_3510_logs/$(date +%Y%m%d_%H%M%S)"
LIST_ONLY=false

usage()
{
    cat <<'EOF'
Usage: ./run_all_3510.sh [options]

Build and run every dav-3510-compatible sample under 02_simd_c_api.

Options:
  --mode MODE          Run mode passed to CMake: npu (default) or sim.
  --jobs N             Parallel build jobs for each sample.
  --timeout-seconds N  Per-sample execution timeout in seconds (default: 300).
  --build-root DIR     Parent directory for isolated CMake build trees.
  --list               List all planned runs without configuring or executing.
  -h, --help           Show this help message.

Before running, source the CANN/AscendC environment that provides CMake's ASC
package. NPU mode requires a dav-3510 device. Use --mode sim for simulation.
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

is_dav2201_only()
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

scenarios_for()
{
    case "$1" in
        02_features/03_c_api/01_data_movement/00_set_l2_cache_mode|\
        03_c_api/00_data_movement/data_copy_gm2l1)
            echo "1 2 3 4"
            ;;
        03_c_api/00_data_movement/data_copy_l0c2gm|\
        03_c_api/00_data_movement/reg_load_store_align)
            echo "1 2 3 4 5 6"
            ;;
        03_c_api/00_data_movement/data_copy_ub2l1|\
        03_c_api/00_data_movement/reg_load_gather|\
        03_c_api/00_data_movement/reg_load_store_mask|\
        03_c_api/02_reg_vector_compute/cast|\
        03_c_api/02_reg_vector_compute/compare|\
        03_c_api/02_reg_vector_compute/data_relayout|\
        03_c_api/03_matrix_compute/mmad)
            echo "1 2"
            ;;
        03_c_api/03_matrix_compute/mmad_mx)
            echo "1 2 3"
            ;;
        *)
            echo "-"
            ;;
    esac
}

case_name()
{
    if [ "$2" = "-" ]; then
        printf '%s' "$1"
    else
        printf '%s [SCENARIO_NUM=%s]' "$1" "$2"
    fi
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
    local scenario=$2
    local name
    local case_id
    local build_dir
    local log_file
    local executable
    local -a cmake_args

    name=$(case_name "$rel_dir" "$scenario")
    case_id=${rel_dir//\//__}
    if [ "$scenario" != "-" ]; then
        case_id="${case_id}__scenario_${scenario}"
    fi
    build_dir="${BUILD_ROOT}/${RUN_MODE}/${case_id}"
    log_file="${LOG_ROOT}/${case_id}.log"

    cmake_args=(
        -S "${SCRIPT_DIR}/${rel_dir}"
        -B "$build_dir"
        -DCMAKE_ASC_ARCHITECTURES=dav-3510
        -DCMAKE_ASC_RUN_MODE="$RUN_MODE"
    )
    if [ "$scenario" != "-" ]; then
        cmake_args+=("-DSCENARIO_NUM=${scenario}")
    fi

    if ! cmake "${cmake_args[@]}" > "$log_file" 2>&1; then
        record_result CONFIGURE_FAIL "$name" "$log_file"
        return
    fi
    if ! cmake --build "$build_dir" --parallel "$JOBS" >> "$log_file" 2>&1; then
        record_result BUILD_FAIL "$name" "$log_file"
        return
    fi
    if ! executable=$(find_executable "$build_dir"); then
        echo 'ERROR: no executable was produced in the build directory.' >> "$log_file"
        record_result EXECUTABLE_MISSING "$name" "$log_file"
        return
    fi

    {
        echo
        echo "===== Running ${executable} ====="
        if command -v timeout > /dev/null 2>&1; then
            timeout --preserve-status "$TIMEOUT_SECONDS" "$executable"
        else
            "$executable"
        fi
    } >> "$log_file" 2>&1
    if [ "$?" -eq 0 ]; then
        record_result PASS "$name" "$log_file"
    else
        record_result RUN_FAIL "$name" "$log_file"
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

echo "SIMD C API dav-3510 run"
echo "Mode: ${RUN_MODE}"
echo "Build root: ${BUILD_ROOT}"
echo "Logs: ${LOG_ROOT}"
echo
printf '%-14s %s\n' STATUS CASE

for project_dir in "${PROJECT_DIRS[@]}"; do
    rel_dir=${project_dir#"${SCRIPT_DIR}/"}
    if is_dav2201_only "$rel_dir"; then
        record_result SKIP "$(case_name "$rel_dir" "-") (dav-2201 only)" '-'
        continue
    fi

    for scenario in $(scenarios_for "$rel_dir"); do
        if [ "$LIST_ONLY" = true ]; then
            record_result PLANNED "$(case_name "$rel_dir" "$scenario")" '-'
        else
            run_case "$rel_dir" "$scenario"
        fi
    done
done

echo
echo "Summary: total=${TOTAL}, pass=${PASS}, fail=${FAIL}, skip=${SKIP}"
echo "Result file: ${RESULT_FILE}"

if [ "$FAIL" -ne 0 ]; then
    exit 1
fi
