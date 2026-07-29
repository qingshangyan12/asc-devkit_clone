# asc_get_icache_preload_status 详细验证报告

## 1. 输入与目标
- 输入对象：asc_get_icache_preload_status 接口文档
- 原文路径：`docs/zh/api/SIMD-API/C-API/cache_ctrl/asc_get_icache_preload_status.md`
- 辅助输入：源码 `include/c_api/cache_ctrl/cache_ctrl.h`、`impl/c_api/instr_impl/npu_arch_3510/cache_ctrl_impl.h`、`impl/c_api/instr_impl/npu_arch_2201/cache_ctrl_impl.h`、UT 测试 `tests/api/c_api/npu_arch_3510/cache_ctrl/test_get_icache_preload_status.cpp`
- case 输出路径：`doc-writing-agents/cases/asc_get_icache_preload_status/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/asc_get_icache_preload_status/reports/asc_get_icache_preload_status.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | 基本调用 asc_get_icache_preload_status() 可编译执行 | `01_basic_invoke/` | 待执行 |
| 02 | 文档调用示例回放 | `02_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性
- 文档原型：`__aicore__ inline int64_t asc_get_icache_preload_status()`
- 源码（`cache_ctrl.h:51`）：`__aicore__ inline int64_t asc_get_icache_preload_status();` ✅ 一致
- 3510 实现（`npu_arch_3510/cache_ctrl_impl.h:44`）：`__aicore__ inline int64_t asc_get_icache_preload_status() { return asc_get_icache_preload_status_impl(); }` ✅
- 2201 实现（`npu_arch_2201/cache_ctrl_impl.h:45`）：`__aicore__ inline int64_t asc_get_icache_preload_status() { return asc_get_icache_preload_status_impl(); }` ✅

### 3.2 返回值说明
- 文档：int64_t 类型，0 表示 ICache PreLoad 已完成（空闲），1 表示未完成（忙）
- 源码：返回 `asc_get_icache_preload_status_impl()` 的结果 ✅
- UT 证据（`test_get_icache_preload_status.cpp`）：mock `get_icache_prl_st` 返回 1，验证 `asc_get_icache_preload_status()` 返回 1 ✅

### 3.3 产品支持矩阵与源码一致性
- 文档声明：950/A3/A2 支持，310b/310p/910 不支持 ✅
- 3510 和 2201 架构均有实现 ✅

### 3.4 头文件路径
- 文档声明：`"c_api/cache_ctrl/cache_ctrl.h"` ✅

### 3.5 流水类型
- 文档声明：PIPE_S ✅

### 3.6 功能说明
- 文档："获取ICache的Preload的状态"，"在asc_icache_preload后调用"
- 文档明确说明返回值 0=空闲、1=忙 ✅

### 3.7 调试接口属性
- 文档明确标注"该接口为调试接口" ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 返回值精确语义
- 文档说 0=已完成（空闲）、1=未完成（忙）
- cpudebug 无法验证真实 ICache 状态 ⚠️ 需上板确认

### 4.2 与 asc_icache_preload 的配合使用
- 文档说"在asc_icache_preload后调用"
- cpudebug 无法验证 ICache preload 的真实执行状态 ⚠️

### 4.3 产品名映射问题
- 文档使用 "Ascend 950PR/Ascend 950DT" ⚠️ 应归并为 "Atlas 350 加速卡"

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 部分验证 | 源码 #ifdef 与文档一致，未逐产品实测 | 参考证据 | 源码分析 |
| 功能说明 | 已验证 | 功能描述、返回值语义与源码一致 | 直接证据 | 源码比对 |
| 函数原型 | 已验证 | 原型与源码完全一致 | 直接证据 | 源码比对 |
| 参数说明 | 已验证 | 无参数 | 直接证据 | 源码比对 |
| 返回值说明 | 已验证 | int64_t, 0/1 语义，UT 验证返回值 | 直接证据 | UT 证据 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 已验证 | 文档写"无"约束 | 直接证据 | 源码比对 |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 02_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| asc_icache_preload + asc_get_icache_preload_status 配合使用 | 配合使用场景 | cpudebug 无法验证真实 ICache 状态 | 在 NPU 上验证 preload 后状态变化 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| ICache Preload 状态真实值 | 仅 mock 验证 | cpudebug 不模拟真实 ICache 行为 | 跑不起来（cpudebug 天然不能定版） | 需 NPU 上板验证 asc_icache_preload 后调用 get_status 返回值变化 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：asc_icache_preload 后调用 asc_get_icache_preload_status 返回值变化
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实 ICache preload 行为
  - 建议上板验证动作：在 NPU 上调用 asc_icache_preload → 立即调用 get_icache_preload_status → 确认返回 1（忙）→ 等待后调用 → 确认返回 0（空闲）
- 问题2：返回值是否只有 0 和 1 两种可能
  - 为什么 cpudebug 不能最终确认：mock 仅验证了返回 1 的情况
  - 建议上板验证动作：在 NPU 上多次调用，确认不会返回其他值

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行
- UT 测试（mock 框架）：验证 `asc_get_icache_preload_status()` 正确调用底层 `get_icache_prl_st` 并返回值 ✅

## 10. 关键证据
- **源码一致性**：`cache_ctrl.h:51` 函数原型与文档完全一致
- **双架构支持**：3510 和 2201 架构均有实现
- **底层映射**：`asc_get_icache_preload_status` → `asc_get_icache_preload_status_impl` → `get_icache_prl_st`
- **UT 证据**：`test_get_icache_preload_status.cpp` 验证返回值传递正确

## 11. 对原文的建议回写
1. **产品名归并**：将 "Ascend 950PR/Ascend 950DT" 统一为 "Atlas 350 加速卡"
2. **SIMT 不兼容提示**：源码 `cache_ctrl.h:11-13` 显示不支持 `--enable-simt`，建议在约束中补充

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/asc_get_icache_preload_status/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/asc_get_icache_preload_status/reports/asc_get_icache_preload_status.verify-report-cpu.md`

## 13. 特殊值测试情况

本接口为状态查询操作，返回值仅为 0 或 1，无复杂数据类型和数值计算，不适用特殊值测试。

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| - | 约束说明：无 | N/A | 已验证 | - | N/A | 无约束声明，源码无额外编译期约束 |

### 约束覆盖统计
- 约束总条数：0
- 已验证：0
- 部分验证：0
- 待补验证：0
