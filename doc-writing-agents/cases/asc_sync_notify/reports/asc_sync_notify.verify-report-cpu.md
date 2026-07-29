# asc_sync_notify 详细验证报告

## 1. 输入与目标

- **API 名称**: `asc_sync_notify`
- **文档路径**: `docs/zh/api/SIMD-API/C-API/sync/asc_sync_notify.md`
- **原型**: `__aicore__ inline void asc_sync_notify(pipe_t pipe, pipe_t tpipe, event_t id)`
- **头文件**: `"c_api/sync/sync.h"`
- **所属管线**: PIPE_S
- **产品支持**: Ascend 950PR/Ascend 950DT（应修正为"Atlas 350 加速卡"），支持 3510 + 2201 架构
- **验证目标**: 确认 API 声明、行为、约束与源码一致

## 2. case 总览

| case | 路径 | 说明 |
|------|------|------|
| 01_basic_invoke | `asc_sync_notify/01_basic_invoke/` | 基本调用 `asc_sync_notify(PIPE_MTE3, PIPE_S, 0)` |
| 02_sample_replay | `asc_sync_notify/02_sample_replay/` | 复现文档示例代码 |

## 3. 已验证成立的内容

- [x] 函数签名 `(pipe_t pipe, pipe_t tpipe, event_t id)` 与文档声明一致
- [x] 头文件路径 `"c_api/sync/sync.h"` 正确
- [x] 产品支持范围 950+A3+A2（3510+2201）与文档一致
- [x] 必须与 `asc_sync_wait` 配对使用的约束在源码层面成立（底层 `set_flag`/`wait_flag` 配对）

## 4. 未验证成立或需降级表述的内容

- **[关键发现] 文档声明 `__aicore__ inline` 但源码为 MACRO**: 源码中 `asc_sync_notify` 是宏定义，展开为 `set_flag(pipe, tpipe, id)`，并非 inline 函数。文档的函数原型描述与实现不一致
- **实际同步行为**: cpudebug 模式无法验证 `set_flag` 是否真正设置硬件事件标志
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
| pair-with-wait | 01_basic_invoke | 需与 asc_sync_wait 联合验证 |

## 6. 需要补充的测试用例

- **SIMT 编译错误验证**: 在 SIMT 模式下编译此 API，确认 `#error` 触发
- **pipe/tpipe 组合遍历**: 测试不同管线组合（如 PIPE_MTE2→PIPE_S, PIPE_V→PIPE_S 等）
- **event_id 边界值**: 测试 event_id 的有效范围和溢出行为
- **不配对 wait 场景**: 单独调用 notify 不调用 wait，验证是否有资源泄漏

## 7. 当前无法完全验证的具体原因与求助点

- MACRO 展开的类型安全性无法在 cpudebug 中验证
- `set_flag` 的硬件语义（事件编号映射、管线有效性）需要上板确认
- 与 `asc_sync_wait` 的配对时序正确性需要硬件环境验证

## 8. 必须上板运行才能最终确认的问题

1. `set_flag` 是否真正触发硬件事件标志
2. 不同 pipe/tpipe 组合是否都被硬件支持
3. event_id 在 3510 和 2201 上的有效范围差异
4. 不配对使用是否导致死锁或未定义行为

## 9. 编译与运行质量

- cpudebug 编译: 未执行（缺少编译环境）
- 上板编译: 未执行
- case 代码仅作为骨架提供，需在完整编译环境中验证

## 10. 关键证据

- 源码确认: `asc_sync_notify` 是 **MACRO** 而非 inline 函数，展开为 `set_flag(pipe, tpipe, id)`
- 文档声明 `__aicore__ inline` 与实际实现不符，这是一个文档缺陷
- SIMT 不可用: 源码中有 `#error` 守卫

## 11. 对原文的建议回写

1. **[关键] 函数原型描述**: 将 `__aicore__ inline void asc_sync_notify(...)` 修正为宏定义说明，或注明"实现为宏展开"
2. **[产品名称]** "Ascend 950PR/Ascend 950DT" → "Atlas 350 加速卡"
3. **[SIMT 限制]** 需新增说明：此 API 不支持 SIMT 模式
4. **[配对约束]** 建议在参数说明中明确 pipe/tpipe 的有效组合范围

## 12. 复现入口

```
cases/asc_sync_notify/01_basic_invoke/   # 基本调用
cases/asc_sync_notify/02_sample_replay/  # 文档示例复现
```

## 13. 特殊值测试情况

| 特殊值 | 测试结果 | 备注 |
|--------|----------|------|
| pipe=PIPE_MTE3, tpipe=PIPE_S, id=0 | case 01 覆盖 | 默认组合 |
| 其他 pipe/tpipe 组合 | 未测试 | 需补充 |
| event_id 边界值 | 未测试 | 需补充 |

## 14. 约束章节覆盖情况

| 约束项 | 文档声明 | 验证状态 | 备注 |
|--------|----------|----------|------|
| 必须与 asc_sync_wait 配对 | 已声明 | 部分验证 | cpudebug 无法验证时序 |
| 产品支持 | 950+A3+A2 | 已确认 | 产品名称需修正 |
| SIMT 不兼容 | 未声明 | 源码确认存在 | 需补充到文档 |
| 函数类型为 inline | 已声明 | **不成立** | 源码为 MACRO，需修正文档 |
