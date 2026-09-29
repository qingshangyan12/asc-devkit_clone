---
name: okf-maintain
description: OKF 知识建设流水线的复用入口：为新 API 文档范围编排 调研（okf-tag-research）→ 添加 OKF 头（okf-yaml-generate）→ 建立索引（okf-index）三段流水线。首个应用实例为 c_api 全量 800 篇（已交付）；其他 API 文档范围按同一流水线接入。只编排不建规则；既有范围内文档增删改的局部修正由各 skill 自身维护边界承担。
---

# OKF 知识建设流水线复用

本 Skill 是 OKF 建设流水线的复用入口：把已在 c_api 全量验证的三段建设流水线
（调研 → 添加头 → 建立索引）打包为可复用编排，供其他 API 文档范围接入使用。
它是**编排者**——只确定接入步骤、检查前置条件、分派建设动作并复核结果；
词表、字段、YAML 头与索引的具体规则分别由对应 skill 承担，不形成第二套规则。

## 职责与边界

- 只负责新 API 文档范围的**全量接入编排**；不处理既有范围内文档增删改的局部修正
  （受影响文档的头按 `okf-yaml-generate` 单文档重跑，索引经 `build_index.py` 重建，
  词表问题开 `okf-tag-research` 新轮次，均直接复用对应 skill）。
- 不新建标签、字段、Profile 或索引规则；词表唯一事实源仍是
  `../okf-tag-generate/taxonomy/tag-taxonomy.json`。
- 范围外知识只登记扩展需求，不自动扩大建设范围。
- 审计痕迹复用各 skill 自带的 run 目录（`okf-tag-research/runs/`、
  `okf-yaml-generate/tmp/<run-id>/`），不另建记录目录。

## 流水线（三段，均在 c_api 全量验证）

| 段 | 复用 skill | 交付物 | 关键门槛 |
| --- | --- | --- | --- |
| 1 调研 | `okf-tag-research` | 冻结词表（当前 core-v9） | 双盲发现、确定性校验、人工批准后冻结 |
| 2 添加头 | `okf-yaml-generate`（description 语义工件来自 `okf-description-generate`，依赖关系内联生成） | 范围内全部文档的 OKF YAML 头、include Doxygen、example marker | prepare→worker→resolve-issues→validate→write；validate 阻断不得写入 |
| 3 建立索引 | `okf-index` | `index/<surface>/{index.json,gaps.json,build-meta.json}` 与查询 CLI（`--surface`） | 校验不过不出索引；单测 + dryrun 全过 |

## 新范围接入清单

1. **确定范围与角色判定**：确认文档根目录，编制该范围的范围清单与角色判定规则
   （参考 c_api 实例 [`../范围清单.md`](../范围清单.md)）。
2. **前置检查**：`api_surfaces` 词表值已在冻结 taxonomy 登记；
   `okf-yaml-generate/config/source-context-v1.yaml` 的 default_tags 已覆盖该目录。
3. **词表覆盖评估**：现有冻结词表不足以描述新范围时，先执行 `okf-tag-research`
   调研轮次，人工批准后冻结，再进入标注。
4. **分批生成 YAML 头**：目录超过 25 篇按批组织，每波最多 6 个 worker；
   运行约定见 `okf-yaml-generate` 总控流程。
5. **索引接入**：在 `okf-index` 的 SCOPES 注册表登记新 surface（scope 根目录、根导航页），
   校准分类/导航规则与该 surface 的 dryrun 查询清单后执行
   `build_index.py --scope <surface>`。索引实例按 surface 隔离（`index/<surface>/`，
   含独立的缺口登记与漂移检测），不影响既有实例。
6. **复核**：`okf-index` 单测与 dryrun 全过；必要时按 `okf-evaluate` 口径回归
   检索质量。

## 当前应用实例

- **c_api**（`docs/zh/api/SIMD-API/c_api`，838 篇）：三段已全部交付——core-v9
  冻结词表、838 篇 OKF YAML 头、持久化索引与查询 CLI（`index/c_api/` 实例）。范围与角色判定见
  [`../范围清单.md`](../范围清单.md)。

## 强制约束

- 禁止跳过任一段直接为文档写头或建索引；词表未冻结不得进入标注。
- 禁止为新范围临时创造未定义标签值、绕过冻结词表或另建词表。
- 禁止手改 `okf-index/index/<surface>/` 构建产物（`index.json`、`build-meta.json`、`概览.md`），
  漂移以重建解决。
- 任一校验失败必须回到对应步骤重做，不得编辑校验结果或跳过门禁。
