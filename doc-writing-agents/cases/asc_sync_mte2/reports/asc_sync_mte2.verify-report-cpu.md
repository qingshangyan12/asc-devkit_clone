# asc_sync_mte2 详细验证报告

## 1. 输入与目标
- 输入对象：asc_sync_mte2 接口文档
- 原文路径：`docs/zh/api/SIMD-API/C-API/sync/asc_sync_mte2.md`
- 辅助输入：源码 `include/c_api/sync/sync.h`、`impl/c_api/instr_impl/npu_arch_3510/sync_impl.h`、`impl/c_api/instr_impl/npu_arch_2201/sync_impl.h`
- case 输出路径：`doc-writing-agents/cases/asc_sync_mte2/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/asc_sync_mte2/reports/asc_sync_mte2.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | 基本调用 asc_sync_mte2(0) 可编译执行 | `01_basic_invoke/` | 待执行 |
| 02 | 文档调用示例回放（带数据搬运+计算） | `02_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性
- 文档原型：`__aicore__ inline void asc_sync_mte2(int id)` ✅
- 源码（sync.h）：asc_sync_mte2 是 inline 函数，委托到 `pipe_barrier(PIPE_MTE2)` ✅ 一致
- 参数 id 类型：文档为 `int`，源码为 `int` ✅ 一致

### 3.2 产品支持矩阵与源码一致性
- 文档：950PR/950DT + A3 + A2 支持
- 源码：3510 和 2201 架构均有实现 ✅ 一致

### 3.3 功能说明
- 文档：等待 MTE2 流水线操作完成 ✅
- 源码：`pipe_barrier(PIPE_MTE2)` 等待 MTE2 流水线 ✅ 一致

### 3.4 头文件路径
- 文档：`"c_api/sync/sync.h"` ✅

### 3.5 流水类型
- 文档：PIPE_S ✅（接口本身属于 PIPE_S 流水类型）

### 3.6 返回值
- 文档：void ✅

### 3.7 底层映射
- asc_sync_mte2(id) → pipe_barrier(PIPE_MTE2) ✅

## 4. 未验证成立或需降级表述的内容

### 4.1 id 参数取值范围未指定
- 文档约束说"无"，但 id 参数的类型为 int
- 文档未说明 id 的取值范围和含义 ⚠️
- 源码中 id 参数在 pipe_barrier 调用中未直接使用（PIPE_MTE2 为固定值），id 的实际作用不明确 ⚠️

### 4.2 真实同步行为
- cpudebug 无法验证 MTE2 流水线同步的真实硬件行为 ⚠️ 需上板确认

### 4.3 id 参数的实际作用
- 源码中 asc_sync_mte2(id) 内部委托到 pipe_barrier(PIPE_MTE2)，id 参数似乎未被使用
- 如果 id 无实际作用，文档应说明或移除该参数 ⚠️

### 4.4 SIMT 不兼容
- 源码 sync.h 开头有 `#error` 禁止 SIMT 模式，文档未提及 ⚠️

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 已验证 | 源码 3510+2201 均有实现 | 直接证据 | 源码分析 |
| 功能说明 | 已验证 | pipe_barrier(PIPE_MTE2) 语义一致 | 直接证据 | 源码比对 |
| 函数原型 | 已验证 | inline 函数与文档一致 | 直接证据 | 01_basic_invoke |
| 参数说明 | 部分验证 | id 类型一致，但取值范围和实际作用未明确 | 参考证据 | 01_basic_invoke |
| 返回值说明 | 已验证 | void 返回 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | 文档说"无"约束，但 id 取值范围未说明 | 参考证据 | 需补充 |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 02_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| id 参数不同值测试 | id 取值范围和作用 | 需 NPU 验证不同 id 值行为 | 在 NPU 上测试 id=0/1/2 等不同值 |
| MTE2 流水线同步测试 | asc_sync_mte2 是否真正等待 MTE2 完成 | 需 NPU 验证硬件行为 | 在 NPU 上验证 MTE2 同步效果 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| id 参数作用 | 不明确 | 源码中 id 未被 pipe_barrier 使用 | 写不出用例 | 需确认 id 参数的实际作用和取值范围 |
| MTE2 同步真实性 | 仅文本核对 | cpudebug 不模拟流水线调度 | 跑不起来 | 需 NPU 上板验证 MTE2 同步效果 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：asc_sync_mte2(id) 是否真正等待 MTE2 流水线操作完成
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟流水线调度
  - 建议上板验证动作：在 NPU 上验证 asc_sync_mte2 后 MTE2 操作是否完成
- 问题2：id 参数的实际作用
  - 为什么 cpudebug 不能最终确认：源码中 id 未直接使用，可能在硬件层面有作用
  - 建议上板验证动作：在 NPU 上测试不同 id 值的行为差异

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行

## 10. 关键证据
- **inline 函数**：asc_sync_mte2 是真正的 inline 函数（非宏）✅
- **底层映射**：asc_sync_mte2(id) → pipe_barrier(PIPE_MTE2)
- **id 参数疑点**：id 参数在源码中未直接传递到 pipe_barrier，实际作用不明确
- **文档约束缺失**：文档说约束为"无"，但 id 取值范围未说明

## 11. 对原文的建议回写
1. **补充 id 参数说明**：明确 id 的取值范围、含义和实际作用
2. **SIMT 不兼容提示**：补充 `--enable-simt` 模式下不可用的说明
3. **约束补充**：将文档约束从"无"修正为包含 id 取值范围说明
4. **与 asc_sync_pipe 的关系**：补充说明 asc_sync_mte2 相当于 asc_sync_pipe(PIPE_MTE2) 的便捷封装

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/asc_sync_mte2/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/asc_sync_mte2/reports/asc_sync_mte2.verify-report-cpu.md`

## 13. 特殊值测试情况
本接口为同步控制操作，无数据类型和数值计算，不适用特殊值测试。

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | 无（文档约束为空） | N/A | N/A | N/A | N/A | ⚠️ id 参数取值范围未说明，应补充约束 |

### 约束覆盖统计
- 约束总条数：0（文档标注）
- 隐含约束：1（id 取值范围）
- 已验证：0
- 部分验证：0
- 待补验证：1
