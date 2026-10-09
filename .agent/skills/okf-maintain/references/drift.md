# 漂移体检与修复

本文件是 okf-maintain 的流程契约：检查哪些漂移、如何定级、账本记什么、如何修复。
字段与写入规则（5 字段、证据必须正文逐字、version 推导、include/marker 写入面）以
建设侧 okf-build 的
[头字段与标注规范.md](../../okf-build/references/头字段与标注规范.md)、
[annotation-contract.md](../../okf-build/references/annotation-contract.md)、
[source-annotation-contract.md](../../okf-build/references/source-annotation-contract.md) 为准，本文件不复述。

## 1. 范围与触发

- 只检查被指定的文档（`--document`）或目录（`--source-dir`）；不指定时检查全量 `c_api`。
- 典型触发：合并新版本文档 / Release Notes 更新后、词表或 registry 换版后、批量编辑文档后、定期巡检。
- 检查三类头：文档 front matter（5 字段）、include Doxygen 注解、样例 `//! [override_id]` marker。
- 只看不写是默认行为；修复需显式 `--write`。

## 2. 漂移分类与定级

| 级别 | 含义 | 处理 |
| --- | --- | --- |
| DET | 确定性漂移：当前头与脚本按现行源推导的结果不一致 | `maintain.py update --write` |
| SEM-HIGH | 语义失效：正文/词表变化导致头内容不再成立，或数据缺陷阻断写入 | 按真实语义重写受影响字段（走 okf-build 标注管线） |
| SEM-LOW | 建议复核：正文变化但证据存活、头与账本不一致等 | 人工复核；需要时重标 + `baseline` 刷新 |
| INFO | 提示：未入账本、include 数据问题（无法生成/比对注解） | 记录在册，不阻断；必要时登记 document-issues |

### 文档头

| class | 级别 | 判定 | 修复 |
| --- | --- | --- | --- |
| `doc.missing_file` | SEM-HIGH | 目标文档不存在 | 核对路径/文档清单 |
| `doc.missing_header` | SEM-HIGH | 无 front matter/无法解析 | 标注管线（prepare→标注→validate→write） |
| `doc.unclassifiable` | SEM-HIGH | 结构无法分类 | 检查正文结构或登记 document-issues |
| `doc.field_schema` | SEM-HIGH/SEM-LOW | 字段集/顺序与 profile 不符；缺必填为 HIGH | 重渲染；缺语义字段走标注 |
| `doc.semantic_fields_missing` | SEM-HIGH | 字段集正确但必填语义字段为空 | 标注管线补写 |
| `doc.okftype` | DET | 重分类结果与头不同且字段集不变 | 原位刷新 |
| `doc.okftype` | SEM-HIGH | 重分类结果与头不同且字段集改变 | 字段增删需标注 |
| `doc.version` / `doc.version_missing` | DET | Release Notes 重推导与头不同/缺失 | 原位刷新 |
| `doc.tag_out_of_taxonomy` / `doc.tag_namespace` | SEM-HIGH | tag 值不在冻结词表 | 按词表重标 |
| `doc.tag_missing_roles` / `doc.tag_missing_operations` | SEM-HIGH | 条件必填未满足 | 标注管线 |
| `doc.dependencies_missing` | SEM-HIGH | 实体页缺 dependencies 字段 | 标注管线补写（可为 `[]`） |
| `doc.dependency_target_missing` | SEM-HIGH | 依赖目标文件不存在 | 按真实语义删除/替换依赖 |
| `doc.dependency_kind` / `doc.dependency_invalid` / `doc.dependency_duplicate` | SEM-LOW | 依赖条目格式/重复问题 | 修正标注后重写 |
| `doc.body_changed_evidence_lost` | SEM-HIGH | 正文变化且账本证据不再逐字存活 | 按真实语义重写受影响字段 |
| `doc.body_changed_evidence_ok` | SEM-LOW | 正文变化但证据全部存活 | 复核后 `baseline` 刷新 |
| `doc.body_changed_unverified` | SEM-LOW | 正文变化且无证据可复检（如账本无证据） | 人工复核；需要时重标 |
| `doc.header_diverged` | SEM-LOW | 头与账本记录不一致（无其他漂移命中时报告） | 复核后 `baseline` 刷新 |
| `doc.unbaselined` | INFO | 账本中无该文档 | `baseline` 建账 |

### include 注解

