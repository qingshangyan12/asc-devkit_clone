# asc_sync_block_wait 详细验证报告

## 1. 输入与目标

- **API 名称**: `asc_sync_block_wait`
- **文档路径**: `docs/zh/api/SIMD-API/C-API/sync/asc_sync_block_wait.md`
- **原型**: 两个重载：
  - `__aicore__ inline void asc_sync_block_wait(pipe_t pipe, uint8_t flag_id)`
  - `__aicore__ inline void asc_sync_block_wait(pipe_t pipe, int64_t flag_id)`
- **头文件**: `"c_api/sync/sync.h"`
- **所属管线**: PIPE_S
- **产品支持**: Ascend 950PR/Ascend 950DT（应修正为"Atlas 350 加速卡"），支持 3510 + 2201 架构
- **验证目标**: 确认 API 声明、行为、跨架构差异、约束与源码一致

## 2. case 总览

| case | 路径 | 说明 |
|------|------|------|
| 01_basic_invoke | `asc_sync_block_wait/01_basic_invoke/` | 基本调用，配对 arrive+wait |
| 02_sample_replay | `asc_sync_block_wait/02_sample_replay/` | 复现文档示例代码 |

## 3. 已验证成立的内容

- [x] 头文件路径 `"c_api/sync/sync.h"` 正确
- [x] 产品支持范围 950+A3+A2（3510+2201）与文档一致
- [x] flag_id 范围 [0, 15] 与文档声明一致
- [x] 源码中存在两个 flag_id 类型重载（uint8_t 和 int64_t）

## 4. 未验证成立或需降级表述的内容

- **[严重发现] 2201 架构下 pipe 参数被静默丢弃**: 源码分析显示：
  - 3510: 映射为 `wait_flag_dev(pipe, flag_id)` — pipe 参数有效
  - 2201: 映射为 `wait_flag_dev(flag_id)` — **pipe 参数被静默忽略，只传 flag_id**
  - 用户面对的签名相同 `(pipe, flag_id)`，但底层实现因架构不同而不同
  - **此差异未在文档中说明**
- **[关键发现] 文档声明 `__aicore__ inline` 但源码为 MACRO**: 源码中 `asc_sync_block_wait` 是宏定义，并非 inline 函数
- **计数器不溢出约束**: 文档声明同一管线同一 flag_id 的计数器不能溢出，cpudebug 无法验证
- **同一 flag_id 同一管线限制**: 文档约束"同一 flag_id 只能在同一管线使用"，需上板验证违反时的后果
- **实际等待行为**: cpudebug 模式无法验证 `wait_flag_dev` 的硬件阻塞语义
- **SIMT 兼容性**: 源码中存在 `#error` 阻止 SIMT 模式编译，但文档未提及此限制
- **产品名称**: 文档中"Ascend 950PR/Ascend 950DT"应更正为"Atlas 350 加速卡"

## 5. quick review 标签与 case 覆盖对齐说明

| 标签 | 对应 case | 覆盖状态 |
|------|-----------|----------|
| basic-invoke | 01_basic_invoke | 已覆盖 |
| sample-replay | 02_sample_replay | 已覆盖 |
| macro-vs-inline | 01_basic_invoke | 已发现（文档写 inline，源码是 MACRO） |
| simt-incompat | 未建 case | 需补充 |
| product-name | 报告层面发现 | 需文档修正 |
| arch-diff-2201-pipe-ignored | 未建 case | **关键发现**，需补充 |
| cross-core-sync | 未建 case | 需多核环境验证 |
| flag-id-boundary | 未建 case | 需补充 |
| counter-overflow | 未建 case | 需补充 |

## 6. 需要补充的测试用例

- **[高优先] 2201 架构 pipe 忽略验证**: 在 2201 设备上测试不同 pipe 参数，验证是否被静默丢弃
- **[高优先] 3510 vs 2201 行为差异**: 构造相同代码在两种架构上运行，对比 pipe 参数是否影响行为
- **SIMT 编译错误验证**: 在 SIMT 模式下编译此 API，确认 `#error` 触发
- **flag_id 边界测试**: 测试 flag_id=0, 15（边界）和 16（溢出）
- **多核 arrive+wait 场景**: 在多核环境中验证配对的跨核同步语义
- **同一 flag_id 多管线使用**: 验证文档约束"同一 flag_id 只能在同一管线使用"违反时的后果
- **计数器溢出场景**: 构造计数器溢出的场景，验证行为

