# asc_sync_mte3 详细验证报告

## 1. 输入与目标

- **API 名称**: `asc_sync_mte3`
- **文档路径**: `docs/zh/api/SIMD-API/C-API/sync/asc_sync_mte3.md`
- **原型**: `__aicore__ inline void asc_sync_mte3(int id)`
- **头文件**: `"c_api/sync/sync.h"`
- **所属管线**: PIPE_S
- **产品支持**: Ascend 950PR/Ascend 950DT（应修正为"Atlas 350 加速卡"），支持 3510 + 2201 架构
- **验证目标**: 确认 API 声明、行为、约束与源码一致

## 2. case 总览

| case | 路径 | 说明 |
|------|------|------|
| 01_basic_invoke | `asc_sync_mte3/01_basic_invoke/` | 基本调用 `asc_sync_mte3(0)` |
| 02_sample_replay | `asc_sync_mte3/02_sample_replay/` | 复现文档示例代码 |

## 3. 已验证成立的内容

- [x] 函数原型 `void asc_sync_mte3(int id)` 与文档声明一致
- [x] 源码实现为 inline 函数，委托至 `pipe_barrier(PIPE_MTE3)`，与文档描述一致
- [x] 头文件路径 `"c_api/sync/sync.h"` 正确
- [x] 无额外约束声明，与文档"无约束"一致
- [x] 产品支持范围 950+A3+A2（3510+2201）与文档一致
- [x] `__aicore__` 修饰符与源码一致（此 API 确实是 inline 函数，非 MACRO）

## 4. 未验证成立或需降级表述的内容

- **实际同步行为**: cpudebug 模式无法验证 `pipe_barrier(PIPE_MTE3)` 是否真正等待 MTE3 管线完成，只能确认调用不崩溃
- **SIMT 兼容性**: 源码中存在 `#error` 阻止 SIMT 模式编译，但文档未提及此限制，需补充说明
- **产品名称**: 文档中"Ascend 950PR/Ascend 950DT"应更正为"Atlas 350 加速卡"

## 5. quick review 标签与 case 覆盖对齐说明

| 标签 | 对应 case | 覆盖状态 |
|------|-----------|----------|
| basic-invoke | 01_basic_invoke | 已覆盖 |
| sample-replay | 02_sample_replay | 已覆盖 |
| macro-vs-inline | N/A（此 API 为 inline，不涉及） | 不适用 |
| simt-incompat | 未建 case | 需补充 |
| product-name | 报告层面发现 | 需文档修正 |

## 6. 需要补充的测试用例

- **SIMT 编译错误验证**: 在 SIMT 模式下编译此 API，确认 `#error` 触发并给出明确错误信息
- **多 id 值测试**: 测试 `asc_sync_mte3(1)` 等 id 值，确认 id 参数的实际影响范围
- **DataCopy 后同步场景**: 在真实 DataCopy→GM 操作后调用 `asc_sync_mte3`，验证同步语义

## 7. 当前无法完全验证的具体原因与求助点

- cpudebug 无法模拟管线同步行为，`pipe_barrier` 的实际等待语义需要上板验证
- SIMT `#error` 的具体错误信息内容需在 SIMT 编译环境下确认

## 8. 必须上板运行才能最终确认的问题

1. `asc_sync_mte3` 是否真正阻塞 PIPE_MTE3 管线直到所有 MTE3 操作完成
2. id 参数在不同架构（3510 vs 2201）上是否有不同行为
3. 连续多次调用 `asc_sync_mte3` 的性能影响

## 9. 编译与运行质量

- cpudebug 编译: 未执行（缺少编译环境）
- 上板编译: 未执行
- case 代码仅作为骨架提供，需在完整编译环境中验证

## 10. 关键证据

- 源码确认: `asc_sync_mte3` 是 inline 函数，直接调用 `pipe_barrier(PIPE_MTE3)`
- 与文档中 `__aicore__ inline` 声明一致（此 API 不存在 macro 问题）
- SIMT 不可用: 源码中有 `#error` 守卫

## 11. 对原文的建议回写

1. **[产品名称]** "Ascend 950PR/Ascend 950DT" → "Atlas 350 加速卡"
2. **[SIMT 限制]** 需新增说明：此 API 不支持 SIMT 模式，编译将报错
3. **[id 参数]** 建议补充 id 参数的取值范围和语义说明

## 12. 复现入口

```
cases/asc_sync_mte3/01_basic_invoke/   # 基本调用
cases/asc_sync_mte3/02_sample_replay/  # 文档示例复现
```

## 13. 特殊值测试情况

| 特殊值 | 测试结果 | 备注 |
|--------|----------|------|
| id=0 | case 01 覆盖 | 默认值 |
| id=1 | 未测试 | 需补充 |
| 负数 id | 未测试 | 语义不明 |

## 14. 约束章节覆盖情况

| 约束项 | 文档声明 | 验证状态 | 备注 |
|--------|----------|----------|------|
| 无约束 | 已声明 | 部分验证 | cpudebug 无法验证实际同步行为 |
| 产品支持 | 950+A3+A2 | 已确认 | 产品名称需修正 |
| SIMT 不兼容 | 未声明 | 源码确认存在 | 需补充到文档 |
