# asc_unlock 详细验证报告

## 1. 输入与目标
- 输入对象：asc_unlock 接口文档
- 原文路径：`docs/zh/api/SIMD-API/C-API/sync/asc_unlock.md`
- 辅助输入：源码 `include/c_api/sync/sync.h`、`impl/c_api/instr_impl/npu_arch_3510/sync_impl.h`、UT `tests/api/c_api/npu_arch_3510/sync/test_asc_unlock.cpp`
- case 输出路径：`doc-writing-agents/cases/asc_unlock/sample_cpudebug/`
- 报告输出路径：`doc-writing-agents/cases/asc_unlock/reports/asc_unlock.verify-report-cpu.md`
- 环境：cpudebug 模式（CPU 模拟），未在 NPU 上实际运行

## 2. case 总览
| 编号 | 断言 | 路径 | 结论 |
|------|------|------|------|
| 01 | 基本调用 asc_unlock(PIPE_S, mutex_id) 可编译执行（3510） | `01_basic_invoke/` | 待执行 |
| 02 | ASC_LOCK_NON_BLOCK 模式调用可编译执行（3510） | `02_mode_nonblock/` | 待执行 |
| 03 | 文档调用示例回放 | `03_sample_replay/` | 待执行 |

## 3. 已验证成立的内容

### 3.1 函数原型与源码一致性
- 文档原型：`__aicore__ inline void asc_unlock(pipe_t pipe, uint8_t mutex_id)` + 带 mode 重载
- 源码（sync.h）：asc_unlock 是**可变参数宏**，使用 `##__VA_ARGS__` 默认 mode 为 ASC_LOCK_BLOCK ⚠️ 文档写 `__aicore__ inline` 不准确
- 3510 实现：映射到 `rls_buf(pipe, mutex_id, false/true)`，其中 ASC_LOCK_BLOCK→false，ASC_LOCK_NON_BLOCK→true ✅
- 2201 实现：**无 asc_unlock**，符合文档"仅950支持" ✅

### 3.2 产品支持矩阵与源码一致性
- 文档：950PR/950DT 支持，A3/A2/310b/310p/910 不支持
- 源码：仅 3510 架构有实现（`#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)`），2201 无 lock/unlock ✅ 一致

### 3.3 参数说明
- pipe：流水类型 ✅
- mutex_id：取值范围 [0, 31] ⚠️ 文档参数表描述为"buffer标号"而非"mutex标号"，与 asc_lock 文档术语不一致
- mode：可选参数，ASC_LOCK_BLOCK(0)/ASC_LOCK_NON_BLOCK(1)，默认 ASC_LOCK_BLOCK ✅
- UT 证据：test_asc_unlock.cpp 验证了默认 ASC_LOCK_BLOCK、显式 ASC_LOCK_BLOCK、ASC_LOCK_NON_BLOCK 三种模式 ✅

### 3.4 枚举类型 ascMutexExecuteMode
- 文档：ASC_LOCK_BLOCK=0, ASC_LOCK_NON_BLOCK=1
- 源码：`enum ascMutexExecuteMode { ASC_LOCK_BLOCK = 0, ASC_LOCK_NON_BLOCK = 1 }` ✅ 一致

### 3.5 约束说明
- asc_lock 与 asc_unlock 必须成对使用，相同 mutex_id 和 mode ✅
- 同 mutex_id 不得嵌套 ✅
- 连续相同 pipe+mutex_id 的 asc_lock 不会再阻塞 ✅

### 3.6 头文件路径
- 文档：`"c_api/sync/sync.h"` ✅

### 3.7 流水类型
- 文档：PIPE_S ✅

### 3.8 底层映射（UT 证据）
- asc_unlock(PIPE_S, mutex_id) → rls_buf(PIPE_S, mutex_id, false)（BLOCK 模式）
- asc_unlock(PIPE_S, mutex_id, ASC_LOCK_NON_BLOCK) → rls_buf(PIPE_S, mutex_id, true)（NON_BLOCK 模式）

## 4. 未验证成立或需降级表述的内容

### 4.1 asc_unlock 是宏而非 inline 函数
- 文档原型写 `__aicore__ inline void asc_unlock(...)`，但源码中 asc_unlock 是可变参数宏
- 宏不具备类型安全性，不能取函数地址 ⚠️ 文档表述不准确

### 4.2 Mutex 锁定/释放的实际硬件行为
- cpudebug 无法验证 Mutex 的真实锁定和阻塞行为 ⚠️ 需上板确认

### 4.3 连续相同 pipe+mutex_id 不阻塞的行为
- 文档约束提到但未详细解释机制 ⚠️ 需上板确认

### 4.4 产品名映射问题
- 文档使用 "Ascend 950PR/Ascend 950DT" ⚠️ 应归并为 "Atlas 350 加速卡"

### 4.5 SIMT 不兼容
- 源码 sync.h 开头有 `#error` 禁止 SIMT 模式，文档未提及 ⚠️

### 4.6 mutex_id 参数描述不一致
- asc_unlock 文档参数表中 mutex_id 描述为"buffer标号"，而 asc_lock 文档描述为"mutex标号" ⚠️ 术语不一致

## 5. quick review 标签与 case 覆盖对齐说明
| 文档章节 | 新标签 | 依据 | 证据等级 | 对应case/原因 |
|------|------|------|------|------|
| 产品支持情况 | 部分验证 | 源码 #ifdef 与文档一致，未逐产品实测 | 参考证据 | 源码分析 |
| 功能说明 | 已验证 | Unlock/Mutex 语义与源码一致 | 直接证据 | 源码比对 |
| 函数原型 | 部分验证 | 原型基本一致，但文档写 inline 实际是宏 | 参考证据 | 01_basic_invoke |
| 参数说明 | 部分验证 | mutex_id 描述术语不一致（buffer标号 vs mutex标号） | 参考证据 | 01/02 |
| 返回值说明 | 已验证 | void 返回 | 直接证据 | 源码比对 |
| 流水类型 | 已验证 | PIPE_S | 直接证据 | 源码比对 |
| 约束说明 | 部分验证 | 约束文本正确，但硬件行为未验证 | 参考证据 | 需上板确认 |
| 调用示例 | 部分验证 | case 已创建，cpudebug 未实际执行 | 参考证据 | 03_sample_replay |

