#!/bin/bash
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------

# Mmad（Cube 矩阵乘）性能测试脚本
# 支持通过命令行参数指定场景编号
# 提取 PipeUtilization.csv 的 aic_cube_time(us)（Cube 计算耗时），折算 Cycle 写入 CSV
# 性能（MAC/cycle）与算力利用率由 generate_roofline.py 基于硬件并行度从 CSV 推导

set -euo pipefail

# 显示帮助信息
show_help() {
    echo "Mmad 性能测试脚本"
    echo ""
    echo "使用方法:"
    echo "  $0 <SCENARIO_NUM> [PLATFORM]"
    echo ""
    echo "参数:"
    echo "  SCENARIO_NUM  测试场景编号（必选）"
    echo "  PLATFORM      平台架构（可选，默认按场景号推导）"
    echo ""
    echo "Atlas A3/A2 训练/推理平台场景 (dav-2201, 主频1800MHz):"
    echo "  1: Mmad b8  (int8 * int8 -> int32)"
    echo "  2: Mmad b16 (half * half -> float)"
    echo "  3: Mmad b32 (float * float -> float)"
    echo "  4: MmadWithSparse b8 (int8 * int8 -> int32, 4:2 结构化稀疏)"
    echo ""
    echo "Ascend 950PR/950DT 平台场景 (dav-3510, 主频1650MHz):"
    echo "  11: Mmad b8  (int8 * int8 -> int32)"
    echo "  12: Mmad b16 (half * half -> float)"
    echo "  13: Mmad b32 (float * float -> float)"
    echo "  14: MmadMx mxfp8 (fp8_e4m3fn * fp8_e4m3fn -> float, scale fp8_e8m0)"
    echo "  15: MmadMx mxfp4 (fp4x2_e2m1 * fp4x2_e2m1 -> float, scale fp8_e8m0)"
    echo ""
    echo "示例:"
    echo "  $0 1              # 测试场景1，默认平台dav-2201"
    echo "  $0 11 dav-3510    # 测试场景11，指定平台dav-3510"
    echo ""
}

# 解析命令行参数
if [ $# -lt 1 ] || [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    show_help
    exit 0
fi

SCENARIO=$1

# 验证场景编号为数字
if ! [[ "$SCENARIO" =~ ^[0-9]+$ ]]; then
    echo "错误: SCENARIO 必须是数字"
    show_help
    exit 1
fi

# 根据场景确定平台和主频
if [ "$SCENARIO" -ge 1 ] && [ "$SCENARIO" -le 4 ]; then
    PLATFORM=${2:-dav-2201}
    if [ "$PLATFORM" != "dav-2201" ]; then
        echo "错误: 场景 ${SCENARIO} 仅支持 dav-2201，当前平台为 ${PLATFORM}"
        exit 1
    fi
    FREQUENCY=1800  # Atlas A3/A2 训练/推理平台主频 1800 MHz
    echo "场景 $SCENARIO 属于 Atlas A3/A2 训练/推理平台 (dav-2201)"
    echo "主频: ${FREQUENCY} MHz"
elif [ "$SCENARIO" -ge 11 ] && [ "$SCENARIO" -le 15 ]; then
    PLATFORM=${2:-dav-3510}
    if [ "$PLATFORM" != "dav-3510" ]; then
        echo "错误: 场景 ${SCENARIO} 仅支持 dav-3510，当前平台为 ${PLATFORM}"
        exit 1
    fi
    FREQUENCY=1650  # Ascend 950PR/950DT 平台主频 1650 MHz
    echo "场景 $SCENARIO 属于 Ascend 950PR/950DT 平台 (dav-3510)"
    echo "主频: ${FREQUENCY} MHz"
else
    echo "错误: 无效的场景编号 $SCENARIO"
    echo "有效范围: 1-4 (Atlas A3/A2 训练/推理平台) 或 11-15 (Ascend 950PR/950DT 平台)"
    exit 1
fi

# Shape 配置数组（按场景精度选择，见方案设计 §4.2）
# 统一固定 M=N，逐步增大 K 直至 L0A/L0B 满载（不同精度满载点不同）
case "$SCENARIO" in
    1)
        # b8 dav-2201：L0A/L0B 在 K=512 时满载（M=N=128）
        SHAPES=("32 32 32" "64 64 64" "128 128 128" "128 256 128" "128 512 128")
        ;;
    11)
        # b8 dav-3510：末组 256 256 256 同时满载 L0A(64KB)/L0C(256KB)
        SHAPES=("32 32 32" "64 64 64" "128 128 128" "128 256 128" "256 256 256")
        ;;
    2)
        # b16 dav-2201：L0A/L0B 在 K=256 时满载（M=N=128）
        SHAPES=("32 32 32" "64 64 64" "64 128 64" "128 128 128" "128 256 128")
        ;;
    12)
        # b16 dav-3510：末组 256 128 256 同时满载 L0A/L0B/L0C
        SHAPES=("32 32 32" "64 64 64" "64 128 64" "128 128 128" "256 128 256")
        ;;
    3|13)
        # b32：L0A 在 M·K=16384 时满载（128 128 128），MN 撑不到 256，两架构均保 4 组
        SHAPES=("32 32 32" "64 64 64" "64 128 64" "128 128 128")
        ;;
    4)
        # sparse b8 dav-2201：K 取 64 倍数，L0A/L0B 在 K=512 时满载
        SHAPES=("64 64 64" "64 128 64" "128 128 128" "128 256 128" "128 512 128")
        ;;
    14)
        # mxfp8 dav-3510：K 取 64 倍数，末组 256 256 256 满载 L0A/L0C
        SHAPES=("64 64 64" "64 128 64" "128 128 128" "128 256 128" "256 256 256")
        ;;
    15)
        # mxfp4 dav-3510：fp4 打包，K 取 64 倍数，末组 256 512 256 同时满载 L0A/L0B/L0C
        SHAPES=("64 64 64" "128 128 128" "128 256 128" "128 512 128" "256 512 256")
        ;;
