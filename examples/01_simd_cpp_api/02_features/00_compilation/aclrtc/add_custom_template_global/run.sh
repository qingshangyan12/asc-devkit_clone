#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# ----------------------------------------------------------------------------------------------------------
#
# Aclrtc add_custom_template_global 样例编译与运行脚本
# 用法: ./run.sh [A2|A5]

set -e

PLATFORM="${1:-A2}"
case "$PLATFORM" in
    A2) NPU_ARCH="dav-2201" ;;
    A5) NPU_ARCH="dav-3510" ;;
    *)  echo "[ERROR] usage: $0 [A2|A5]   (got '$PLATFORM')"; exit 2 ;;
esac

: "${ASCEND_HOME_DIR:=/usr/local/Ascend/cann}"
source "$ASCEND_HOME_DIR/set_env.sh"

BASE_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
CPP_FILE="add_custom_template_global.cpp"

cd "$BASE_DIR"

# 1. 输入数据 & golden 生成
python3 scripts/gen_data.py

# 2. 编译 Host 程序
g++ -DACLRTC_NPU_ARCH="\"$NPU_ARCH\"" "$CPP_FILE" \
    -I. \
    -I"$ASCEND_HOME_PATH/include" -L"$ASCEND_HOME_PATH/lib64" \
    -lascendcl -lacl_rtc -o main

# 3. 运行核函数（读 input/*.bin，写 output/output.bin）
./main

# 4. 精度校验
python3 scripts/verify_result.py output/output.bin output/golden.bin

# 5. 清理产物
rm -f main
rm -rf input output
