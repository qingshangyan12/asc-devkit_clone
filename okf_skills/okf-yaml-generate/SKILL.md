---
name: okf-yaml-generate
description: 建设侧唯一打标记入口：为指定 Ascend DevKit API 文档或目录生成、校验并写入完整 OKF YAML 头（description、tags、dependencies、Doxygen 关联、样例 marker），含 full 全管线与 tags、description、deps、check 子命令及字段闭包校验。
---

# OKF YAML 生成

本 Skill 是建设侧唯一打标记入口，统一由 `scripts/controller.py` 提供 CLI。页面分类和允许字段以
`config/source-context-v1.yaml`、`config/document-profile-registry.yaml` 为准；标签词表和必填证据规则以
`../okf-tag-research/taxonomy/tag-taxonomy.json`、`../okf-tag-research/taxonomy/tag-rules.yaml` 为准。
这些规则由脚本校验，无需按目录手写 YAML 字段或复述解析器逻辑。
`version` 由语义输入提供；可先用 `scripts/lifecycle_stage.py` 扫描解包后的 CANN 头文件和 Release Notes，
输出按函数原型或宏归并的生命周期证据。

## 范围与写入

仅处理 `docs/zh/api/SIMD-API/{c_api,basic_api,adv_api,tensor_api}` 下指定的文件或目录。
每次使用本 skill 的 `tmp/<run-id>` 作为唯一运行目录。头文件和样例是取证输入；
默认写入仅限选中文档的 front matter、有证据的 include Doxygen 注释和样例调用 marker。
脚本可移除旧的 `api_signature` YAML 围栏；不改其他 Markdown 正文、声明、实现、样例代码或索引正文。

所有子命令遵循同一写入门禁：校验或 dry-run 未通过不得写入；同一文档禁止并发写入。
用户要求生成或更新时，校验通过后执行写入；明确要求 dry-run 或禁止写入时停在校验。

## 统一 CLI

| 子命令 | 用途 | 入口脚本 |
| --- | --- | --- |
| 无子命令（full 管线） | `prepare → worker → resolve-issues → validate → write` 全管线，一次产出完整 YAML 头 | 本 Skill 原有管线 |
| `tags` | 按冻结词表为已有头的文档选择并写入 tag 块，先 dry-run 后 `--write` | `scripts/write_tags.py` |
| `description` | Agent 撰写单 API description，逐字证据校验后受控写入 | `scripts/description.py` |
| `deps` | 基于正文证据生成并写入 `dependencies`，替换旧 `usage_dependencies` | `scripts/generate_dependencies.py` |
| `check` | 字段注册/生成器/校验器/Profile 一致性闭包检查；同时内嵌为 `prepare`/`validate` 的强制关口 | `scripts/field_manager.py` |

各子命令对应的独立脚本仍可单独调用（`scripts/write_tags.py`、`scripts/description.py`、
`scripts/generate_dependencies.py`、`scripts/field_manager.py`），行为与子命令一致。

## full 管线（默认）

1. `prepare` 扫描目录内全部 Markdown，按正文分类并建立任务包，不按文件名跳过索引页。
   每批最多 25 篇；读取 `classification-report.json` 确认逐页类型。无法分类的页面在报告中
   列明路径和原因，`prepare` 停止且不创建任务包；字段闭包检查失败时也停止。
2. 对每批非 `reference.index` 文档准备哈希绑定的 `semantic-input.json`：
   `description` 来自本 Skill `description` 模式验证通过的结果，
   `tag` 每个值有逐字证据，`dependencies` 有结构化结果。
   按 [语义输入契约](references/semantic-contract.md)填写；索引页由链接直接生成，无需语义输入。
3. 运行 `worker`。单文件只需一个 worker；目录批次最多并发 6 个，不按字段拆 agent。
   仅在需要修改 worker 结果或处理 Doxygen、marker 时读 [工件契约](references/artifact-contract.md)。
4. 运行 `resolve-issues` 生成 `resolution-input.json`。有问题时只把该文件交给一轮
   问题处理 agent；仅在证据冲突时回读相关原文。决议按
   [问题处理规则](references/issue-resolution.md)提交；没有问题则跳过问题处理 agent。
5. 运行 `validate`，成功后按请求运行 `write`。读取 `error-report.json`；
   如有问题，最终反馈给出问题数和 `error-report.md` 的绝对路径。

在本 skill 目录执行（单文件用 `--document`，目录用 `--source-dir`）：

