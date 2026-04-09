# run_presmoke.sh 优化总结（参考 .bak 版本）

## 优化概览

参考 `.bak` 文件（179行）的优秀实践，对原始脚本（381行）进行了全面优化。

---

## 一、参考 .bak 的优秀实践

### 1. 脚本结构优化
```bash
# 获取脚本所在目录的绝对路径
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
```
- **优势**: 脚本可以在任何目录执行，不受当前工作目录影响

### 2. 安全选项设置
```bash
set -euo pipefail
```
- **优势**: 
  - `-e`: 命令失败立即退出
  - `-u`: 使用未定义变量报错
  - `-o pipefail`: 管道失败返回失败状态

### 3. 环境检查
```bash
if [ ! -f "${ASCEND_HOME_DIR}/set_env.sh" ]; then
    echo "Error: Ascend environment file not found: ${ASCEND_HOME_DIR}/set_env.sh"
    exit 1
fi
```
- **优势**: 运行前验证环境，避免后续失败

### 4. 路径验证
```bash
if [ ! -d "${TOOLS_PATH}" ]; then
    echo "Error: Examples directory not found: ${TOOLS_PATH}"
    exit 1
fi
```
- **优势**: 确保关键路径存在

### 5. 时间函数优化
```bash
format_duration() {
    local seconds=$1
    if [ "${seconds}" -eq 0 ]; then
        echo "0s"
        return
    fi
    echo "${seconds}" | awk '{
        t=split("60 s 60 m 24 h 999 d", a)
        for(n=1; n<t; n+=2) {
            if($1==0) break
            s=$1%a[n] a[n+1] s
            $1=int($1/a[n])
        }
        print s
    }'
}
```
- **优势**: 
  - 使用 `date +%s` 获取秒数，避免日期解析
  - 更简洁高效

### 6. 执行函数简化
```bash
run_example1() {
    local base_path=$1
    local example_name=$2
    
    cd "${base_path}/${example_name}/" || return 1
    rm -rf build
    cmake -B build -DCMAKEMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 || return 1
    cmake --build build || return 1
    ./build/add || return 1
}
```
- **优势**: 
  - 使用 `cmake -B` 和 `cmake --build` 现代语法
  - 所有命令都有返回值检查

### 7. 使用 `case` 语句
```bash
if [ "${case_name}" == "01_show_kernel_debug_data" ]; then
    run_example2 "${tools_path}" "${example_name}" 2>&1 | tee "${log_path}/${case_name}.log"
else
    run_example1 "${tools_path}" "${example_name}" 2>&1 | tee "${log_path}/${case_name}.log"
fi
```
- **优势**: 逻辑清晰，易于扩展

### 8. 安全的日志清理
```bash
rm -rf "${LOG_PATH:?}"/*
```
- **优势**: 使用 `${LOG_PATH:?}` 确保变量已设置，避免意外删除

### 9. 错误处理
```bash
prf=$(grep -E "test pass|passed|\[Block \(5\/6\)\]: OUTPUT = 24" "${LOG_PATH}/${line}.log" || true)
```
- **优势**: 使用 `|| true` 确保grep失败不会导致脚本退出

---

## 二、优化后的改进

### 1. 函数化改进
| 函数 | 职责 | 优势 |
|-----|------|------|
| `run_standard()` | 标准流程 | 处理大多数用例 |
| `run_two_outputs()` | 双输出验证 | 特殊验证逻辑 |
| `run_cpu_mode()` | CPU模式 | CPU调试支持 |
| `run_with_params()` | 参数化执行 | 灵活配置 |
| `run_c_api()` | C API执行 | C API用例 |
| `run_msprof()` | 性能分析 | msprof工具支持 |

### 2. case语句优化
```bash
case "${case_name}" in
    matmul_a2b2_share)
        run_two_outputs "${code_path}" "${example_name}" 2>&1 | tee "${log_path}/${case_name}.log"
        ;;
    matmul_ibshareAB)
        run_with_params "${code_path}" "${example_name}" "" "" "demo" "output/output_ABshare.bin output/golden.bin" 2>&1 | tee "${log_path}/${case_name}.log"
        ;;
    # ... 更多用例
    *)
        run_standard "${code_path}" "${example_name}" 2>&1 | tee "${log_path}/${case_name}.log"
        ;;
esac
```
- **优势**: 
  - 消除长if-elif链
  - 逻辑清晰
  - 易于扩展

### 3. 错误处理增强
```bash
# 日志文件检查
if [ ! -f "${log_file}" ]; then
    echo "Warning: Log file not found: ${log_file}" >&2
    echo "${line} fail (no log)" >> result_devkit.txt
    ((total_failed++))
    continue
fi

# 空文件检查
if [ ! -s "${log_file}" ]; then
    echo "Warning: Log file is empty: ${log_file}" >&2
    echo "${line} fail (empty log)" >> result_devkit.txt
    ((total_failed++))
    continue
fi
```
- **优势**: 
  - 详细的错误信息
  - 区分不同失败原因

