# asc_sync_data_barrier 详细验证报告

## 1. 输入与目标

- **API 名称**: `asc_sync_data_barrier`
- **文档路径**: `docs/zh/api/SIMD-API/C-API/sync/asc_sync_data_barrier.md`
- **原型**: `__aicore__ inline void asc_sync_data_barrier(mem_dsb_t arg)`
- **头文件**: `"c_api/sync/sync.h"`
- **所属管线**: PIPE_S
- **产品支持**: Ascend 950PR/Ascend 950DT（应修正为"Atlas 350 加速卡"），支持 3510 + 2201 架构
- **验证目标**: 确认 API 声明、行为、参数枚举值、约束与源码一致

## 2. case 总览

| case | 路径 | 说明 |
|------|------|------|
| 01_dsb_all | `asc_sync_data_barrier/01_dsb_all/` | 测试 `DSB_ALL` 参数 |
| 02_dsb_ddr | `asc_sync_data_barrier/02_dsb_ddr/` | 测试 `DSB_DDR` 参数 |
| 03_dsb_ub | `asc_sync_data_barrier/03_dsb_ub/` | 测试 `DSB_UB` 参数 |
| 04_sample_replay | `asc_sync_data_barrier/04_sample_replay/` | 复现文档示例代码 |

## 3. 已验证成立的内容

- [x] 函数签名 `void asc_sync_data_barrier(mem_dsb_t arg)` 与文档声明一致
- [x] 头文件路径 `"c_api/sync/sync.h"` 正确
- [x] 参数枚举 `mem_dsb_t`: DSB_ALL, DSB_DDR, DSB_UB 与文档一致
- [x] DSB_SEQ 为保留值，文档正确标注
- [x] 产品支持范围 950+A3+A2（3510+2201）与文档一致
- [x] 约束"必须传入字面量枚举值"在源码层面成立（MACRO 展开要求编译期常量）

## 4. 未验证成立或需降级表述的内容

- **[关键发现] 文档声明 `__aicore__ inline` 但源码为 MACRO**: 源码中 `asc_sync_data_barrier` 是宏定义，展开为 `dsb(arg)`，并非 inline 函数
- **DSB_ALL 限制**: 文档正确指出 DSB_ALL 仅等待 GM+UB，不等待 L0C/L1，但此限制的重要性需要上板验证其实际影响
- **DSB_SEQ 保留状态**: 文档标注 DSB_SEQ 为 reserved，源码中对应值存在但未使用，需确认是否有未来计划
- **实际屏障行为**: cpudebug 模式无法验证 `dsb()` 是否真正执行数据同步屏障
- **SIMT 兼容性**: 源码中存在 `#error` 阻止 SIMT 模式编译，但文档未提及此限制
- **产品名称**: 文档中"Ascend 950PR/Ascend 950DT"应更正为"Atlas 350 加速卡"

## 5. quick review 标签与 case 覆盖对齐说明

| 标签 | 对应 case | 覆盖状态 |
|------|-----------|----------|
| dsb-all | 01_dsb_all | 已覆盖 |
| dsb-ddr | 02_dsb_ddr | 已覆盖 |
| dsb-ub | 03_dsb_ub | 已覆盖 |
| sample-replay | 04_sample_replay | 已覆盖 |
| macro-vs-inline | 所有 case | 已发现（文档写 inline，源码是 MACRO） |
| simt-incompat | 未建 case | 需补充 |
| dsb-all-limitation | 01_dsb_all | 文档已声明，需上板验证 |
| dsb-seq-reserved | 未建 case | 保留值，低优先级 |

## 6. 需要补充的测试用例

- **SIMT 编译错误验证**: 在 SIMT 模式下编译此 API，确认 `#error` 触发
- **DSB_ALL 与 L0C/L1 实际影响**: 构造 L0C/L1 写入后使用 DSB_ALL 的场景，验证数据是否不可见
- **DSB_SEQ 传入测试**: 验证传入 DSB_SEQ 是否编译报错或运行时异常
- **运行时变量传参**: 验证非字面量参数是否在编译期报错
- **连续多次 dsb 场景**: 测试连续调用不同 dsb 类型的行为

