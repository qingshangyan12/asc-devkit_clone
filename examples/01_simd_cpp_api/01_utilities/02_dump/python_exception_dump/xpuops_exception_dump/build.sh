#!/bin/bash
# 编译 libxpuops_exception_dump.so（在已安装 CANN 的容器/环境内执行）
set -e
cd "$(dirname "$0")"

if [ -n "$ASCEND_TOOLKIT_HOME" ]; then
    TOOLKIT="$ASCEND_TOOLKIT_HOME"
else
    TOOLKIT="/usr/local/Ascend/ascend-toolkit/latest"
fi
source "$TOOLKIT/set_env.sh" 2>/dev/null || true

rm -rf build && mkdir -p build && cd build
cmake .. && make -j
mkdir -p ../lib
cp -f libxpuops_exception_dump.so ../lib/
echo "build output: $(dirname "$PWD")/lib/libxpuops_exception_dump.so"