esac

# 生成时间戳，创建唯一输出目录
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
PERF_DATA_DIR="perf_data_${TIMESTAMP}_scenario${SCENARIO}"
RESULT_CSV="${PERF_DATA_DIR}/perf_result_scenario${SCENARIO}.csv"

# 颜色输出
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}Mmad 性能测试 - 场景${SCENARIO}${NC}"
echo -e "${GREEN}平台: ${PLATFORM}${NC}"
echo -e "${GREEN}========================================${NC}"

# 创建性能数据输出目录
echo -e "${GREEN}创建性能数据目录: ${PERF_DATA_DIR}${NC}"
mkdir -p "${PERF_DATA_DIR}"

# 检查编译产物，不存在则按当前平台编译
if [ ! -f "build/demo" ]; then
    echo -e "${YELLOW}未找到可执行文件，开始编译...${NC}"
    mkdir -p build && cd build
    cmake -DCMAKE_ASC_ARCHITECTURES="${PLATFORM}" \
          -DASC_DIR="${ASC_DIR:-${ASCEND_HOME_PATH}/lib64/cmake}" ..
    make -j
    cd ..
fi

# 初始化 CSV 文件
# 列说明：
# - Test_ID: 测试编号
# - M, K, N: 矩阵维度
# - Shape: 矩阵规格字符串
# - AIC_Cube_Time(us): Cube 计算耗时（aic_cube_time）
# - Cycle: 折算 Cycle 数（Time * Frequency）
# 性能与算力利用率由 generate_roofline.py 基于硬件并行度从本 CSV 推导，此处不重复计算
echo "Test_ID,M,K,N,Shape,AIC_Cube_Time(us),Cycle" > "${RESULT_CSV}"

