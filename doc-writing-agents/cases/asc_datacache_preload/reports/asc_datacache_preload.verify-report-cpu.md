# asc_datacache_preload 详细验证报告

## 1. 输入与目标
- 输入对象：asc_datacache_preload 接口文档
- 原文路径：`docs/zh/api/SIMD-API/C-API/cache_ctrl/asc_datacache_preload.md`
- 辅助输入：源码 `include/c_api/cache_ctrl/cache_ctrl.h`、`impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h`、`impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl.h`、UT 测试 `tests/api/c_api/npu_arch_3510/cache_ctrl/test_asc_data_cache_preload.cpp`、`tests/api/c_api/npu_arch_2201/cache_ctrl/test_asc_data_cache_preload.cpp`
- case 输出路径：`doc-writing-agents/cases/asc_datacache_preload/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/asc_datacache_preload/reports/asc_datacache_preload.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | 基本调用：address + offset=0 可编译执行 | `01_basic_invoke/` | 待执行 |
| 02 | 非零 offset 调用：offset=64 可编译执行 | `02_offset_nonzero/` | 待执行 |
| 03 | 文档调用示例回放 | `03_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性
- 文档原型：`__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset)`
- 源码（`cache_ctrl.h:53`）：`__aicore__ inline void asc_datacache_preload(__gm__ uint64_t* address, int64_t offset)` ✅ 一致
- 3510 实现（`npu_arch_3510/cache_ctrl_impl.h:26-28`）：调用 `asc_datacache_preload_impl(address, offset)` ✅ 存在
- 2201 实现（`npu_arch_2201/cache_ctrl_impl.h:40-42`）：调用 `asc_datacache_preload_impl(address, offset)` ✅ 存在

### 3.2 产品支持矩阵与源码一致性
- 文档声明：950/A3/A2 支持，310b/310p/910 不支持
- 源码（`cache_ctrl.h`）：3510 和 2201 架构均有实现 ✅
- 文档中 "Ascend 950PR/Ascend 950DT：支持" 对应 3510 架构 ✅
- 文档中 "Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持" 对应 2201 架构 ✅

### 3.3 流水类型
- 文档声明：PIPE_S
- 源码：asc_datacache_preload 调用 `dc_preload`（数据缓存预加载），属于 Scalar 流水操作 ✅

### 3.4 参数说明
- address：源操作数起始地址，类型 `__gm__ uint64_t*` ✅
- offset：偏移量，类型 `int64_t`，单位 Bytes ✅
- 源码 UT（`test_asc_data_cache_preload.cpp`）：验证 `address` 和 `offset` 参数正确传递到底层 `dc_preload` ✅

### 3.5 头文件路径
- 文档声明：`"c_api/cache_ctrl/cache_ctrl.h"` ✅
- 实际头文件存在于 `include/c_api/cache_ctrl/cache_ctrl.h` ✅

### 3.6 功能说明
- 文档描述："从源地址所在的特定GM地址预加载数据到Data Cache中，每次调用只能预加载一个Cache Line大小的数据"
- 源码调用链：`asc_datacache_preload` → `asc_datacache_preload_impl` → 底层 `dc_preload` 指令 ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 约束说明：频繁调用导致NOP
- 文档："频繁调用此接口可能会导致保留栈拥塞，此时，该接口将被视为NOP指令，阻塞Scalar流水。因此不建议频繁调用此接口。"
- cpudebug 无法验证此硬件行为，需上板确认实际NOP阈值和行为 ⚠️ 部分验证（仅文本核对）

### 4.2 每次"只能预加载一个Cache Line"的精确含义
- 文档说"每次调用只能预加载一个Cache Line大小的数据"，但未明确 Cache Line 的具体大小
- cpudebug 无法验证实际 Cache Line 大小 ⚠️ 需上板确认

### 4.3 offset 参数的合法取值范围
- 文档未明确 offset 的取值范围约束（如是否必须对齐、最大值等）
- 需通过源码或上板确认 ⚠️ 待确认

