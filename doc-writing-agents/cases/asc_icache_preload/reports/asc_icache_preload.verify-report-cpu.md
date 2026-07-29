# asc_icache_preload 详细验证报告

## 1. 输入与目标
- 输入对象：asc_icache_preload 接口文档
- 原文路径：`docs/zh/api/SIMD-API/C-API/cache_ctrl/asc_icache_preload.md`
- 辅助输入：源码 `include/c_api/cache_ctrl/cache_ctrl.h`、`impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h`、`impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl.h`、UT 测试 `tests/api/c_api/npu_arch_3510/cache_ctrl/test_icache_preload.cpp`
- case 输出路径：`doc-writing-agents/cases/asc_icache_preload/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/asc_icache_preload/reports/asc_icache_preload.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | 基本调用 asc_icache_preload(addr, len) 可编译执行 | `01_basic_invoke/` | 待执行 |
| 02 | prefetch_len 边界值测试（AIC max=15, AIV max=7） | `02_prefetch_len_boundary/` | 待执行 |
| 03 | 文档调用示例回放（含 asc_get_program_counter） | `03_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性
- 文档原型：`__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len)`
- 源码（`cache_ctrl.h:33`）：`__aicore__ inline void asc_icache_preload(const void* addr, int64_t prefetch_len);` ✅ 一致
- 3510 实现（`npu_arch_3510/cache_ctrl_impl.h:21-24`）：调用 `asc_icache_preload_impl(addr, prefetch_len)` ✅
- 2201 实现（`npu_arch_2201/cache_ctrl_impl.h:47-50`）：调用 `asc_icache_preload_impl(addr, prefetch_len)` ✅

### 3.2 参数说明
- addr（输入）：预加载数据的地址，类型 `const void*` ✅
- prefetch_len（输入）：预加载数据的长度，类型 `int64_t`，单位为 2KB ✅

### 3.3 约束说明：prefetch_len 取值范围
- 文档约束：`prefetch_len < ICache大小 / 2K`
  - AIC ICache = 32KB → 最大 prefetch_len = 32K/2K - 1 = 15 ✅
  - AIV ICache = 16KB → 最大 prefetch_len = 16K/2K - 1 = 7 ✅
- 计算验证：32KB / 2KB = 16，所以 prefetch_len < 16，最大值 15 ✅
- 计算验证：16KB / 2KB = 8，所以 prefetch_len < 8，最大值 7 ✅

### 3.4 产品支持矩阵与源码一致性
- 文档声明：950/A3/A2 支持，310b/310p/910 不支持 ✅
- 3510 和 2201 架构均有实现 ✅

### 3.5 头文件路径
- 文档声明：`"c_api/cache_ctrl/cache_ctrl.h"` ✅

### 3.6 流水类型
- 文档声明：PIPE_S ✅

### 3.7 UT 证据
- 3510 UT（`test_icache_preload.cpp`）：验证 `asc_icache_preload(ptr, len)` 正确调用底层 `asc_icache_preload`（mock），参数传递正确 ✅

### 3.8 调用示例中的 asc_get_program_counter
- 文档示例使用 `asc_get_program_counter() & 0xFFFFFFFFFFFF` 获取当前 PC 地址
- 此为合法用法 ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 ICache Preload 实际效果
- cpudebug 无法验证指令预加载到 ICache 的实际效果 ⚠️ 需上板确认

### 4.2 prefetch_len 超出约束时的行为
- 文档要求 prefetch_len < ICache_size / 2K
- 超出时是否报错或产生未定义行为未明确 ⚠️ 需确认

### 4.3 AIC 与 AIV 的 ICache 大小差异
- 文档提到 AIC ICache=32KB、AIV ICache=16KB
- cpudebug 无法验证不同核类型的 ICache 大小差异 ⚠️

### 4.4 产品名映射问题
- 文档使用 "Ascend 950PR/Ascend 950DT" ⚠️ 应归并为 "Atlas 350 加速卡"

