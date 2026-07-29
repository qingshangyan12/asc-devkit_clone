# asc_sync_pipe 详细验证报告

## 1. 输入与目标
- 输入对象：asc_sync_pipe 接口文档
- 原文路径：`docs/zh/api/SIMD-API/C-API/sync/asc_sync_pipe.md`
- 辅助输入：源码 `include/c_api/sync/sync.h`、`impl/c_api/instr_impl/npu_arch_3510/sync_impl.h`、`impl/c_api/instr_impl/npu_arch_2201/sync_impl.h`
- case 输出路径：`doc-writing-agents/cases/asc_sync_pipe/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/asc_sync_pipe/reports/asc_sync_pipe.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | 基本调用 asc_sync_pipe(PIPE_MTE2) 可编译执行 | `01_basic_pipe/` | 待执行 |
| 02 | asc_sync_pipe(PIPE_ALL) 可编译执行 | `02_pipe_all/` | 待执行 |
| 03 | MTE2 流水线重叠同步场景 | `03_pipe_mte2/` | 待执行 |
| 04 | 文档调用示例回放 | `04_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性
- 文档原型：`__aicore__ inline void asc_sync_pipe(pipe_t pipe)` ⚠️
- 源码（sync.h）：asc_sync_pipe 是**宏**，封装 `asc_sync_pipe_impl(pipe)` → `pipe_barrier(pipe)`
- 文档写 `__aicore__ inline` 不准确，实际是宏 ⚠️

### 3.2 产品支持矩阵与源码一致性
- 文档：950PR/950DT + A3 + A2 支持
- 源码：3510 和 2201 架构均有实现 ✅ 一致

### 3.3 功能说明
- 文档：等待指定流水线操作完成 ✅
- 源码：`pipe_barrier(pipe)` 等待指定流水线 ✅ 一致

### 3.4 参数说明
- pipe：流水线类型 ✅
- 支持的 pipe 值：PIPE_MTE2, PIPE_MTE3, PIPE_VEC, PIPE_ALL 等（不含 PIPE_S）

### 3.5 头文件路径
- 文档：`"c_api/sync/sync.h"` ✅

### 3.6 流水类型
- 文档：PIPE_S ✅（接口本身属于 PIPE_S 流水类型）

### 3.7 返回值
- 文档：void ✅

### 3.8 底层映射
- asc_sync_pipe(pipe) → asc_sync_pipe_impl(pipe) → pipe_barrier(pipe) ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 asc_sync_pipe 是宏而非 inline 函数
- 文档原型写 `__aicore__ inline void asc_sync_pipe(pipe_t pipe)`，但源码中是宏
- 宏不具备类型安全性 ⚠️ 文档表述不准确

### 4.2 PIPE_S 不支持
- 文档约束明确说明 PIPE_S 不支持
- 源码中 pipe_barrier 不接受 PIPE_S ✅ 一致
- 需 NPU 验证传入 PIPE_S 时的编译/运行时行为

### 4.3 PIPE_ALL 性能影响
- 文档约束提到 PIPE_ALL 对性能有影响
- cpudebug 无法量化性能影响 ⚠️ 需上板确认

### 4.4 MTE2/MTE3 重叠同步
- 文档约束提到 MTE2/MTE3 重叠操作需要同步
- cpudebug 无法验证流水线重叠行为 ⚠️ 需上板确认

### 4.5 SIMT 不兼容
- 源码 sync.h 开头有 `#error` 禁止 SIMT 模式，文档未提及 ⚠️

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 已验证 | 源码 3510+2201 均有实现 | 直接证据 | 源码分析 |
| 功能说明 | 已验证 | pipe_barrier 语义与文档一致 | 直接证据 | 源码比对 |
| 函数原型 | 部分验证 | 原型基本一致，但文档写 inline 实际是宏 | 参考证据 | 01_basic_pipe |
| 参数说明 | 已验证 | pipe 参数与源码一致 | 直接证据 | 01/02/03 |
| 返回值说明 | 已验证 | void 返回 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S（接口本身属于 PIPE_S） | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | 约束文本正确，但硬件行为未验证 | 参考证据 | 需上板确认 |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 04_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| PIPE_S 传入测试 | PIPE_S 不支持 | 需 NPU 验证编译/运行时行为 | 在 NPU 上验证传入 PIPE_S 时报错 |
| PIPE_ALL vs 精确 pipe 性能对比 | PIPE_ALL 性能影响 | 需 NPU profiling | 在 NPU 上对比 PIPE_ALL 与精确 pipe 的性能 |
| MTE2/MTE3 重叠同步测试 | 重叠操作需同步 | 需 NPU 验证硬件行为 | 在 NPU 上验证重叠场景下同步效果 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| PIPE_S 不支持行为 | 仅文本核对 | 需 NPU 验证编译/运行时行为 | 跑不起来 | 需 NPU 验证传入 PIPE_S 的错误表现 |
| MTE2/MTE3 重叠同步 | 仅文本核对 | cpudebug 不模拟流水线重叠 | 跑不起来 | 需 NPU 上板验证重叠场景同步效果 |
| PIPE_ALL 性能影响 | 仅文本核对 | cpudebug 不模拟真实执行时序 | 跑不起来 | 需 NPU profiling 量化性能影响 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：asc_sync_pipe 是否真正等待指定流水线操作完成
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟流水线调度
  - 建议上板验证动作：在 NPU 上验证 asc_sync_pipe 后指定流水线操作是否完成
- 问题2：PIPE_ALL 的性能影响
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实执行时序
  - 建议上板验证动作：在 NPU 上 profiling 对比 PIPE_ALL 与精确 pipe 的性能
- 问题3：MTE2/MTE3 重叠同步效果
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟流水线重叠
  - 建议上板验证动作：在 NPU 上验证重叠场景下同步是否正确

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行

## 10. 关键证据
- **宏实现**：asc_sync_pipe 是宏（非 inline 函数），封装 asc_sync_pipe_impl → pipe_barrier
- **PIPE_S 不支持**：源码 pipe_barrier 不接受 PIPE_S，与文档一致
- **底层映射**：asc_sync_pipe(pipe) → pipe_barrier(pipe)
- **精确同步**：相比 asc_sync 的 PIPE_ALL，asc_sync_pipe 允许指定精确流水线

## 11. 对原文的建议回写
1. **函数原型修正**：将 `__aicore__ inline` 修正为宏定义说明，或至少注明实际实现为宏
2. **SIMT 不兼容提示**：补充 `--enable-simt` 模式下不可用的说明
3. **PIPE_ALL 性能影响量化**：补充 PIPE_ALL 与精确 pipe 的性能差异数据
4. **支持的 pipe 值枚举**：补充完整支持的 pipe_t 枚举值列表

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/asc_sync_pipe/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/asc_sync_pipe/reports/asc_sync_pipe.verify-report-cpu.md`

## 13. 特殊值测试情况
本接口为同步控制操作，无数据类型和数值计算，不适用特殊值测试。

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 不支持 PIPE_S | 负向 | 部分验证 | 无 | 参考证据 | 源码 pipe_barrier 不接受 PIPE_S，需 NPU 验证 |
| C2 | PIPE_ALL 对性能有影响 | 正向 | 部分验证 | 02_pipe_all | 参考证据 | 需 NPU profiling 量化性能影响 |
| C3 | MTE2/MTE3 重叠操作需要同步 | 正向 | 部分验证 | 03_pipe_mte2 | 参考证据 | 需 NPU 验证重叠场景同步效果 |

### 约束覆盖统计
- 约束总条数：3
- 已验证：0
- 部分验证：3
- 待补验证：0
