#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. Please do not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# # 用法：
# #   bash test.sh            # 测试 SCENARIO_NUM 1~8 全部场景
# #   SCENARIO_NUM=3 bash test.sh # 只测试单个场景

# SCENARIO_NUM=6    # 设置场景编号
# mkdir -p build && cd build;                              # 创建并进入build目录
# cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;  # 编译工程
# python3 ../scripts/gen_data.py --scenario $SCENARIO_NUM          # 生成测试输入数据
# ./demo                                                   # 执行编译生成的可执行程序，执行样例
# python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin  # 验证输出结果是否正确

ALL_SCENRIAOS=${SCENARIO_NUM:-"1 2 3 4 5 6 7 8"}
PASS=0
FAIL=0

for c in ${ALL_SCENRIAOS}; do
    echo "========== SCENARIO_NUM=${c} =========="
    rm -rf build
    mkdir -p build && cd build
    cmake .. -DSCENARIO_NUM=${c} > /dev/null 2>&1
    if ! make -j > /dev/null 2>&1; then
        echo "[BUILD] SCENARIO_NUM=${c} FAILED"
        FAIL=$((FAIL + 1))
        cd ..
        continue
    fi
    python3 ../scripts/gen_data.py --scenario ${c}
    ./demo
    if python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin; then
        PASS=$((PASS + 1))
    else
        FAIL=$((FAIL + 1))
    fi
    cd ..
    echo ""
done

echo "=============================="
echo "Summary: PASS=${PASS}, FAIL=${FAIL}"
[ ${FAIL} -eq 0 ] && echo "All cases passed!" || echo "Some cases failed!"
