#!/bin/bash
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
rm -rf "$SCRIPT_DIR/build"
mkdir -p "$SCRIPT_DIR/build" && cd "$SCRIPT_DIR/build"
cmake .. && make -j
./asc_unpack_example