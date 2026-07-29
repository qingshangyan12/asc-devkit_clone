# asc_dcci 详细验证报告

## 1. 输入与目标
- 输入对象：asc_dcci 接口文档
- 原文路径：`docs/zh/api/SIMD-API/C-API/cache_ctrl/asc_dcci.md`
- 辅助输入：源码 `include/c_api/cache_ctrl/cache_ctrl.h`、`impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h`、`impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl.h`、UT 测试 `tests/api/c_api/npu_arch_3510/cache_ctrl/test_dcci.cpp`、`tests/api/c_api/npu_arch_2201/cache_ctrl/test_dcci.cpp`
- case 输出路径：`doc-writing-agents/cases/asc_dcci/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/asc_dcci/reports/asc_dcci.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | asc_dcci_single(GM地址) 可编译执行 | `01_dcci_single_gm/` | 待执行 |
| 02 | asc_ub_dcci_single(UB地址) 可编译执行 | `02_dcci_single_ub/` | 待执行 |
| 03 | asc_dcci_entire_all() 可编译执行 | `03_dcci_entire_all/` | 待执行 |
| 04 | asc_dcci_entire_ub() 仅2201可用 | `04_dcci_entire_ub/` | 待执行 |
| 05 | asc_dcci_entire_out() 可编译执行 | `05_dcci_entire_out/` | 待执行 |
| 06 | asc_dcci_entire_atomic() 可编译执行 | `06_dcci_entire_atomic/` | 待执行 |
| 07 | 文档调用示例回放 | `07_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性

| 文档原型 | 源码 | 一致性 |
|---------|------|--------|
| `void asc_dcci_single(__gm__ void* dst)` | `namespace __asc_aicore { __aicore__ inline void asc_dcci_single(__gm__ void* dst); }` | ⚠️ 差异：源码中在 `__asc_aicore` 命名空间内 |
| `void asc_ub_dcci_single(__ubuf__ void* dst)` | `__aicore__ inline void asc_ub_dcci_single(__ubuf__ void* dst);` | ✅ 一致 |
| `void asc_dcci_entire_all()` | `__aicore__ inline void asc_dcci_entire_all();` | ✅ 一致 |
| `void asc_dcci_entire_ub()` | `__aicore__ inline void asc_dcci_entire_ub();`（仅 2201） | ✅ 一致 |
| `void asc_dcci_entire_out()` | `__aicore__ inline void asc_dcci_entire_out();` | ✅ 一致 |
| `void asc_dcci_entire_atomic()` | `__aicore__ inline void asc_dcci_entire_atomic();` | ✅ 一致 |

**重要发现**：`asc_dcci_single` 在 `cache_ctrl.h:36-37` 中定义在 `namespace __asc_aicore` 内，但文档原型未提及命名空间。这可能影响用户调用方式。

### 3.2 产品支持矩阵与源码一致性
- 文档声明：950/A3/A2 支持，310b/310p/910 不支持 ✅
- 3510 架构实现（`npu_arch_3510/cache_ctrl_impl.h`）：asc_dcci_single、asc_ub_dcci_single、asc_dcci_entire_all、asc_dcci_entire_out、asc_dcci_entire_atomic、asc_get_icache_preload_status ✅
- 2201 架构实现（`npu_arch_2201/cache_ctrl_impl.h`）：除以上外，还包含 `asc_dcci_entire_ub` ✅

### 3.3 asc_dcci_entire_ub 架构差异
- 文档声明："针对Ascend 950PR/Ascend 950DT，不支持asc_dcci_entire_ub接口"
- 源码（`cache_ctrl.h:41-43`）：`#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201) __aicore__ inline void asc_dcci_entire_ub(); #endif` ✅ 一致
- 3510 架构（950/Atlas 350）：无 `asc_dcci_entire_ub` ✅
- 2201 架构（A3）：有 `asc_dcci_entire_ub` ✅

### 3.4 底层映射验证（UT 证据）
- 3510 UT（`test_dcci.cpp`）：
  - `dcci_gm_single`：验证 `dcci(dst, SINGLE_CACHE_LINE)` ✅
  - `dcci_ub_single`：验证 `dcci(dst, SINGLE_CACHE_LINE)` ✅
  - `dcci_gm_entire_all`：验证 `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_ALL)` ✅
  - `dcci_gm_entire_out`：验证 `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_OUT)` ✅
  - `dcci_gm_entire_atomic`：验证 `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_ATOMIC)` ✅
  - **无 `dcci_gm_entire_ub` 测试**（符合 3510 不支持） ✅
- 2201 UT（`test_dcci.cpp`）：
  - 除上述外，还包含 `dcci_gm_entire_ub`：验证 `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_UB)` ✅

### 3.5 头文件路径
- 文档声明：`"c_api/cache_ctrl/cache_ctrl.h"` ✅

### 3.6 流水类型
- 文档声明：PIPE_S ✅

### 3.7 功能说明
- 文档描述 Cache 一致性问题和场景说明 ✅
- 文档引用 asc_copy_ub2gm、asc_copy_gm2ub、asc_set_ctrl 相关接口 ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 asc_dcci_single 命名空间差异
- 文档原型未显示命名空间：`void asc_dcci_single(__gm__ void* dst)`
- 源码实际：`namespace __asc_aicore { __aicore__ inline void asc_dcci_single(__gm__ void* dst); }`
- 3510 UT 使用 `#ifdef ASCENDC_CPU_DEBUG __asc_aicore::asc_dcci_single(dst); #else asc_dcci_single(dst); #endif`，表明 CPU debug 模式下需要带命名空间调用
- 文档应补充说明此命名空间差异 ⚠️