## 7. 当前无法完全验证的具体原因与求助点

- cpudebug 无法验证 `dsb()` 的实际硬件屏障行为
- DSB_ALL "不等待 L0C/L1"的实际影响需要构造特定的 L0C/L1 数据流场景验证
- MACRO 展开的编译期约束（必须传字面量）需要实际编译验证错误提示是否友好

## 8. 必须上板运行才能最终确认的问题

1. `dsb(DSB_ALL)` 是否真的不等待 L0C/L1 操作完成
2. `dsb(DSB_DDR)` 与 `dsb(DSB_UB)` 的精确等待范围
3. 3510 与 2201 上 dsb 行为是否有差异
4. 传入 DSB_SEQ 的运行时行为（是否未定义或报错）

## 9. 编译与运行质量

- cpudebug 编译: 未执行（缺少编译环境）
- 上板编译: 未执行
- case 代码仅作为骨架提供，需在完整编译环境中验证

## 10. 关键证据

- 源码确认: `asc_sync_data_barrier` 是 **MACRO** 而非 inline 函数，展开为 `dsb(arg)`
- DSB_ALL 限制: 文档正确描述了"仅等待 GM+UB，不等待 L0C/L1"
- DSB_SEQ 为保留值: 枚举中存在但未实际使用
- SIMT 不可用: 源码中有 `#error` 守卫

## 11. 对原文的建议回写

1. **[关键] 函数原型描述**: 将 `__aicore__ inline void asc_sync_data_barrier(...)` 修正为宏定义说明，或注明"实现为宏展开"
2. **[产品名称]** "Ascend 950PR/Ascend 950DT" → "Atlas 350 加速卡"
3. **[SIMT 限制]** 需新增说明：此 API 不支持 SIMT 模式
4. **[DSB_ALL 限制]** 建议在约束中更显著地标注 DSB_ALL 不等待 L0C/L1，避免用户误用
5. **[DSB_SEQ]** 建议明确说明 DSB_SEQ 为保留值，传入可能导致未定义行为
6. **[字面量约束]** 建议在参数说明中补充"必须传入编译期字面量枚举值，不支持运行时变量"

## 12. 复现入口

```
cases/asc_sync_data_barrier/01_dsb_all/       # DSB_ALL
cases/asc_sync_data_barrier/02_dsb_ddr/       # DSB_DDR
cases/asc_sync_data_barrier/03_dsb_ub/        # DSB_UB
cases/asc_sync_data_barrier/04_sample_replay/ # 文档示例复现
```

## 13. 特殊值测试情况

| 特殊值 | 测试结果 | 备注 |
|--------|----------|------|
| DSB_ALL | case 01 覆盖 | 正常用例 |
| DSB_DDR | case 02 覆盖 | 正常用例 |
| DSB_UB | case 03 覆盖 | 正常用例 |
| DSB_SEQ | 未测试 | 保留值，需补充 |
| 运行时变量 | 未测试 | 需补充（应编译报错） |

## 14. 约束章节覆盖情况

| 约束项 | 文档声明 | 验证状态 | 备注 |
|--------|----------|----------|------|
| 必须传字面量枚举值 | 已声明 | 部分验证 | MACRO 机制确认，需编译验证错误提示 |
| DSB_ALL 不等待 L0C/L1 | 已声明 | 需上板验证 | 文档正确，实际影响需确认 |
| DSB_SEQ 为保留值 | 已声明 | 源码确认 | 传入行为未验证 |
| 产品支持 | 950+A3+A2 | 已确认 | 产品名称需修正 |
| SIMT 不兼容 | 未声明 | 源码确认存在 | 需补充到文档 |
| 函数类型为 inline | 已声明 | **不成立** | 源码为 MACRO，需修正文档 |