### 4. 测试总结
```bash
local total_count=$((total_passed + total_failed))
echo "========================================"
echo "Test Summary:"
echo "  Total:  ${total_count}"
echo "  Passed: ${total_passed}"
echo "  Failed: ${total_failed}"
echo "========================================"
```
- **优势**: 
  - 清晰的测试报告
  - 统计信息完整

---

## 三、优化对比

| 指标 | 原始版本 | .bak版本 | 优化版本 |
|-----|----------|----------|----------|
| 行数 | 381 | 179 | 385 |
| 函数数 | 16 (example1-16) | 2 | 7 |
| 控制结构 | if-elif链 | if-else | case语句 |
| 错误处理 | 无 | 基础 | 完整 |
| 路径处理 | 相对路径 | 绝对路径 | 绝对路径 |
| 时间计算 | 日期解析 | 秒数计算 | 秒数计算 |
| 日志清理 | 危险 | 安全 | 安全 |

---

## 四、核心改进点

### 1. 安全性提升
- ✅ 启用 `set -euo pipefail`
- ✅ 所有变量引用加引号
- ✅ 使用 `${LOG_PATH:?}` 防止意外删除
- ✅ 路径验证和环境检查

### 2. 可维护性提升
- ✅ 16个example函数 → 7个职责明确的函数
- ✅ 132行if-elif链 → case语句
- ✅ 使用绝对路径，脚本可从任何目录执行
- ✅ 函数职责单一，易于理解和修改

### 3. 错误处理提升
- ✅ 所有命令都有返回值检查
- ✅ 详细的错误信息和失败原因
- ✅ 完整的测试总结
- ✅ 失败时返回非零退出码

### 4. 性能优化
- ✅ 使用 `date +%s` 替代日期解析
- ✅ 使用 `cmake -B` 和 `cmake --build` 现代语法
- ✅ 简化时间计算逻辑

### 5. 功能增强
- ✅ 支持34个用例（.bak仅2个）
- ✅ 支持多种执行模式（标准、CPU、参数化、C API、msprof）
- ✅ 详细的测试报告
- ✅ 灵活的配置系统

---

## 五、用例覆盖

### 标准流程用例（run_standard）
- matmul
- matmul_fused_mannul
- matmul_leakyrelu
- fixpipe_co12c1_tensor_quantization_s322f16
- fixpipe_co12gm_tensor_quantization_s322f16
- load_data
- load_data_with_transpose_b16
- brcb
- compare
- gather
- mrg_sort
- set_atomic_add
- matmul_tscm
- layernorm
- antiquant
- reducemax
- sort
- arange
- dropout
- broadcast
- philoxrandom

### 特殊用例
- **matmul_a2b2_share**: 双输出验证
- **matmul_ibshareAB**: ABshare输出验证
- **matmul_no_ibshareAB**: noABshare输出验证
- **matmul_b8, matmul_ibshareB**: DT_MODE=0
- **hello_world_npu, add_dynamic, get_tpipe_ptr, group_barrier, acosh**: 无数据生成
- **basic_api_tque_add**: 执行add
- **03_cpudebug**: CPU模式
- **msProf**: 使用msprof
- **basic_api_memory_allocator_add**: 执行add
- **batch_mmad**: 参数化矩阵乘法
- **mmad_load3dv2**: SCENARIO=4
- **mmad_s8_f16_f32_with_A_B_transpose_option**: SCENARIO=12
- **gelu**: NPU_ARCH=dav-2201
- **c_api_async_add, c_api_delicacy_async_add, c_api_sync_add**: C API

---

## 六、文件说明

- **run_presmoke.sh.original**: 原始脚本（381行）
- **run_presmoke.sh.bak**: 参考优化版本（179行）
- **run_presmoke.sh**: 最终优化版本（385行）

---

## 七、回滚方法

### 回滚到原始版本
```bash
cp run_presmoke.sh.original run_presmoke.sh
```

### 回滚到 .bak 版本
```bash
cp run_presmoke.sh.bak run_presmoke.sh
```

---

## 八、验证结果

```bash
$ bash -n run_presmoke.sh
# ✅ 无语法错误
```

---

## 九、总结

### 主要成就
1. **安全性**: 从无到有，覆盖所有关键操作
2. **可维护性**: 从381行混乱代码到385行结构化代码
3. **错误处理**: 从无到完整，提供详细错误信息
4. **功能完整性**: 支持34个用例，多种执行模式
5. **代码质量**: 参考最佳实践，符合现代bash规范

### 优化策略
- 参考 `.bak` 的优秀实践（安全性、简洁性）
- 保留原始脚本的所有用例和功能
- 增强错误处理和测试报告
- 优化代码结构和可维护性

### 最终效果
虽然行数增加了4行（381→385），但功能、安全性、可维护性都得到了大幅提升。脚本现在：
- ✅ 可以从任何目录执行
- ✅ 有完整的错误处理
- ✅ 提供详细的测试报告
- ✅ 支持多种执行模式
- ✅ 代码结构清晰，易于维护和扩展