# 循环测试不同的 shape
test_id=1
for shape in "${SHAPES[@]}"; do
    read M K N <<< "$shape"
    shape_str="${M}_${K}_${N}"

    echo -e "${YELLOW}----------------------------------------${NC}"
    echo -e "${YELLOW}测试 ${test_id}: Shape [${M}, ${K}, ${N}]${NC}"
    echo -e "${YELLOW}----------------------------------------${NC}"

    # 清理之前的 msprof 输出目录
    rm -rf OPPROF_* 2>/dev/null || true

    # 使用 msprof 采集性能数据。msprof 可能返回非 0，不能被 set -e 提前中断。
    echo -e "${GREEN}开始 msprof 性能采集...${NC}"
    set +e
    msprof op build/demo "${SCENARIO}" "${M}" "${K}" "${N}" > /dev/null 2>&1
    msprof_exit_code=$?
    set -e
    if [ "${msprof_exit_code}" -ne 0 ]; then
        echo -e "${RED}msprof 执行失败（退出码: ${msprof_exit_code}）${NC}"
        echo "${test_id},${M},${K},${N},${shape_str},ERROR,ERROR,ERROR" >> "${RESULT_CSV}"
        test_id=$((test_id + 1))
        continue
    fi

    # 查找 msprof 生成的性能数据目录
    msprof_dir=$(ls -dt OPPROF_* 2>/dev/null | head -n 1 || true)

    if [ -z "$msprof_dir" ] || [ ! -d "$msprof_dir" ]; then
        echo -e "${RED}未找到 msprof 输出目录 OPPROF_*${NC}"
        echo "${test_id},${M},${K},${N},${shape_str},N/A,N/A,N/A" >> "${RESULT_CSV}"
        test_id=$((test_id + 1))
        continue
    fi

    echo -e "${GREEN}msprof 输出目录: ${msprof_dir}${NC}"

    # 从 PipeUtilization.csv 提取 aic_cube_time(us)（Cube 计算耗时）
    cube_time="N/A"

    if [ -f "${msprof_dir}/PipeUtilization.csv" ]; then
        echo -e "${GREEN}找到 PipeUtilization.csv${NC}"
        echo -e "${YELLOW}PipeUtilization.csv 内容:${NC}"
        head -n 2 "${msprof_dir}/PipeUtilization.csv"

        # 确定 cube 行类型（可能为 cube0 / cube）
        cpu_type="cube"
        if grep -q "cube0" "${msprof_dir}/PipeUtilization.csv"; then
            cpu_type="cube0"
        elif grep -q "cube" "${msprof_dir}/PipeUtilization.csv"; then
            cpu_type="cube"
        fi

        # 方法1：动态查找 aic_cube_time 列索引（兼容带/不带单位）
        metric_col_index=$(awk -F ',' 'NR==1 {for(i=1;i<=NF;i++) if($i=="aic_cube_time(us)" || $i=="aic_cube_time") print i}' "${msprof_dir}/PipeUtilization.csv" | head -n 1)

        if [ ! -z "$metric_col_index" ]; then
            cube_time=$(awk -F ',' -v col="$metric_col_index" -v type="$cpu_type" '$2==type {print $col}' "${msprof_dir}/PipeUtilization.csv" | sed 's/[[:space:]]//g')
            echo -e "${GREEN}动态搜索列名: aic_cube_time 列索引=${metric_col_index}${NC}"
        fi

        # 方法2备用：动态搜索失败时，从数据行提取首个合法数值兜底
        if [ -z "$cube_time" ] || [ "$cube_time" = "NA" ]; then
            echo -e "${YELLOW}动态搜索失败，尝试备用方法...${NC}"
            cube_time=$(awk -F ',' 'NR==2 {for(i=1;i<=NF;i++) if($i ~ /^[0-9.]+$/) print $i}' "${msprof_dir}/PipeUtilization.csv" | grep -E '^[0-9.]+$' | head -n 1 | sed 's/[[:space:]]//g' || true)
            if [ ! -z "$cube_time" ] && [ "$cube_time" != "NA" ]; then
                echo -e "${GREEN}备用方法提取成功: ${cube_time} us${NC}"
            else
                echo -e "${RED}所有方法均失败${NC}"
            fi
        fi

        echo -e "${GREEN}从 PipeUtilization.csv 提取: aic_cube_time = ${cube_time} us${NC}"
    else
        echo -e "${YELLOW}警告: 未找到 PipeUtilization.csv${NC}"
        ls -la "${msprof_dir}/" || true
    fi

    # 验证提取的数据是否为有效数值
    if [ "$cube_time" = "N/A" ] || [ -z "$cube_time" ] || [ "$cube_time" = "NA" ]; then
        echo -e "${RED}未能提取有效的性能数据${NC}"
        cube_time="N/A"
    elif ! [[ "$cube_time" =~ ^[0-9.]+$ ]]; then
        echo -e "${YELLOW}警告: 提取的数据格式异常: ${cube_time}${NC}"
        cube_time=$(echo "$cube_time" | grep -oE '[0-9.]+' | head -n 1 || true)
        if [ -z "$cube_time" ]; then
            cube_time="N/A"
        fi
    fi

    # 计算派生指标
    cycle_count="N/A"

    if [ "$cube_time" != "N/A" ] && [[ "$cube_time" =~ ^[0-9.]+$ ]]; then
        # Cycle = Time(us) * Frequency(MHz)
        cycle_count=$(awk "BEGIN {printf \"%.2f\", ${cube_time} * ${FREQUENCY}}")

        echo -e "${GREEN}性能指标计算:${NC}"
        echo -e "${GREEN}  Cube 计算耗时: ${cube_time} us${NC}"
        echo -e "${GREEN}  Cycle数: ${cycle_count} cycles${NC}"
        echo -e "${GREEN}  计算量: ${M}*${N}*${K} MAC（性能由 generate_roofline.py 推导）${NC}"
    fi

    # 记录结果到 CSV
    echo "${test_id},${M},${K},${N},${shape_str},${cube_time},${cycle_count}" >> "${RESULT_CSV}"

    echo -e "${GREEN}测试 ${test_id} 完成${NC}"
    echo -e "${GREEN}  Shape: [${M}, ${K}, ${N}]${NC}"
    echo -e "${GREEN}  AIC_Cube_Time: ${cube_time} us${NC}"
    echo -e "${GREEN}  Cycle: ${cycle_count} cycles${NC}"

    # 归档本轮 msprof 输出目录
    if [ "$cube_time" != "N/A" ]; then
        mv "${msprof_dir}" "${PERF_DATA_DIR}/test_${test_id}_${shape_str}" 2>/dev/null || true
    fi

    test_id=$((test_id + 1))
done

# 输出汇总结果
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}性能测试汇总结果${NC}"
echo -e "${GREEN}========================================${NC}"
cat "${RESULT_CSV}"

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}测试完成！${NC}"
echo -e "${GREEN}场景: ${SCENARIO}${NC}"
echo -e "${GREEN}平台: ${PLATFORM}${NC}"
echo -e "${GREEN}结果文件: ${RESULT_CSV}${NC}"
echo -e "${GREEN}性能数据目录: ${PERF_DATA_DIR}${NC}"
echo -e "${GREEN}========================================${NC}"

# 表格格式展示
echo -e "\n${YELLOW}性能数据表格:${NC}"
if command -v column >/dev/null 2>&1; then
    column -t -s ',' "${RESULT_CSV}"
else
    cat "${RESULT_CSV}"
fi