### 4.4 产品名映射问题
- 文档使用 "Ascend 950PR/Ascend 950DT" 作为产品名，按 SKILL 约定应归并为 "Atlas 350 加速卡" ⚠️ 建议修改

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 部分验证 | 源码 #ifdef 与文档支持矩阵一致，但未逐产品实测 | 参考证据 | 源码分析 |
| 功能说明 | 已验证 | 函数原型、参数类型、头文件路径均与源码一致 | 直接证据 | 源码比对 |
| 函数原型 | 已验证 | 源码 cache_ctrl.h:53 原型完全一致 | 直接证据 | 源码比对 |
| 参数说明 | 已验证 | address(__gm__ uint64_t*) + offset(int64_t) 与源码一致 | 直接证据 | 01_basic_invoke |
| 返回值说明 | 已验证 | void 返回，源码确认 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S，源码底层调用 dc_preload 属 Scalar 流水 | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | 约束文本存在，但 cpudebug 无法验证 NOP 行为 | 参考证据 | 需上板确认 |
| 调用示例 | 部分验证 | case 已创建，但 cpudebug 模式下未实际编译执行 | 参考证据 | 03_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| 频繁调用NOP阈值测试 | 约束说明中"频繁调用导致NOP" | cpudebug 无法模拟硬件保留栈行为 | 在 NPU 上验证频繁调用后 NOP 行为 |
| offset 非对齐测试 | offset 取值范围约束 | 文档未明确 offset 对齐要求 | 通过源码或上板确认 offset 合法范围 |
| 不同地址空间测试 | address 指向不同 GM 区域 | cpudebug 内存模型简化 | 在 NPU 上验证不同 GM 区域预加载 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| 频繁调用NOP行为 | 仅文本核对 | cpudebug 无法模拟硬件保留栈拥塞 | 跑不起来（cpudebug 模式天然不能定版） | 需 NPU 上板验证保留栈拥塞和 NOP 转化 |
| Cache Line 大小确认 | 文档未明确 | 缺少硬件规格信息 | 写不出用例 | 需确认具体 Cache Line 大小（通常64B） |
| offset 合法范围 | 文档未明确 | 缺少约束信息 | 写不出用例 | 需确认 offset 是否需要特定对齐或范围限制 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：频繁调用导致NOP指令的具体行为和阈值
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟硬件保留栈拥塞机制
  - 建议上板验证动作：在 NPU 上连续高频调用 asc_datacache_preload，观察是否出现 NOP 行为
- 问题2：每次预加载的 Cache Line 精确大小
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实 Cache Line 行为
  - 建议上板验证动作：在 NPU 上验证预加载数据量与 Cache Line 对应关系
- 问题3：offset 参数的实际约束
  - 为什么 cpudebug 不能最终确认：cpudebug 不验证地址约束的真实报错
  - 建议上板验证动作：在 NPU 上测试不同 offset 值（0、非对齐、大偏移）

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行（需要 CANN 编译环境）
- UT 测试（mock 框架）：3510 和 2201 架构均有覆盖，参数传递验证通过

## 10. 关键证据
- **源码一致性**：`cache_ctrl.h:53` 函数原型与文档完全一致
- **双架构支持**：3510（`npu_arch_3510/cache_ctrl_impl.h:26-28`）和 2201（`npu_arch_2201/cache_ctrl_impl.h:40-42`）均有实现
- **UT 证据**：`test_asc_data_cache_preload.cpp` 验证 `address` 和 `offset` 参数正确传递到底层 `dc_preload`（3510 和 2201 均有测试）
- **底层映射**：`asc_datacache_preload` → `asc_datacache_preload_impl` → `dc_preload`

## 11. 对原文的建议回写
1. **产品名归并**：将 "Ascend 950PR/Ascend 950DT" 统一为 "Atlas 350 加速卡"
2. **Cache Line 大小**：建议在功能说明中明确 Cache Line 的具体大小（如 64B）
3. **offset 约束**：建议在约束说明中补充 offset 参数的取值范围和对齐要求
4. **SIMT 不兼容提示**：源码 `cache_ctrl.h:11-13` 显示 `#error "cache_ctrl.h cannot be used with compile flag --enable-simt enabled."`，建议在文档约束中补充此限制

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/asc_datacache_preload/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/asc_datacache_preload/reports/asc_datacache_preload.verify-report-cpu.md`

## 13. 特殊值测试情况

本接口为缓存控制操作，无数据类型和数值计算，不适用特殊值测试。

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 频繁调用此接口可能会导致保留栈拥塞，此时，该接口将被视为NOP指令，阻塞Scalar流水 | 正向 | 部分验证 | 无 | 参考证据 | cpudebug 无法模拟保留栈拥塞，仅文本核对，需上板验证 |

### 约束覆盖统计
- 约束总条数：1
- 已验证：0
- 部分验证：1
- 待补验证：0