### 4.5 调用示例中 prefetch_length=32 的合理性
- 文档调用示例中 `int64_t prefetch_length = 32`
- 若 AIV ICache=16KB，则 max prefetch_len=7，32 远超限制
- 此示例可能仅适用于 AIC 场景，建议文档补充说明 ⚠️

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 部分验证 | 源码 #ifdef 与文档一致，未逐产品实测 | 参考证据 | 源码分析 |
| 功能说明 | 已验证 | 功能描述与源码语义一致 | 直接证据 | 源码比对 |
| 函数原型 | 已验证 | 原型与源码完全一致 | 直接证据 | 源码比对 |
| 参数说明 | 已验证 | addr(const void*) + prefetch_len(int64_t) 一致 | 直接证据 | 源码比对 |
| 返回值说明 | 已验证 | void 返回 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | prefetch_len 范围计算正确，但超限行为未验证 | 参考证据 | 02_prefetch_len_boundary |
| 调用示例 | 部分验证 | case 已创建，但示例 prefetch_len=32 可能超 AIV 限制 | 参考证据 | 03_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| prefetch_len 超限测试 | prefetch_len >= ICache_size/2K 时的行为 | 需 NPU 环境验证实际报错 | 在 NPU 上测试超限 prefetch_len 是否报错 |
| AIV 场景测试 | AIV ICache=16KB, max=7 | 文档未区分 AIC/AIV 示例 | 在 AIV 核上验证 prefetch_len 边界 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| ICache Preload 实际效果 | 仅文本核对 | cpudebug 不模拟真实 ICache 行为 | 跑不起来（cpudebug 天然不能定版） | 需 NPU 上板验证预加载后指令命中情况 |
| prefetch_len 超限行为 | 仅计算验证 | cpudebug 不验证参数越界 | 跑不起来 | 需 NPU 测试超限 prefetch_len 的报错或异常 |
| AIC/AIV ICache 大小差异 | 仅文本核对 | cpudebug 不区分核类型 | 跑不起来 | 需确认当前 NPU 环境是 AIC 还是 AIV |

## 8. 必须上板运行才能最终确认的问题
- 问题1：asc_icache_preload 后指令是否真正加载到 ICache
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实 ICache 行为
  - 建议上板验证动作：在 NPU 上调用 asc_icache_preload 后执行目标代码段，观察 ICache 命中率
- 问题2：prefetch_len 超出约束时的行为
  - 为什么 cpudebug 不能最终确认：cpudebug 不验证参数越界的实际后果
  - 建议上板验证动作：在 NPU 上传入 prefetch_len=16（AIC 超限）或 8（AIV 超限），观察是否报错
- 问题3：调用示例 prefetch_length=32 在 AIV 上的适用性
  - 为什么 cpudebug 不能最终确认：需区分 AIC/AIV 核类型
  - 建议上板验证动作：在 AIV 核上运行文档示例，确认是否异常

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行
- UT 测试（mock 框架）：3510 架构有测试，验证参数正确传递 ✅

## 10. 关键证据
- **源码一致性**：`cache_ctrl.h:33` 函数原型与文档完全一致
- **双架构支持**：3510 和 2201 架构均有实现
- **底层映射**：`asc_icache_preload` → `asc_icache_preload_impl` → 底层 icache preload 指令
- **约束计算验证**：AIC max=15, AIV max=7，数学正确
- **UT 证据**：`test_icache_preload.cpp` 验证参数传递正确

## 11. 对原文的建议回写
1. **产品名归并**：将 "Ascend 950PR/Ascend 950DT" 统一为 "Atlas 350 加速卡"
2. **调用示例补充**：示例中 `prefetch_length = 32` 仅适用于 AIC（32KB ICache），建议补充 AIV 场景的示例或标注适用核类型
3. **约束补充**：建议在约束说明中补充 prefetch_len 超限时的行为（报错/未定义/静默截断）
4. **SIMT 不兼容提示**：源码 `cache_ctrl.h:11-13` 显示不支持 `--enable-simt`，建议在约束中补充

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/asc_icache_preload/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/asc_icache_preload/reports/asc_icache_preload.verify-report-cpu.md`

## 13. 特殊值测试情况

本接口为缓存控制操作，参数为地址和长度，无数值计算，不适用特殊值测试。但 prefetch_len 的边界值已在 case 02 中设计覆盖。

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | prefetch_len参数单位为2K Byte，取值需满足 preFetchLen < ICache大小 / 2K | 正向 | 部分验证 | 02_prefetch_len_boundary | 参考证据 | 数学计算验证 AIC max=15, AIV max=7，但超限行为未验证 |
| C1a | AIC ICache=32KB, max prefetch_len=15 | 正向 | 部分验证 | 02_prefetch_len_boundary | 参考证据 | cpudebug 无法验证真实 ICache 大小 |
| C1b | AIV ICache=16KB, max prefetch_len=7 | 正向 | 部分验证 | 02_prefetch_len_boundary | 参考证据 | cpudebug 无法验证真实 ICache 大小 |
| C1c | prefetch_len 超限行为 | 负向 | 待补验证 | 无 | N/A | 需 NPU 验证超限是否报错 |

### 约束覆盖统计
- 约束总条数：1（展开后 3 子项）
- 已验证：0
- 部分验证：2（C1a, C1b）
- 待补验证：1（C1c）