| class | 级别 | 判定 | 修复 |
| --- | --- | --- | --- |
| `include.stale` | DET | dry-run 推导结果与现有头不同（缺 tag/参数/架构） | `update --write`（幂等） |
| `include.header_missing` | DET | 文档映射到的头文件不存在 | 核对头文件路径 |
| `include.unresolved` / `include.issue` / `include.apply_failed` | INFO | 目标声明无法比对/参数表不全等数据问题 | 核对文档原型/参数表；必要时登记 document-issues |

### examples marker

| class | 级别 | 判定 | 修复 |
| --- | --- | --- | --- |
| `examples.stale` | DET | 被指定文档声明 API 命中的 featured 样例中，marker 缺失或与调用点异 id | `update --write` |
| `examples.example_missing` | SEM-LOW | featured 指向的样例目录不存在 | 核对 featured 名单/样例目录 |
| `examples.issue` / `examples.skip` | INFO | 调用点无法精确匹配/插入重叠等 | 人工核对样例与 featured 名单 |

配置指纹（词表、version-overrides、profile registry、type registry）与账本不一致时，
报告在 `configs` 中标出变化项；受影响文档由上述 DET/SEM 条目自然暴露。

## 3. 状态账本

`state/c_api.json`（随仓提交）：

```json
{
  "schema_version": 1,
  "surface": "c_api",
  "surface_root": "docs/zh/api/SIMD-API/c_api",
  "configs": {"taxonomy_sha256": "...", "version_overrides_sha256": "...",
               "profile_registry_sha256": "...", "type_registry_sha256": "..."},
  "documents": {
    "<仓内相对路径>": {
      "body_sha256": "正文（front matter 之后）哈希",
      "file_sha256": "整文件哈希",
      "header": {"okftype": "...", "description": "...", "tag": {}, "dependencies": [], "version": "..."},
      "evidence": {"description": ["..."], "tag": {"roles:compute": "..."},
                    "dependencies": {"<path>|<kind>": "..."}},
      "recorded_at": "ISO 时间"
    }
  }
}
```

- 账本回答两个问题：正文相对上次头写入是否变过（`body_sha256`）；当时的逐字证据还在不在（`evidence`）。
- `evidence` 只在能拿到标注工件时记录（`baseline --from-run`），随后每次语义修复后刷新；缺失时降级为
  `doc.body_changed_unverified`。
- include/examples 不需要账本：任何时刻都能从现行源重算比对。

## 4. 命令与流程

```bash
# 1. 体检（默认全部检查项；--checks doc 可只做快速确定性检查）
python3 scripts/maintain.py check --document <rel>... [--checks doc,include,examples]

# 2. 确定性修复（默认 dry-run；--write 落盘）
python3 scripts/maintain.py update --document <rel>... --write

# 3. 语义修复：按报告列出的 SEM 条目，走 okf-build 标注管线
python3 .agent/skills/okf-build/scripts/controller.py prepare \
  --devkit-root <repo> --document <rel>... --run-dir tmp/maintain/<ts>/annotate
#      Agent 按 annotation-contract.md 写 annotations/*.json（正文逐字证据）
python3 .agent/skills/okf-build/scripts/controller.py validate --run-dir tmp/maintain/<ts>/annotate
python3 .agent/skills/okf-build/scripts/controller.py write --run-dir tmp/maintain/<ts>/annotate --write

# 4. 刷新账本（记录新正文哈希与新证据）
python3 scripts/maintain.py baseline --from-run tmp/maintain/<ts>/annotate

# 5. 复检
python3 scripts/maintain.py check --document <rel>...
```

- `check` 退出码：无漂移（仅 INFO）为 0，否则 1，可用于巡检脚本。
- `update` 只处理 DET：okftype/version 原位刷新（正文与语义字段字节不变），include/examples 幂等写入；
  语义漂移留给第 3 步，不由脚本猜测。
- 修复完成且 `check` 清零后，检索侧无需人工动作：okf-search 的 `ensure_index` 会依据索引指纹自行判废重建。

## 5. 约束

- 不自动删除既有内容：孤儿注解/marker 不在本 skill 处理范围；文档问题登记 okf-build 的
  [document-issues.md](../../okf-build/references/document-issues.md)。
- 不手改索引构建产物；不改正文与代码逻辑。
- 语义修复必须有正文逐字证据；证据不足时阻断并登记，不得用临时值绕过。
