#!/bin/bash
# 批跑脚本：依次运行 SCENARIO_NUM=1、2、3 三种场景

echo "========================================"
echo "SetAtomicAdd 确定性计算批跑脚本"
echo "========================================"

# 场景配置
SCENARIOS=(1 2 3)
SCENARIO_NAMES=("单个AIV核内" "多个AIV核间" "多个AIC核间")

# 清理之前的构建目录
rm -rf build

# 依次运行每个场景
for idx in "${!SCENARIOS[@]}"; do
    SCENARIO_NUM=${SCENARIOS[$idx]}
    SCENARIO_NAME=${SCENARIO_NAMES[$idx]}
    
    echo ""
    echo "----------------------------------------"
    echo "正在运行场景 ${SCENARIO_NUM}: ${SCENARIO_NAME}"
    echo "----------------------------------------"
    
    # 创建并进入build目录
    mkdir -p build && cd build
    
    # 编译工程
    echo "编译中..."
    cmake .. -DSCENARIO_NUM=${SCENARIO_NUM}
    if [ $? -ne 0 ]; then
        echo "场景 ${SCENARIO_NUM} 编译失败！"
        cd ..
        continue
    fi
    
    make -j
    if [ $? -ne 0 ]; then
        echo "场景 ${SCENARIO_NUM} 编译失败！"
        cd ..
        continue
    fi
    
    # 生成测试数据
    echo "生成测试数据..."
    python3 ../scripts/gen_data.py
    if [ $? -ne 0 ]; then
        echo "场景 ${SCENARIO_NUM} 数据生成失败！"
        cd ..
        continue
    fi
    
    # 执行样例
    echo "执行样例..."
    ./demo
    if [ $? -ne 0 ]; then
        echo "场景 ${SCENARIO_NUM} 执行失败！"
        cd ..
        continue
    fi
    
    # 验证结果
    echo "验证结果..."
    python3 ../scripts/verify_result.py output/output.bin output/golden.bin
    if [ $? -eq 0 ]; then
        echo "场景 ${SCENARIO_NUM}: ${SCENARIO_NAME} - PASS"
    else
        echo "场景 ${SCENARIO_NUM}: ${SCENARIO_NAME} - FAIL"
    fi
    
    # 返回上级目录
    cd ..
    
    # 清理build目录，为下一个场景做准备
    rm -rf build
done

echo ""
echo "========================================"
echo "所有场景执行完毕"
echo "========================================"