```bash
python3 scripts/controller.py prepare --devkit-root /path/to/asc-devkit --document docs/zh/api/SIMD-API/c_api/foo.md --run-dir tmp/run-id
python3 scripts/controller.py worker --task tmp/run-id/batches/batch-01/task.json --semantic-input tmp/run-id/batches/batch-01/semantic-input.json --output tmp/run-id/batches/batch-01/worker-result.json
python3 scripts/controller.py resolve-issues --run-dir tmp/run-id
python3 scripts/controller.py resolve-issues --run-dir tmp/run-id --decisions tmp/run-id/resolution-decisions.json
python3 scripts/controller.py validate --run-dir tmp/run-id
python3 scripts/controller.py write --run-dir tmp/run-id

python3 scripts/lifecycle_stage.py \
  --package 9.0.0=/path/to/cann-9.0.0 \
  --package 9.2.0=/path/to/cann-9.2.0 \
  --release-notes docs/zh/api/appendix/Release_Notes/CANN_9_2_0.md \
  --output tmp/run-id/lifecycle.json
```

索引页的 worker 可省略 `--semantic-input`；没有问题时省略带 `--decisions` 的第二次
`resolve-issues`。worker 可先无语义输入生成供取证用的 scaffold，但该结果不能通过最终校验，
必须用已验证语义输入重新运行。

## tags 模式

为指定 API Markdown 选择已有 tag 值，并把结果写入 YAML front matter。只消费已批准的冻结词表，
不得扩展或另建词表；不参与词表调研、候选标签发现、检索评测或依赖关系生成。
目标是单个 Markdown 文件或明确指定的 API Markdown 目录，且目标文件已完成基础 OKF YAML；
没有 front matter 时先走 full 管线基础阶段，不能由本模式猜测或补齐其他字段。

工作流程：

1. 确认精确的 DevKit 根目录和用户指定的文件或目录范围，不以近期修改文件代替目标。
2. 读取冻结 taxonomy；只使用其中已有的 namespace 和 value。
3. 阅读目标 Markdown 的标题、功能描述、原型、参数说明和约束，收集逐字证据。必要时对照至少两个相邻或同操作 API，但不能把对照文档的事实写成当前文档的证据。
4. 为 `roles` 选择至少一个有证据的值；计算、搬运或初始化 API 还必须选择 `operations`。计算 API 必须选择 `execution_models`，因此内存矢量与寄存器矢量等同名 API 不得省略执行模型。其他 namespace 仅在有证据且有检索用途时填写；不适用字段省略，不填空数组、临时值或猜测值。
5. 生成 annotation artifact，运行默认 dry-run。dry-run 必须成功后，才能用 `--write` 执行实际写入。
6. 写入后重新解析目标 front matter，确认只改变 `tag`，并确认所有已写 namespace 属于冻结词表且满足条件必填规则。

强制标签要求：当前 taxonomy 允许以下 9 个 namespace：

`api_surfaces`、`roles`、`families`、`operations`、`objects`、`execution_models`、`source_locations`、`destination_locations`、`traits`。

`api_surfaces` 和 `roles` 是全局必填；api_surfaces 必须由稳定资源路径证明。`operations` 对 `compute`、
`data_movement`、`data_initialization` 角色条件必填；`execution_models` 对 `compute` 条件必填。
其他字段按正文证据和实际检索需求选填。

YAML 使用主题加内容的数组形式，每个 namespace 只能出现一次：

```yaml
tag:
  - api_surfaces: [c_api]
  - roles: [compute]
  - families: [arithmetic]
  - operations: [add]
  - objects: [vector]
  - execution_models: [register_vector]
  - source_locations: [vector_register]
  - destination_locations: [vector_register]
  - traits: [elementwise]
```

输出顺序固定为 taxonomy 的 namespace 顺序，namespace 内的值按字典序排列。必填和条件必填规则统一读取
`../okf-tag-research/taxonomy/tag-rules.yaml`。不得写成 `tags`、`semantic_tags`，也不得把主题和值压缩成
无 namespace 的字符串数组。

证据要求：

- annotation 中每个选中的 `namespace:value` 都必须有 `evidence` 对应项。
- 除 `api_surfaces` 这类稳定导航标签外，证据必须是目标 Markdown 中逐字出现的短摘录。
- 不能仅凭目录、文件名、API 名称、dtype、架构、头文件声明、样例注释或相邻 API 推断语义标签。
- `traits:deprecated` 仅在目标 Markdown 明确声明当前页面描述的接口或符号自身已废弃时选择；
  “用于标注其他废弃接口”的宏不属于废弃实体。
- 不能为了满足必填 namespace 修改正文、声明、参数名、类型、实现代码、头文件或样例。

Annotation artifact（放在项目约定的临时目录，通常为 `tmp/okf-agent/<run-id>/tags.json`；
`tag` 必须包含且只包含 9 个 namespace；`proposed_tags` 必须为空）：

