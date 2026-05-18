#!/bin/bash
set -e
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [ -f "${HOME}/Ascend/ascend-toolkit/set_env.sh" ]; then
    source "${HOME}/Ascend/ascend-toolkit/set_env.sh"
fi
P=0; F=0
for d in "$SCRIPT_DIR"/*/; do
    n=$(basename "$d")
    a=$(grep "npu-arch" "$d/CMakeLists.txt" 2>/dev/null | grep -oP 'dav-\d+')
    [ -z "$a" ] && a="dav-3510"
    sl="/home/jinhu/Ascend/cann-9.1.0-beta.1/x86_64-linux/simulator/${a}/lib"
    rm -rf "$d/build" 2>/dev/null; mkdir -p "$d/build"
    cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=${a} -S "$d" -B "$d/build" > /dev/null 2>&1
    make -j -C "$d/build" > /dev/null 2>&1
    exe=$(find "$d/build" -maxdepth 1 -type f -executable | head -1)
    [ -z "$exe" ] && echo "[SKIP] $n" && continue
    (cd "$d/build" && LD_LIBRARY_PATH="${sl}:${LD_LIBRARY_PATH}" "$exe" > /dev/null 2>&1)
    csv="$d/build/${n}.csv"; [ -f "$csv" ] && cp "$csv" "$d/" 2>/dev/null
    grep -q "OK" "$d/${n}.csv" 2>/dev/null && echo "[PASS] $n" && P=$((P+1)) || echo "[FAIL] $n" && F=$((F+1))
done
echo "=== ${P} passed, ${F} failed ==="
