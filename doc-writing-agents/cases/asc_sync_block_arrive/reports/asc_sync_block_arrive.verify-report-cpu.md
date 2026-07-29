# asc_sync_block_arrive 详细验证报告

## 1. 输入与目标

- **API 名称**: `asc_sync_block_arrive`
- **文档路径**: `docs/zh/api/SIMD-API/C-API/sync/asc_sync_block_arrive.md`
- **原型**: `__aicore__ inline void asc_sync_block_arrive(pipe_t pipe, int64_t flag_id)`
- **头文件**: `"c_api/sync/sync.h"`
- **所属管线**: PIPE_S
- **产品支持**: Ascend 950PR/Ascend 950DT（应修正为"Atlas 350 加速卡"），支持 3510 + 2201 架构
- **验证目标**: 确认 API 声明、行为、约束与源码一致

## 2. case 总览

| case | 路径 | 说明 |
|------|------|------|
| 01_basic_invoke | `asc_sync_block_arrive/01_basic_invoke/` | 基本调用 `asc_sync_block_arrive(PIPE_S, 0)` |
| 02_sample_replay | `asc_sync_block_arrive/02_sample_replay/` | 复现文档示例代码 |

## 3. 已验证成立的内容

- [x] 函数签名 `(pipe_t pipe, int64_t flag_id)` 与文档声明一致
- [x] 头文件路径 `"c_api/sync/sync.h"` 正确
- [x] 产品支持范围 950+A3+A2（3510+2201）与文档一致
- [x] 源码委托: 展开为 `ffts_cross_core_sync(pipe, GetfftsConfig(flag_id))`，mode=0x02，与跨核同步语义一致

## 4. 未验证成立或需降级表述的内容

- **[关键发现] 文档声明 `__aicore__ inline` 但源码为 MACRO**: 源码中 `asc_sync_block_arrive` 是宏定义，并非 inline 函数
- **内核类型配置约束**: 文档提到内核类型配置限制，但具体规则不清晰，需进一步明确
- **Matmul 不可共用约束**: 文档声明不可与 Matmul 同时使用，cpudebug 无法验证此约束的运行时影响
- **计数器最大值 15**: 文档声明 flag_id 对应计数器最大值为 15，cpudebug 无法验证溢出行为
- **实际跨核同步行为**: cpudebug 模式无法验证 `ffts_cross_core_sync` 的硬件行为
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
| cross-core-sync | 未建 case | 需多核环境验证 |
| counter-overflow | 未建 case | 需补充 |
| matmul-co-use | 未建 case | 需补充 |

## 6. 需要补充的测试用例

- **SIMT 编译错误验证**: 在 SIMT 模式下编译此 API，确认 `#error` 触发
- **flag_id 边界测试**: 测试 flag_id=0, 15（边界）和 16（溢出）的行为
- **多核 arrive+wait 场景**: 在多核环境中验证 arrive/wait 配对的跨核同步语义
- **Matmul 共用场景**: 构造 arrive 与 Matmul 共用的场景，验证是否产生冲突
- **不同 pipe 参数**: 测试 PIPE_S 以外的 pipe 参数是否有效

## 7. 当前无法完全验证的具体原因与求助点

- cpudebug 不支持多核模拟，无法验证跨核同步行为
- `ffts_cross_core_sync` 和 `GetfftsConfig` 的内部实现细节不透明
- 计数器溢出行为需要硬件环境确认
- Matmul 共用约束的运行时表现需要上板验证

## 8. 必须上板运行才能最终确认的问题

1. `ffts_cross_core_sync` 是否真正实现跨核到达同步
2. flag_id 超过 15 时的实际行为（是否硬件异常或静默截断）
3. 与 Matmul 共用是否导致数据损坏或死锁
4. 3510 与 2201 上的跨核同步机制差异
5. GetfftsConfig 的具体映射规则

## 9. 编译与运行质量

- cpudebug 编译: 未执行（缺少编译环境）
- 上板编译: 未执行
- case 代码仅作为骨架提供，需在完整编译环境中验证

## 10. 关键证据

- 源码确认: `asc_sync_block_arrive` 是 **MACRO** 而非 inline 函数
- 底层实现: `ffts_cross_core_sync(pipe, GetfftsConfig(flag_id))`，mode=0x02
- 计数器约束: flag_id 对应计数器最大 15，源码通过 `GetfftsConfig` 进行映射
- SIMT 不可用: 源码中有 `#error` 守卫

## 11. 对原文的建议回写

1. **[关键] 函数原型描述**: 将 `__aicore__ inline void asc_sync_block_arrive(...)` 修正为宏定义说明，或注明"实现为宏展开"
2. **[产品名称]** "Ascend 950PR/Ascend 950DT" → "Atlas 350 加速卡"
3. **[SIMT 限制]** 需新增说明：此 API 不支持 SIMT 模式
4. **[计数器约束]** 建议明确说明 flag_id 的有效范围 [0, 15] 及溢出后果
5. **[Matmul 限制]** 建议在约束中更详细地说明与 Matmul 不可共用的原因和表现
6. **[内核类型配置]** 建议补充内核类型配置的具体要求和限制

## 12. 复现入口

```
cases/asc_sync_block_arrive/01_basic_invoke/   # 基本调用
cases/asc_sync_block_arrive/02_sample_replay/  # 文档示例复现
```

## 13. 特殊值测试情况

| 特殊值 | 测试结果 | 备注 |
|--------|----------|------|
| pipe=PIPE_S, flag_id=0 | case 01 覆盖 | 默认值 |
| flag_id=15 | 未测试 | 边界值，需补充 |
| flag_id=16 | 未测试 | 溢出值，需补充 |
| flag_id=-1 | 未测试 | 负数，语义不明 |
| 非 PIPE_S 的 pipe | 未测试 | 需补充 |

## 14. 约束章节覆盖情况

| 约束项 | 文档声明 | 验证状态 | 备注 |
|--------|----------|----------|------|
| 内核类型配置 | 已声明 | 未验证 | 需上板确认 |
| 不可与 Matmul 共用 | 已声明 | 未验证 | 需上板确认 |
| 计数器最大 15 | 已声明 | 源码确认 | 溢出行为未验证 |
| 产品支持 | 950+A3+A2 | 已确认 | 产品名称需修正 |
| SIMT 不兼容 | 未声明 | 源码确认存在 | 需补充到文档 |
| 函数类型为 inline | 已声明 | **不成立** | 源码为 MACRO，需修正文档 |
