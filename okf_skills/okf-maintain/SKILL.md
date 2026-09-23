---
name: okf-maintain
description: 执行 OKF 知识资产的具体维护动作：变更对账与影响面分析（impact.py）、文档增删改后的 YAML 头与索引一致更新（复用 okf-yaml-generate 与 okf-index）、缺口登记增补、标签问题升级 okf-tag-research、受影响评测复核，并留存维护记录。变更或问题已确认后使用；触发机制不在职责内，不另建标签、YAML 或索引规则。
---

# OKF 知识维护

本 Skill 是 OKF 闭环的最后一段：知识资产出现变化或错误后的具体维护动作。它是**协调者**——
只确定影响范围、分派修正动作、复核结果并留痕；标签、YAML 头与索引的具体建设分别复用
`okf-tag-research`/`okf-tag-generate`、`okf-yaml-generate`、`okf-index`，不形成第二套规则。

## 职责与边界

- 只执行已确认的维护动作（文档变更、检索反馈、评测问题的**触发与判定不在范围**）。
- 维护对象是知识资产：范围内文档的 OKF YAML 头、`okf-index/index/`（含 gaps.json）、维护记录。
- 不修改其他 skill 的定义与脚本；索引产物更新经 `build_index.py` 重跑产生，不手改 `index.json`。
- 范围外知识只登记扩展需求，不自动扩大建设范围。

## 资产结构

```
okf-maintain/
  scripts/impact.py      # 变更对账 + 影响面分析（只读，标准库）
  records/               # 维护记录：每次动作一个 md（命名与模板见下文）
  tests/test_impact.py   # 纯函数单测 + 真仓冒烟
```

## 维护动作目录

| 动作 | 内容 | 复用机制 |
| --- | --- | --- |
| A 文档变更接入 | 修改→核对头与正文一致后按证据修头；新增→按范围清单规则 1-6 判角色后生成头；删除/移动→修正指向旧路径的依赖边 | `okf-yaml-generate`（`--document` 单文档粒度） |
| B 索引一致性 | 重跑构建（校验不过不出索引）→ 漂移确认 → 单测 | `okf-index/scripts/build_index.py` + tests |
| C 缺口登记 | gaps.json 按证据增补/清理；hint/evidence 必须是真实路径 | 人工编辑 + 构建强校验 |
| D 标签/词表问题 | 歧义或词表不足时开新调研轮次，禁止临时造标签 | `okf-tag-research` |
| E 范围外知识 | 记录扩展需求，待另行确定范围 | 维护记录"遗留项" |
| F 反馈回流复核 | 受影响 dryrun 查询；必要时重跑受影响评测题，质量不降低门槛 | `okf-index/tests`、`okf-evaluate` |

## 标准维护流程（对齐流程与架构说明书第 6 节）

1. **变更对账与影响面分析**（仓库根执行）：

   ```bash
   python3 okf-skills/okf-maintain/scripts/impact.py          # 文本报告
   python3 okf-skills/okf-maintain/scripts/impact.py --json   # 机器可读
   ```

   退出码：0 无漂移；1 索引资产缺失/损坏（先跑 build_index.py）；2 有漂移（报告已输出）。
   产出：变更清单（修改/新增/删除）、指纹漂移（词表/缺口登记）、每篇影响面（家族成员、
   反向依赖、依赖出边）、受影响评测题与 dryrun 查询、建议动作。

2. **核对原始依据**：逐篇读取变更文档正文，确认正文与 YAML 头差异点；证据规则以
   `OKF字段规范.md` 为准（逐字证据，不得由名称/目录/架构推断）。

3. **分派修正动作**（按动作目录 A-F）：
   - 修改/新增：`okf-yaml-generate` 的 prepare→worker→resolve-issues→validate→write，
     `--document` 限定受影响文档，不整目录重跑；
   - 删除/移动：先修**指向旧路径**的文档依赖边（impact 报告"反向依赖"列出全部来源），
     再处理被删文档自身；
   - 标签歧义/词表缺口：升级 `okf-tag-research`，本轮先按现有冻结词表降级或阻断；
   - 缺口登记变化：按证据更新 gaps.json。

4. **重建索引**：

   ```bash
   python3 okf-skills/okf-index/scripts/build_index.py        # 校验不过直接失败，不出索引
   python3 okf-skills/okf-index/scripts/build_index.py --check
   ```

5. **复核**：

   ```bash
   python3 -m unittest discover -s okf-skills/okf-index/tests
   ```

   再对 impact 报告列出的受影响 dryrun 查询逐条执行 `query.py` 核对候选不缺失；
   检索/评测反馈触发的维护须重跑对应评测题并与基线对照（质量不降低门槛）。

6. **写维护记录**：按模板在 `records/` 新建 `<YYYY-MM-DD>-<slug>.md`。

## 维护记录模板

```markdown
# <YYYY-MM-DD>-<slug>

- 日期 / 执行者：
- 触发来源：文档变更 | 检索反馈 | 评测问题 | 词表轮次 | 其他（附证据链接）
- 变更清单：impact.py 对账结果摘要
- 影响面：家族 / 反向依赖 / 受影响评测题
- 执行动作：分派到哪些 skill、改了哪些文件（含索引 diff 概要）
- 复核结果：build_index --check / 单测 / dryrun / 受影响评测题
- 遗留项：升级词表、扩展需求、待人工确认
```

## 强制约束

- 禁止手改 `okf-index/index/index.json`、`build-meta.json`、`概览.md`（纯构建产物，漂移以重建解决）。
- 禁止临时创造未定义标签值、绕过冻结词表或另建词表。
- 禁止修改 Markdown 正文、头文件声明、实现代码与样例（维护只动 front matter 与 okf-skills 资产）。
- 新目录并入 scope 属范围扩展：仅记扩展需求，不自动改 classify 规则与 dryrun 清单。
- 任一校验失败必须回到对应步骤重做，不得编辑校验结果或跳过门禁。
- 每次维护动作必须有记录；无记录的资产变更视为未完成维护。