```json
{
  "schema_version": 1,
  "annotations": [
    {
      "path": "docs/zh/api/SIMD-API/c_api/.../asc_example.md",
      "tag": {
        "api_surfaces": ["c_api"],
        "roles": ["compute"],
        "families": ["arithmetic"],
        "operations": ["add"],
        "objects": ["vector"],
        "execution_models": ["register_vector"],
        "source_locations": ["vector_register"],
        "destination_locations": ["vector_register"],
        "traits": ["elementwise"]
      },
      "evidence": {
        "operations:add": "目标 Markdown 中支持该操作的原文摘录"
      },
      "confidence": 0.9,
      "proposed_tags": []
    }
  ]
}
```

默认只校验和预览，不写文件；校验成功后才允许写入：

```bash
python3 scripts/controller.py tags --devkit-root /path/to/asc-devkit --input tmp/okf-agent/<run-id>/tags.json
python3 scripts/controller.py tags --devkit-root /path/to/asc-devkit --input tmp/okf-agent/<run-id>/tags.json --write
```

写入器只修改 Markdown front matter 中的 tag 块，不生成 tag 索引，不改 headers、examples 或正文。
每次目录运行都应使用一个范围精确的 aggregate artifact；不得把目录外的 API 混入工件。

失败处理：taxonomy 不是 `frozen`、namespace 不一致或 value 不存在、缺少 namespace、值为空、
证据缺失或证据不逐字匹配、目标路径不在支持的 API 文档目录、front matter 不存在或 YAML 无法解析时，
一律停止且不写入；dry-run 未成功禁止追加 `--write`。

## description 模式

只处理一个 API Markdown 的 `description`。Agent 负责理解 API 语义并撰写描述；脚本只固定输入、
校验证据与文档哈希、生成写入计划和执行受控写入。脚本不得从标题、文件名、正则匹配的段落或既有 YAML
自行提取、拼接或改写 description。

输出要求：description 使用中文单段摘要，准确说明 API 的核心动作、处理对象和结果或关键约束。它必须：

- 只陈述目标 Markdown 正文直接支持的事实，不添加未说明的能力、dtype、架构、调用顺序或性能结论；
- 不是标题、文件名、函数名或正文的机械首行复制；
- 对每一项语义结论提供目标 Markdown 正文中的一至三条逐字 evidence；
- 无法可靠概括时输出 `unresolved`，不猜测。默认不能将 unresolved 结果写入文档。

允许阅读标题、功能说明、原型、参数说明、约束和示例章节来理解语义。相邻文档或头文件只可帮助消除歧义，
不能作为 description 的证据来源。

流程：

1. 对用户给定的单个 API 执行 `prepare`。脚本创建带输入哈希的任务包，未修改 Markdown。
2. Agent 只读取任务包指定的 Markdown 正文，撰写 `description-result.json`。创建前必须阅读
   [工件契约](references/description-artifact-contract.md)。
3. 执行 `validate`。它验证 JSON 结构、输入哈希和 evidence 逐字匹配；校验器不评价或生成摘要语义。
4. 执行不带 `--write` 的 `write`，检查 `write-plan.json`。通过后才执行带 `--write` 的相同命令。
   写入只修改现有 front matter 的顶层 `description`；若字段不存在则插入到 `resource` 后。它不会创建
   front matter，也不会改动正文、tag、signature、dependencies、examples 或 version。

```bash
python3 scripts/controller.py description prepare \
  --devkit-root /path/to/asc-devkit \
  --document docs/zh/api/SIMD-API/c_api/.../asc_example.md \
  --run-id asc-example-description \
  --run-dir tmp/okf-agent/asc-example-description

# Agent 写入 tmp/okf-agent/asc-example-description/description-result.json 后：
python3 scripts/controller.py description validate \
  --run-dir tmp/okf-agent/asc-example-description \
  --result tmp/okf-agent/asc-example-description/description-result.json

python3 scripts/controller.py description write \
  --run-dir tmp/okf-agent/asc-example-description \
  --result tmp/okf-agent/asc-example-description/description-result.json

python3 scripts/controller.py description write \
  --run-dir tmp/okf-agent/asc-example-description \
  --result tmp/okf-agent/asc-example-description/description-result.json \
  --write
```

`unresolved` 结果只保留问题工件。只有用户明确要求写空值时，才可在最后一条命令附加
`--allow-unresolved --write`。

与 full 管线的关系：full 管线的 scaffold 不再从正文自动生成 description。完整 YAML 流程需要
description 时，先运行本模式，将经验证的 `description`、`evidence` 和文档哈希带入 worker artifact；
缺少该工件时应报告 `description` 未决，而不是让总控脚本补写摘要。

