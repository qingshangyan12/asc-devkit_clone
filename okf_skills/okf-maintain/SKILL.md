---
name: okf-maintain
description: 新 API 文档范围接入 OKF 的三段流水线（调研→重标注→索引）编排入口：只确定接入步骤、检查前置条件、分派建设动作并复核结果，不新建标签、字段、Profile 或索引规则。
---

# OKF 流水线复用

本 Skill 是建设流水线的复用入口：把已在 c_api 试点验证的三段流水线（词表调研、重标注、
建立索引）打包为新 API 文档范围的标准接入路径。只编排不建规则——词表归 `okf-tag-research`，
文档头归 `okf-yaml-generate`，索引与推导规则归 `okf-index`，字段契约归
[`../字段与推导规范.md`](../字段与推导规范.md)。既有范围内文档增删改的局部修正直接复用
对应 skill 自身的维护边界（单文档重跑、重建索引、词表新轮次），不设集中维护环节。

## 输入

- 新 API 文档范围（文档根目录及角色分布；范围基线统计参考 `README.md` 第 4 节）。
- 前置检查项：目标目录在 `okf-yaml-generate` 的源上下文范围内；
  索引 `SCOPES` 注册表可表达该范围（必要时登记新 surface 或放开 categories）。
- 既有字段规范、冻结词表（core-v10）与各 skill 的 run 目录约定。

## 三段流水线

| 段 | 复用 skill | 交付物 | 关键门槛 |
| --- | --- | --- | --- |
| 1 调研 | `okf-tag-research` | 冻结词表（3 语义 namespace；可推导维度不入词表） | 双盲发现→校验→**人工批准**后冻结 |
| 2 重标注 | `okf-yaml-generate` | 范围内全部文档的 5 字段 slim 头（version 由 Release Notes 推导） | prepare→标注→validate→write；validate 未过不得写入 |
| 3 建立索引 | `okf-index` | `index/<surface>/`（CANN×架构×层级门控）+ 查询 CLI | 校验不过不出索引；单测+dryrun 全过 |

## 接入清单

1. **确定范围与角色判定**：确认文档根目录，编制范围清单与基线统计。
2. **词表覆盖评估**：core-v10 不足时先执行调研轮次并人工批准（遗留候选见
   `okf-tag-research/runs/core-v10/` 的 review notes，如 `strided_transfer`）。
3. **分批重标注**：超过 25 篇按批组织（标注 Agent 每批产出 `annotations/*.json`，
   证据正文逐字），每批走完 validate 再 write。
4. **索引接入**：在 `okf-index/scripts/build_index.py` 的 `SCOPES` 登记或放开范围、
   扩充该范围的 dryrun 查询清单后重建；确认 `unresolved.json` 残留并按需登记
   `derive-overrides`。
5. **复核**：`okf-index` 单测与 dryrun 全过；`okf-yaml-generate check` 字段闭包通过；
   `okf-retrieve` 按新范围修订检索口径。

## 强制约束

- 禁止跳过任一段直接写头或建索引；词表未冻结不得进入标注。
- 禁止临时创造未定义标签值；禁止手改索引构建产物。
- 任一校验失败必须回到对应步骤重做；文档问题登记 `document-issues.md` 并回流源文档修复。
