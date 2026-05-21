#!/bin/bash
set -e
D="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
B="${D}/build"
echo "=== asc_ceil ==="
mkdir -p "${B}"
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -S "${D}" -B "${B}"
make -j -C "${B}"
echo ""
"${B}/c_api_ceil_example"