强制约束：

- 只接受用户指定的单个 API Markdown，路径必须在 `docs/zh/api/SIMD-API/` 下。
- `prepare` 之后目标文档发生任何改变，必须重新开始运行；不得复用旧 result。
- 不得用 API 名称、目录、产品支持表、头文件、样例注释或相邻文档作为 evidence。
- dry-run 未通过时，禁止使用 `--write`。默认拒绝写入空 description。

## deps 模式

只生成 `dependencies`，不处理 tag、索引、检索策略、头文件或样例源码。字段形状以同级的
[`OKF字段规范.md`](../OKF字段规范.md) 为准；详细契约见 [依赖契约](references/dependency-contract.md)，
证据规则见 [证据规则](references/evidence-rules.md)。旧 guard 工具只作为关系识别、路径校验和环检测的参考，
本 Skill 不调用它的 `usage_dependencies` 写入器，也不迁移旧工具。

输出字段：每个目标 API 都必须得到一个 `dependencies` 数组：

- 有明确关系时，数组包含 `path`、`kind`，必要时包含 `condition`。
- 没有明确关系时，写入 `dependencies: []`。
- 目标文档已有 `usage_dependencies` 时，删除旧字段并写入新的 `dependencies`；最终不能并存。
- 当前字段不表达 `status`、`example_before`、`all_of` 或 `any_of`。

工作流程：

1. 确认精确 DevKit 根目录和用户指定的 API 文件或目录。只处理 `docs/zh/api/SIMD-API/` 下的
   `c_api`、`basic_api`、`adv_api` 和 `tensor_api` 文档。
2. 阅读执行 API 的正文、约束、参数说明和代码示例，并读取正文明确指向的候选 API。
3. 只根据正文中的明确关系生成候选；不能凭名称、头文件、dtype、架构或一次示例调用推断依赖。
4. 为每个依赖保存逐字证据。证据格式和关系边界见两个 reference 文件，主代理在写工件前必须阅读它们。
5. 生成 annotation artifact，并运行默认 dry-run；dry-run 成功后，使用完全相同的工件执行写入：

```bash
python3 scripts/controller.py deps --devkit-root /path/to/asc-devkit --input tmp/okf-agent/<run-id>/dependencies.json
python3 scripts/controller.py deps --devkit-root /path/to/asc-devkit --input tmp/okf-agent/<run-id>/dependencies.json --write
```

6. 写入后重新解析目标 front matter，确认 `usage_dependencies` 已移除、`dependencies` 与工件一致，且正文未变化。

Annotation artifact 必须是以下结构的 JSON。每个 dependency 条目都必须带证据；证据只用于校验，不会写入 YAML：

```json
{
  "schema_version": 1,
  "annotations": [
    {
      "document_path": "docs/zh/api/SIMD-API/c_api/.../execute.md",
      "dependencies": [
        {
          "path": "docs/zh/api/SIMD-API/c_api/.../configure.md",
          "kind": "requires",
          "condition": "量化模式下",
          "evidence": {
            "source_path": "docs/zh/api/SIMD-API/c_api/.../execute.md",
            "excerpt": "量化模式下需要先完成配置。"
          }
        }
      ]
    }
  ]
}
```

没有依赖的文档使用 `"dependencies": []`，不能省略该键。

安全边界：

- writer 只修改目标 Markdown 的 `dependencies` 和旧 `usage_dependencies` 块。
- 不生成 `api-dependency-index.yaml` 或任何其他索引。
- 不修改 Markdown 正文、头文件、样例源码、实现代码或其他未指定 API 文档。
- 任一路径、证据、关系类型、YAML 或循环校验失败，整个写入停止。

## check 字段闭包

字段不能只修改注册表；必须同时具备注册、生成、校验和测试能力。`check` 子命令提供只读检查：

```bash
python3 scripts/controller.py check
```

字段检查必须确认：

- `common_fields`、`common_required_fields`、`field_order`、`field_catalog` 互相一致；
- 每个 Profile 引用的字段已注册；
- 每个注册字段都有代码生成能力和校验能力；
- 选中文档可以分类到已注册的 `okftype`；
- 生成器不会产生注册表之外的字段。

缺少生成器或校验器时，`prepare`/`validate` 的闭包关口中止运行，不得创建可执行批次或写入文档。

新增字段要求：新增字段需要同步提交字段语义、Profile 归属、必填性、输出顺序、生成器、校验器、
证据来源和测试。不引入独立的 `field_types` YAML；字段类型规则由现有校验器实现，字段能力由代码能力映射声明。