## 7. 当前无法完全验证的具体原因与求助点

- cpudebug 不支持多核模拟，无法验证跨核同步行为
- 2201 架构下 pipe 参数被忽略的实际影响需要在 2201 设备上验证
- `wait_flag_dev` 的内部实现不透明，无法确认 pipe 参数在 3510 上的具体作用
- 两种架构的差异是否会导致用户代码在不同设备上行为不同，需要端到端验证

## 8. 必须上板运行才能最终确认的问题

1. **[严重] 2201 上 pipe 参数被忽略是否影响同步正确性** — 用户可能在不知情下依赖 pipe 参数选择同步目标
2. `wait_flag_dev` 在 3510 上的完整语义（pipe 参数如何影响等待行为）
3. flag_id 超过 15 时的实际行为
4. 同一 flag_id 在不同管线使用是否导致数据损坏
5. 3510 与 2201 上 arrive+wait 配对的完整行为对比
6. 计数器溢出时的硬件行为

## 9. 编译与运行质量

- cpudebug 编译: 未执行（缺少编译环境）
- 上板编译: 未执行
- case 代码仅作为骨架提供，需在完整编译环境中验证

## 10. 关键证据

- **[严重] 跨架构实现差异**:
  - 3510 源码: `wait_flag_dev(pipe, flag_id)` — 2 参数
  - 2201 源码: `wait_flag_dev(flag_id)` — 1 参数，pipe 被静默丢弃
  - 用户侧签名完全相同: `asc_sync_block_wait(pipe, flag_id)`
  - **文档未记录此差异**
- 源码确认: `asc_sync_block_wait` 是 **MACRO** 而非 inline 函数
- SIMT 不可用: 源码中有 `#error` 守卫
- 两个重载: uint8_t flag_id 和 int64_t flag_id 版本共存

## 11. 对原文的建议回写

1. **[严重] 跨架构差异说明**: 必须在文档中新增说明：在 2201 架构下 pipe 参数将被忽略，底层仅使用 flag_id 进行同步。建议提供条件编译示例或运行时检测方法
2. **[关键] 函数原型描述**: 将 `__aicore__ inline void asc_sync_block_wait(...)` 修正为宏定义说明，或注明"实现为宏展开"
3. **[产品名称]** "Ascend 950PR/Ascend 950DT" → "Atlas 350 加速卡"
4. **[SIMT 限制]** 需新增说明：此 API 不支持 SIMT 模式
5. **[flag_id 范围]** 建议明确说明 flag_id 有效范围 [0, 15] 及溢出后果
6. **[同一 flag_id 同一管线]** 建议说明违反此约束的具体后果（硬件异常/未定义行为）
7. **[重载说明]** 建议说明 uint8_t 与 int64_t 两个重载的使用场景差异

## 12. 复现入口

```
cases/asc_sync_block_wait/01_basic_invoke/   # 基本调用（配对 arrive+wait）
cases/asc_sync_block_wait/02_sample_replay/  # 文档示例复现
```

## 13. 特殊值测试情况

| 特殊值 | 测试结果 | 备注 |
|--------|----------|------|
| pipe=PIPE_S, flag_id=0 | case 01 覆盖 | 默认值 |
| flag_id=15 | 未测试 | 边界值，需补充 |
| flag_id=16 | 未测试 | 溢出值，需补充 |
| flag_id=-1 (int64_t) | 未测试 | 负数，需补充 |
| 2201 上不同 pipe 值 | 未测试 | **关键**：验证 pipe 是否被忽略 |
| 3510 上不同 pipe 值 | 未测试 | 对比验证 |

## 14. 约束章节覆盖情况

| 约束项 | 文档声明 | 验证状态 | 备注 |
|--------|----------|----------|------|
| flag_id [0, 15] | 已声明 | 源码确认 | 溢出行为未验证 |
| 计数器不溢出 | 已声明 | 未验证 | 需上板确认 |
| 同一 flag_id 同一管线 | 已声明 | 未验证 | 需上板确认违反后果 |
| 产品支持 | 950+A3+A2 | 已确认 | 产品名称需修正 |
| SIMT 不兼容 | 未声明 | 源码确认存在 | 需补充到文档 |
| 函数类型为 inline | 已声明 | **不成立** | 源码为 MACRO，需修正文档 |
| 2201 pipe 参数被忽略 | **未声明** | **源码确认** | **严重缺失，必须补充** |
