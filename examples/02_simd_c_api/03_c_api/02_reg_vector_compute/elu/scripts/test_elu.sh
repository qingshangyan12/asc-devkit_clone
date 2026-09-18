#!/bin/bash
# =============================================================================
# ELU 算子一键测试脚本
# 用法：bash scripts/test_elu.sh
# 功能：编译 → 运行 → 生成测试报告 → 提交到仓库
# =============================================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ELU_DIR="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$ELU_DIR/build"
REPORT_FILE="$ELU_DIR/测试报告.txt"
TIMESTAMP=$(date -u '+%Y-%m-%d %H:%M:%S UTC')

echo "=========================================="
echo " ELU 算子测试 - $TIMESTAMP"
echo "=========================================="

# 清理旧报告
rm -f "$REPORT_FILE"

# 辅助函数：追加输出到报告
record() {
    echo "$@" >> "$REPORT_FILE"
    echo "$@"
}

record "=========================================="
record " ELU 算子测试报告"
record " 生成时间: $TIMESTAMP"
record "=========================================="
record ""

# ---- 1. 环境信息 ----
record "━━━ 1. 环境信息 ━━━"
record ""

record "--- npu-smi info ---"
npu_info=$(npu-smi info 2>&1) || true
record "$npu_info"
record ""

record "--- CANN 版本 ---"
if [ -f /usr/local/Ascend/version.cfg ]; then
    record "$(cat /usr/local/Ascend/version.cfg)"
elif [ -f /usr/local/Ascend/cann-*/version.cfg ]; then
    record "$(cat /usr/local/Ascend/cann-*/version.cfg)"
else
    cann_ver=$(ls -d /usr/local/Ascend/cann-* 2>/dev/null | head -1)
    record "CANN: ${cann_ver:-未找到}"
fi
record ""

record "--- 操作系统 ---"
record "$(uname -a)"
record ""

record "--- 编译工具 ---"
record "cmake: $(cmake --version 2>&1 | head -1)"
record "gcc: $(gcc --version 2>&1 | head -1)"
record "python3: $(python3 --version 2>&1)"
record ""

# ---- 2. 编译 ----
record "━━━ 2. 编译 ━━━"
record ""

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

record "--- cmake ---"
cmake_output=$(cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. 2>&1)
record "$cmake_output"
record ""

record "--- make ---"
make_output=$(make -j 2>&1)
record "$make_output"
record ""

# 检查编译是否成功
if [ -f "$BUILD_DIR/demo" ]; then
    record "✅ 编译成功（demo 可执行文件已生成）"
else
    record "❌ 编译失败（demo 未生成）"
    record "===== 测试终止 ====="
    exit 1
fi
record ""

# ---- 3. 生成测试数据 ----
record "━━━ 3. 生成测试数据 ━━━"
record ""
python3 "$ELU_DIR/scripts/gen_data.py" 2>&1 | tee -a "$REPORT_FILE"
record ""

# ---- 4. 运行测试 ----
record "━━━ 4. 运行测试 ━━━"
record ""
run_output=$(./demo 2>&1)
record "$run_output"
record ""

# 检查运行结果
if echo "$run_output" | grep -q "test pass!"; then
    record "✅ fp32 测试通过"
else
    record "❌ fp32 测试失败"
fi

if echo "$run_output" | grep -q "fp16 test pass!"; then
    record "✅ fp16 测试通过"
else
    record "❌ fp16 测试失败"
fi
record ""

# ---- 5. 精度验证（如有 gen_data.py 输出的对比结果） ----
record "━━━ 5. 精度验证 ━━━"
record ""
# 尝试运行精度对比脚本（如果有）
if [ -f "$ELU_DIR/scripts/verify.py" ]; then
    python3 "$ELU_DIR/scripts/verify.py" 2>&1 | tee -a "$REPORT_FILE"
else
    record "精度对比脚本未提供，依赖运行输出中的 error 信息"
fi
record ""

# ---- 6. 汇总 ----
record "━━━ 6. 测试汇总 ━━━"
record ""
if echo "$run_output" | grep -q "test pass!" && echo "$run_output" | grep -q "fp16 test pass!"; then
    record "✅ 最终结论：全部测试通过"
else
    record "❌ 最终结论：有测试未通过，请检查以上输出"
fi
record ""
record "=========================================="
record " 测试结束时间: $(date -u '+%Y-%m-%d %H:%M:%S UTC')"
record "=========================================="

echo ""
echo "=========================================="
echo " ✅ 测试完成！报告已生成："
echo "    $REPORT_FILE"
echo "=========================================="
echo ""
echo "下一步，远程 Agent 请执行："
echo "  git add examples/02_simd_c_api/03_c_api/02_reg_vector_compute/elu/测试报告.txt"
echo "  git commit -m \"test(elu): 添加测试报告\""
echo "  git push origin elu-dev"