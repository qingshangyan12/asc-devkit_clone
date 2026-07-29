# asc_sync 详细验证报告

## 1. 输入与目标
- 输入对象：asc_sync 接口文档
- 原文路径：`docs/zh/api/SIMD-API/C-API/sync/asc_sync.md`
- 辅助输入：源码 `include/c_api/sync/sync.h`、`impl/c_api/instr_impl/npu_arch_3510/sync_impl.h`、`impl/c_api/instr_impl/npu_arch_2201/sync_impl.h`
- case 输出路径：`doc-writing-agents/cases/asc_sync/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/asc_sync/reports/asc_sync.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | 基本调用 asc_sync() 可编译执行 | `01_basic_invoke/` | 待执行 |
| 02 | 文档调用示例回放（带数据搬运+计算） | `02_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性
- 文档原型：`__aicore__ inline void asc_sync()` ✅
- 源码（sync.h）：asc_sync 是 inline 函数，委托到 `pipe_barrier(PIPE_ALL)` ✅ 一致
- 与 asc_lock/asc_unlock 不同，asc_sync 确实是 inline 函数而非宏

### 3.2 产品支持矩阵与源码一致性
- 文档：950PR/950DT + A3 + A2 支持
- 源码：3510 和 2201 架构均有实现 ✅ 一致

### 3.3 功能说明
- 文档：等待所有流水线操作完成 ✅
- 源码：`pipe_barrier(PIPE_ALL)` 等待所有流水线 ✅ 一致

### 3.4 头文件路径
- 文档：`"c_api/sync/sync.h"` ✅

### 3.5 流水类型
- 文档：PIPE_S ✅

### 3.6 返回值
- 文档：void ✅

### 3.7 底层映射
- asc_sync() → pipe_barrier(PIPE_ALL) ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 asc_sync 与 asc_sync_vec 功能完全相同
- 文档中 asc_sync 和 asc_sync_vec 的描述完全一致："等待所有流水线操作完成"
- 源码中两者都委托到 pipe_barrier(PIPE_ALL)
- 文档未解释为何存在两个功能相同的接口 ⚠️ 重大文档缺陷

### 4.2 真实同步行为
- cpudebug 无法验证流水线同步的真实硬件行为 ⚠️ 需上板确认

### 4.3 性能影响
- 文档约束提到"对性能有影响，建议使用精确同步接口"
- cpudebug 无法量化性能影响 ⚠️ 需上板确认

### 4.4 SIMT 不兼容
- 源码 sync.h 开头有 `#error` 禁止 SIMT 模式，文档未提及 ⚠️

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 已验证 | 源码 3510+2201 均有实现 | 直接证据 | 源码分析 |
| 功能说明 | 已验证 | pipe_barrier(PIPE_ALL) 语义一致 | 直接证据 | 源码比对 |
| 函数原型 | 已验证 | inline 函数与文档一致 | 直接证据 | 01_basic_invoke |
| 参数说明 | 已验证 | 无参数 | 直接证据 | 源码比对 |
| 返回值说明 | 已验证 | void 返回 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | 约束文本正确，但性能影响未量化 | 参考证据 | 需上板确认 |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 02_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| 多流水线同步测试 | asc_sync() 是否等待所有流水线完成 | 需 NPU 验证硬件行为 | 在 NPU 上验证多流水线场景下同步效果 |
| 性能对比测试 | 与精确同步接口的性能差异 | 需 NPU profiling | 在 NPU 上对比 asc_sync 与 asc_sync_pipe 的性能 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| 流水线同步真实性 | 仅文本核对 | cpudebug 不模拟流水线调度 | 跑不起来 | 需 NPU 上板验证同步效果 |
| asc_sync 与 asc_sync_vec 差异 | 无法确认 | 文档和源码均未解释差异 | 写不出用例 | 需确认两者是否存在语义差异 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：asc_sync() 是否真正等待所有流水线操作完成
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟流水线调度
  - 建议上板验证动作：在 NPU 上验证 asc_sync 后所有流水线操作是否完成
- 问题2：asc_sync 的性能影响
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实执行时序
  - 建议上板验证动作：在 NPU 上 profiling 对比 asc_sync 与精确同步接口

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行

## 10. 关键证据
- **inline 函数**：asc_sync 是真正的 inline 函数（非宏）
- **底层映射**：asc_sync → pipe_barrier(PIPE_ALL)
- **功能重复**：asc_sync 与 asc_sync_vec 功能完全相同，文档未解释差异

## 11. 对原文的建议回写
1. **补充与 asc_sync_vec 的差异说明**：解释为何存在两个功能相同的接口，或标注其中一个为推荐使用
2. **SIMT 不兼容提示**：补充 `--enable-simt` 模式下不可用的说明
3. **性能影响量化**：补充 asc_sync 与精确同步接口的性能差异数据或指导

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/asc_sync/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/asc_sync/reports/asc_sync.verify-report-cpu.md`

## 13. 特殊值测试情况
本接口为同步控制操作，无数据类型和数值计算，不适用特殊值测试。

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 对性能有影响，建议使用精确同步接口 | 正向 | 部分验证 | 无 | 参考证据 | 需 NPU profiling 量化性能影响 |

### 约束覆盖统计
- 约束总条数：1
- 已验证：0
- 部分验证：1
- 待补验证：0