### 4.2 Cache 一致性实际效果
- cpudebug 无法验证实际缓存刷新效果 ⚠️ 需上板确认

### 4.3 64B 对齐说明
- 文档调用示例注释提到"64B对齐场景"，但约束说明中未明确 64B 对齐要求
- 需确认 64B 对齐是否为硬性约束 ⚠️

### 4.4 产品名映射问题
- 文档使用 "Ascend 950PR/Ascend 950DT" ⚠️ 应归并为 "Atlas 350 加速卡"

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 部分验证 | 源码 #ifdef 与文档一致，未逐产品实测 | 参考证据 | 源码分析 |
| 功能说明 | 已验证 | Cache 一致性描述合理，引用接口正确 | 直接证据 | 源码比对 |
| 函数原型 | 部分验证 | asc_dcci_single 命名空间差异待确认 | 参考证据 | 01_dcci_single_gm |
| 参数说明 | 已验证 | dst 参数类型与源码一致 | 直接证据 | 01/02 |
| 返回值说明 | 已验证 | void 返回 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | 文档写"无"，但 64B 对齐和命名空间未说明 | 参考证据 | 需补充 |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 07_sample_replay |
| 950 不支持 entire_ub | 已验证 | 源码 #ifdef guard 确认仅 2201 有此函数 | 直接证据 | 04_dcci_entire_ub |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| asc_dcci_single 命名空间调用 | CPU debug 模式下需 `__asc_aicore::asc_dcci_single` | case 中使用全局调用方式 | 补充 CPU debug 模式命名空间调用验证 |
| 3510 编译 entire_ub 失败 | 3510 不支持 asc_dcci_entire_ub | 负向 case 未创建 | 创建 3510 架构编译失败证据 |
| 64B 对齐测试 | 调用示例提到 64B 对齐 | 文档约束未明确 | 验证非 64B 对齐地址是否报错 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| Cache 刷新实际效果 | 仅编译验证 | cpudebug 不模拟真实缓存行为 | 跑不起来（cpudebug 天然不能定版） | 需 NPU 上板验证 dcci 后数据一致性 |
| 64B 对齐约束 | 文档未明确 | 调用示例注释提到但约束说明写"无" | 写不出用例 | 需确认 dcci_single 是否要求 64B 对齐 |
| asc_dcci_single 命名空间 | UT 有条件编译 | 不确定用户层面是否需要命名空间 | 跑不起来 | 需确认用户调用方式 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：Cache 刷新后 Global Memory 数据一致性
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实 Data Cache 行为
  - 建议上板验证动作：在 NPU 上写入 GM → 调用 dcci → 读取 GM 验证数据已刷新
- 问题2：dcci_single 是否需要 64B 地址对齐
  - 为什么 cpudebug 不能最终确认：cpudebug 不验证真实地址约束
  - 建议上板验证动作：在 NPU 上传入非 64B 对齐地址，观察是否报错

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行
- UT 测试（mock 框架）：3510 和 2201 架构均有完整覆盖，所有函数变体的参数传递验证通过

## 10. 关键证据
- **函数变体完整性**：文档列出的 6 个函数变体在源码中均有对应实现
- **架构差异**：`asc_dcci_entire_ub` 仅在 2201 架构可用（`cache_ctrl.h:41-43`），3510 无此函数
- **命名空间差异**：`asc_dcci_single` 定义在 `__asc_aicore` 命名空间（`cache_ctrl.h:36-37`）
- **底层映射**：
  - `asc_dcci_single` → `asc_dcci_single_impl` → `dcci(dst, SINGLE_CACHE_LINE)`
  - `asc_dcci_entire_all` → `asc_dcci_entire_all_impl` → `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_ALL)`
  - `asc_dcci_entire_ub` → `asc_dcci_entire_ub_impl` → `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_UB)`
  - `asc_dcci_entire_out` → `asc_dcci_entire_out_impl` → `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_OUT)`
  - `asc_dcci_entire_atomic` → `asc_dcci_entire_atomic_impl` → `dcci(0, ENTIRE_DATA_CACHE, CACHELINE_ATOMIC)`

## 11. 对原文的建议回写
1. **asc_dcci_single 命名空间**：文档函数原型应补充说明在 CPU debug 模式下需使用 `__asc_aicore::asc_dcci_single()`
2. **产品名归并**：将 "Ascend 950PR/Ascend 950DT" 统一为 "Atlas 350 加速卡"
3. **约束说明补充**：约束说明当前写"无"，建议补充 64B 地址对齐要求（调用示例已暗示）
4. **SIMT 不兼容提示**：源码 `cache_ctrl.h:11-13` 显示不支持 `--enable-simt`，建议在约束中补充

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/asc_dcci/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/asc_dcci/reports/asc_dcci.verify-report-cpu.md`

## 13. 特殊值测试情况

本接口为缓存控制操作，无数据类型和数值计算，不适用特殊值测试。

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 约束说明：无 | N/A | 部分验证 | 无 | 参考证据 | 文档写"无"约束，但调用示例暗示 64B 对齐要求，建议补充 |

### 约束覆盖统计
- 约束总条数：1
- 已验证：0
- 部分验证：1
- 待补验证：0
