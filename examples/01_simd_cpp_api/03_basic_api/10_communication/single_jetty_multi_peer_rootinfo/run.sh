#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

BYTES="${1:-4096}"
IP_PORT="${2:-tcp://127.0.0.1:8899}"

if [[ ! "${BYTES}" =~ ^[0-9]+$ ]] || ((BYTES == 0)); then
    echo "bytes must be a positive integer." >&2
    exit 1
fi
if ((BYTES % 4 != 0)); then
    echo "bytes must be aligned to sizeof(int32_t)." >&2
    exit 1
fi

cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" \
    -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build "${BUILD_DIR}" -j

PIDS=()
cleanup()
{
    local exit_code=$?
    trap - EXIT INT TERM
    for pid in "${PIDS[@]}"; do
        kill "${pid}" 2>/dev/null || true
    done
    for pid in "${PIDS[@]}"; do
        wait "${pid}" 2>/dev/null || true
    done
    exit "${exit_code}"
}
trap cleanup EXIT INT TERM

for rank in 0 1; do
    device="${rank}"
    "${BUILD_DIR}/demo" \
        2 \
        "${rank}" \
        "${device}" \
        "${BYTES}" \
        "${IP_PORT}" &
    PIDS+=("$!")
done

remaining=${#PIDS[@]}
while ((remaining > 0)); do
    if wait -n; then
        remaining=$((remaining - 1))
    else
        status=$?
        echo "A rank process failed with status ${status}." >&2
        exit "${status}"
    fi
done
PIDS=()

echo "rank 0 and rank 1 run success."