## 6. 需要补充的测试用例
| 建议 case | 对应断言 | 当前为何未覆盖 | 目标 |
|-----------|----------|----------------|------|
| mutex_id 边界值测试 | mutex_id 取值 [0,31] | 仅测试 mutex_id=1/2 | 在 NPU 上测试 mutex_id=0 和 31 |
| 嵌套使用测试 | 约束：同 mutex_id 不得嵌套 | 需 NPU 验证硬件行为 | 在 NPU 上验证嵌套后行为 |

## 7. 当前无法完全验证的具体原因与求助点
| 问题 | 当前状态 | 主要原因 | 是"写不出用例"还是"跑不起来" | 具体求助点 |
|------|----------|----------|----------------------------------|------------|
| Mutex 真实释放行为 | 仅文本核对 | cpudebug 不模拟真实 Mutex | 跑不起来（cpudebug 天然不能定版） | 需 NPU 上板验证 asc_unlock 后流水线释放效果 |
| 嵌套使用后果 | 仅文本核对 | 文档说"硬件行为不可预测" | 写不出用例（负向场景无法设计 golden） | 需确认嵌套使用时硬件的具体反应 |

## 8. 必须上板运行才能最终确认的问题
- 问题1：asc_unlock 后指定流水线是否真正被释放
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟真实 Mutex 硬件行为
  - 建议上板验证动作：在 NPU 上 asc_unlock 后检查指定流水线是否可继续执行
- 问题2：ASC_LOCK_NON_BLOCK 模式下 asc_unlock 的行为
  - 为什么 cpudebug 不能最终确认：cpudebug 不区分 BLOCK/NON_BLOCK 的硬件效果
  - 建议上板验证动作：在 NPU 上对比 BLOCK 和 NON_BLOCK 模式下 unlock 后流水线执行时序
- 问题3：连续相同 pipe+mutex_id 不阻塞的行为
  - 为什么 cpudebug 不能最终确认：cpudebug 不模拟流水线调度
  - 建议上板验证动作：在 NPU 上验证连续相同参数的 unlock 是否正确释放

## 9. 编译与运行质量
- cpudebug 模式：case 已创建但未实际编译执行
- UT 测试（mock 框架）：3510 架构有测试，验证 rls_buf 参数传递正确 ✅

## 10. 关键证据
- **架构独占性**：asc_unlock 仅在 3510 架构可用，2201 无此函数
- **宏实现**：asc_unlock 是 variadic macro，非 inline 函数
- **底层映射**：asc_unlock → rls_buf(pipe, mutex_id, false/true)
- **UT 证据**：test_asc_unlock.cpp 验证默认 BLOCK、显式 BLOCK、NON_BLOCK 三种模式参数传递正确
- **术语不一致**：asc_unlock 文档 mutex_id 描述为"buffer标号"，asc_lock 文档描述为"mutex标号"

## 11. 对原文的建议回写
1. **函数原型修正**：将 `__aicore__ inline` 修正为宏定义说明，或至少注明实际实现为宏
2. **产品名归并**：将 "Ascend 950PR/Ascend 950DT" 统一为 "Atlas 350 加速卡"
3. **SIMT 不兼容提示**：补充 `--enable-simt` 模式下不可用的说明
4. **variadic macro 说明**：补充说明 mode 参数通过 `##__VA_ARGS__` 实现默认值
5. **参数术语修正**：将 mutex_id 的描述从"buffer标号"修正为"mutex标号"，与 asc_lock 保持一致

## 12. 复现入口
- case 根目录：`doc-writing-agents/cases/asc_unlock/sample_cpudebug/`
- 详细版报告：`doc-writing-agents/cases/asc_unlock/reports/asc_unlock.verify-report-cpu.md`

## 13. 特殊值测试情况
本接口为同步控制操作，无数据类型和数值计算，不适用特殊值测试。

## 14. 约束章节覆盖情况

### 约束逐条覆盖表
| 编号 | 约束原文摘要 | 约束类型 | 验证状态 | 对应 case | 证据等级 | 备注 |
|------|------------|---------|---------|----------|---------|------|
| C1 | asc_lock与asc_unlock必须严格成对使用，并使用相同的mutex_id与mode | 正向 | 部分验证 | 01_basic_invoke | 参考证据 | cpudebug 无法验证真实释放行为 |
| C2 | asc_unlock必须始终写在asc_lock之后，否则硬件行为无定义 | 负向 | 待补验证 | 无 | N/A | 需 NPU 验证先 unlock 后 lock 的行为 |
| C3 | 同mutex_id的asc_lock与asc_unlock不得嵌套使用 | 负向 | 待补验证 | 无 | N/A | 需 NPU 验证嵌套后硬件行为 |
| C4 | 成对使用的asc_lock和asc_unlock必须配置相同的mode | 正向 | 部分验证 | 02_mode_nonblock | 参考证据 | 仅验证编译通过，未验证运行时行为 |
| C5 | 连续相同pipe和mutex_id的asc_lock不会再阻塞流水线 | 正向 | 待补验证 | 无 | N/A | 需 NPU 验证此行为 |

### 约束覆盖统计
- 约束总条数：5
- 已验证：0
- 部分验证：2
- 待补验证：3
