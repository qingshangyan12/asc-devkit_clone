# OKF Build README

这是建设侧 skill `okf-build` 的全貌说明。看这一篇就能知道：它是干什么的、怎么用、每个文件是干什么的、
词表和配置文件里的每个字段是什么意思。

## 1. 这个 skill 是干什么的

### 1.1 背景

C API 文档有 800 多篇，靠人翻目录找接口很慢。检索侧 skill `okf-search` 把这些文档变成可检索的索引，
但索引不是凭空来的——它依赖文档里预先写好的三类"头"。`okf-build` 就负责生产并维护这些头，
以及配套的词表和文档问题台账。简单说：

- `okf-search`：消费方，建索引、查接口；
- `okf-build`：生产方，调研词表、写头、编排新文档范围接入；
- 两者互不引用对方文件，可以各自独立分发。

### 1.2 三类"头"是什么

| 头 | 写在哪 | 承载什么 | 谁写 |
| --- | --- | --- | --- |
| 文档头 | 每篇 Markdown 顶部的 YAML front matter（5 个字段） | 语义信息：这篇讲什么、属于哪类操作、和谁配套、哪个版本可用 | 本 skill 的文档打标管线 |
| include 头 | `include/c_api/**/*.h` 里函数声明上方的 Doxygen 注释 | 结构信息：重载编号、适用架构、参数方向与含义 | 本 skill 的源码标注管线（大部分是仓内既有） |
| examples 头 | 样例源码调用点上下成对的 `//! [override_id]` | 关联信息：这个样例调用的是哪个重载 | 本 skill 的源码标注管线 |

### 1.3 三条流水线 + 一个独立功能

1. **词表调研**（tag-research）：确定 tag 词表有哪些合法值。两个子代理分别从文档、include、样例里
   独立找候选，脚本做确定性校验，主代理裁决，人工批准后冻结。当前生产词表是 `core-v10`。
2. **打标写入**（yaml-generate）：把文档批量换成 5 字段 slim 头（prepare → 标注 → validate → write），
   顺带把 include 注解和 examples marker 补齐。
3. **接入编排**（maintain）：新 API 文档范围接入时的操作手册，只负责分派步骤和复核，不新造规则。
4. **检索评测**（evaluate，独立功能）：建测试集、测检索耗时与 token 消耗、和基线对照。
   **不参与流程编排**，按需单独使用。

### 1.4 边界

- 只处理 `docs/zh/api/SIMD-API/c_api`（C API）范围；不改文档正文、不改接口实现。
- 不建索引、不做检索（那是 `okf-search`）。
- 写头前提是词表已冻结；词表没冻结就写不了。

## 2. 怎么快速使用

前提：本地有 asc-devkit 仓库、Python 3（脚本依赖 PyYAML，缺了就 `pip install pyyaml`）。
打标脚本从本 skill 目录执行（`.agent/skills/okf-build/`）；运行目录每次新建，不要复用人家的。
提示：让 AI 按 `SKILL.md` 路由使用最简单，下面是手工命令。

### 2.1 新范围全量接入（推荐入口）

按 `references/maintain.md`，三步走：

```bash
# 第 1 步：词表够不够？不够就先跑一轮调研（见 2.3），人工批准后冻结
# 第 2 步：分批打标，每批 ≤25 篇（见 2.2），validate 过了才 write
# 第 3 步：把范围交给 okf-search 建索引；本侧复核 controller.py check 通过
python3 scripts/controller.py check
```

### 2.2 只给一批文档打头

```bash
# prepare：扫描文档、固化哈希、从 Release Notes 推导 version，产出任务包
python3 scripts/controller.py prepare \
  --devkit-root /path/to/asc-devkit \
  --source-dir docs/zh/api/SIMD-API/c_api/vector_datamove \
  --run-dir tmp/<run-id>

# 中间：标注 Agent 读 tmp/<run-id>/annotation-task.json，按 annotation-contract.md
#       把语义判断写成 tmp/<run-id>/annotations/*.json（只写 description/tag/dependencies）
python3 scripts/controller.py validate --run-dir tmp/<run-id>   # 校验哈希/词表/逐字证据/依赖图
python3 scripts/controller.py write --run-dir tmp/<run-id>             # 默认 dry-run，只看不写
python3 scripts/controller.py write --run-dir tmp/<run-id> --write     # 真正写入
python3 scripts/controller.py check                                    # 字段闭包自检
```

### 2.3 词表调研

```bash
python3 scripts/tag_research.py prepare \
  --devkit-root /path/to/asc-devkit \
  --source-dir docs/zh/api/SIMD-API/c_api/vector_datamove \
  --run-id <run-id> --output-dir runs/<run-id>
# 启动两个全新子代理 discovery-a / discovery-b（协议见 references/worker-protocol.md）
python3 scripts/tag_research.py validate-discovery --run-dir runs/<run-id> --worker discovery-a
python3 scripts/tag_research.py validate-discovery --run-dir runs/<run-id> --worker discovery-b
python3 scripts/tag_research.py summarize-discovery --run-dir runs/<run-id>   # 主代理写 review
python3 scripts/tag_research.py validate-review --run-dir runs/<run-id>
python3 scripts/tag_research.py freeze-provisional --run-dir runs/<run-id>
python3 scripts/tag_research.py build-release-bundle --run-dir runs/<run-id>  # 给人审阅的发布包
python3 scripts/tag_research.py approve --run-dir runs/<run-id> \
  --approval runs/<run-id>/approval.json    # 人工批准后冻结，产物进 taxonomy/releases/
```

批准后才有资格切换生产词表 `config/tag-taxonomy.json`（这是单独的人工动作）。
另有 `validate-annotation` 用于校验逐篇标注是否覆盖全部必填概念。

### 2.4 源码标注（include Doxygen + 样例 marker）

```bash
python3 scripts/annotate_sources.py include   [--only <doc子串>] [--refresh] [--write]
python3 scripts/annotate_sources.py examples  [--featured-dir featured/] [--refresh] [--write]
```

默认 dry-run，只产出 diff 预览和问题清单到 `tmp/source-annotate/`；`--write` 才落盘，且幂等。

### 2.5 检索评测（独立功能，按需）

```bash
# 采集某个子会话的耗时/token/审计数据
python3 scripts/collect_metrics.py --start-ms <起> --end-ms <止> --task-id <ID> \
  --out baseline/results/<ID>.metrics.json
# 对照 gold 评分
python3 scripts/score.py --tasks testset/tasks.yaml \
  --answers baseline/results/answers.jsonl --out baseline/results/score.json
```

评测流程、指标定义和隔离约束见 `references/evaluate.md` 与 `references/评测口径.md`。

## 3. 内部实现方案

### 3.1 数据流

```text
三类源（API 文档 / include 头 / examples）
   │
   ├─ 词表调研：双盲发现 → 校验 → 主代理裁决 → 人工批准 → config/tag-taxonomy.json（冻结）
   │
   ├─ 文档打标：prepare（哈希+分类+version 推导） → 标注 artifacts → validate → write
   │       写入每篇文档的 front matter（文档头 5 字段）
   │
   └─ 源码标注：解析文档原型/参数表/架构 → 渲染 Doxygen 写 include，渲染 marker 写样例
            （写入 include Doxygen 头 与 examples marker 头）
```

设计原则：凡是脚本能从三类源推出来的（签名、架构、dtype、位置、样例、导航、掩码、废弃……）
一律不写进文档头，留给 `okf-search` 在索引期推导；文档头只放脚本推不出来的语义判断。
所有确定性校验（哈希、词表合法性、逐字证据、依赖图、version 一致性）都由脚本做，Agent 只做语义判断。

### 3.2 目录与文件作用

```text
okf-build/
├── SKILL.md                        # 给 AI 的入口：工作流路由、全局约束、常用命令
├── README.md                       # 本文件：给人的全貌说明
├── agents/openai.yaml              # 客户端 UI 元数据（显示名、默认提示词）
├── config/                         # 运行配置（脚本直接读取）
│   ├── tag-taxonomy.json           # 冻结生产词表（唯一事实源）
│   ├── version-overrides.yaml      # version 推导的人工裁决表
│   ├── source-context-v1.yaml      # 源文档范围与文档族配置
│   ├── document-profile-registry.yaml  # 页面种类 → 允许/必填字段
│   └── prototype-id/type-registry.yaml # C 类型 → 原型 ID 类型码表
├── featured/                       # 样例重点展示名单（agent 产出、随包版本化；examples marker 输入）
├── references/                     # 规则与流程文档（按需阅读）
│   ├── README.md                   # reference 导航
│   ├── tag-research.md             # 词表调研流程
│   ├── yaml-generate.md            # 文档打标流程
│   ├── maintain.md                 # 新范围接入编排
│   ├── evaluate.md                 # 检索评测流程（独立功能）
│   ├── 头字段与标注规范.md          # 字段契约唯一事实源（5 字段/证据规则/version）
│   ├── annotation-contract.md      # annotations/*.json 的格式与规则
│   ├── source-annotation-contract.md # include/样例标注的写入面与格式
│   ├── artifact-contract.md        # 调研阶段各种 JSON 工件的格式
│   ├── worker-protocol.md          # 调研发起子代理的提示词协议
│   ├── 评测口径.md                  # 评测指标定义与对照判据
│   └── document-issues.md          # API 文档自身问题的登记台账
├── scripts/                        # 全部可执行脚本（见下表）
├── taxonomy/                       # 词表工件
│   ├── README.md                   # 词表工件说明
│   ├── tag-rules.yaml              # 必填规则、证据规则、可推导边界
│   └── releases/core-v10-frozen.json # 人工批准冻结的当前生产版本
├── runs/                           # 词表调研运行记录（含 core-v10 全过程工件）
├── tmp/                            # 打标/源码标注运行记录（运行期生成，不提交）
├── tests/test_controller.py        # 文档打标管线的单元测试（含真实文档 dry-run）
├── testset/tasks.yaml              # 评测任务集 + gold + 判分依据
├── baseline/                       # 评测基线：原始回答、逐任务指标、基线报告
└── reports/                        # 评测报告
```

脚本作用：

| 脚本 | 作用 |
| --- | --- |
| `tag_research.py` | 词表调研 CLI：prepare / 校验 / 汇总 / 冻结 / 批准 |
| `controller.py` | 文档打标 CLI：prepare / validate / write / check |
| `annotate_sources.py` | 源码标注驱动：include / examples 两个子命令 |
| `derive_version.py` | 从 Release Notes 推导每个 API 的 version 区间 |
| `extract_api_yaml_metadata.py` | 原型解析大库：文档原型、参数表、架构、头文件映射 |
| `extract_api_descriptions.py` | 文档结构分类参考器（被调研脚本复用） |
| `doxygen_annotations.py` | include Doxygen 注解的渲染与合并规则 |
| `example_snippet_markers.py` | 样例 marker 的匹配与渲染规则 |
| `prototype_id.py` | 由函数原型确定性生成 `override_id`（重载编号） |
| `model_registry.py` | 加载 Profile 注册表，按文档结构判定页面种类 |
| `collect_metrics.py` | 从会话存储采集评测耗时/token/隔离审计数据 |
| `score.py` | 对照 gold 自动评分（缺口正确性需人工终审） |

### 3.3 三类头的字段含义

#### 文档头（5 字段，顺序固定）

```yaml
---
okftype: reference.api
description: "对两个源矢量执行逐元素加法并将结果写入目的矢量，……"
tag:
  - roles: [compute]
  - operations: [add]
  - traits: [elementwise, high_dimensional]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/general_description_and_constraints.md
    kind: constrained_by
version: ">=9.0.0"
---
```

| 字段 | 含义 | 必填性 |
| --- | --- | --- |
| `okftype` | 页面种类，如 `reference.api`；决定这篇允许写哪些字段 | 必填（所有页面） |
| `description` | 中文单段摘要，8–180 字，讲清动作、对象和关键约束 | 实体页/概念页等必填 |
| `tag` | 语义标签，仅 `roles/operations/traits` 三个 namespace | 实体页必填 `roles` |
| `dependencies` | 配套文档及关系，可空数组 | 实体页必填 |
| `version` | CANN 生命周期区间：`>=9.0.0` 表示 9.0.0 及以后，`>=9.0.0,<=9.2.0` 表示仅该区间可用 | 实体页必填 |

`okftype` 取值（来自 Profile 注册表）：

| okftype | 含义 | 需要的字段 |
| --- | --- | --- |
| `reference.api` | 可调用接口页（含架构变体页） | description + tag + dependencies + version |
| `reference.constant` | 常量定义页 | 同上 |
| `reference.enum` | 枚举/枚举别名页 | 同上 |
| `reference.macro` | 宏定义页 | 同上 |
| `reference.union` | 结构体/联合体页 | 同上 |
| `reference.type` | 数据类型目录页 | 同上 |
| `reference.index` | 导航目录页（只有 okftype 一个字段） | 无 |
| `reference.concept` | 概述、约束、机制说明页 | 仅 description |
| `reference.catalog` | 汇总、能力矩阵、对照表页 | 仅 description |
| `reference.release` | 版本发布/变更说明页 | 仅 description |

`tag` 三个 namespace 的填法：

| namespace | 含义 | 必填规则 | 例子 |
| --- | --- | --- | --- |
| `roles` | API 在流程里的角色 | 实体页必填 | compute、data_movement、configuration、query |
| `operations` | 直接执行或配置的操作 | roles 含 compute/data_movement/data_initialization 时必填 | add、copy、conversion、synchronization |
| `traits` | 影响检索的稳定行为特征 | 选填（有正文证据才填） | elementwise、unaligned_transfer、atomic、rounding_mode |

合法值以 `config/tag-taxonomy.json` 为准，不允许临时造值。

`dependencies[].kind` 取值：

| kind | 含义 |
| --- | --- |
| `requires` | 要配合它一起用 |
| `configures` | 用它做配置 |
| `constrained_by` | 受它约束（如通用约束页） |
| `related` | 相关 |
| `replaced_by` | 废弃接口指向的替代目标 |

#### include 头（Doxygen）

```cpp
/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUf32_PUf32_PUf32_u32
 * @archs 2201,3510
 * @param [out] dst 目的操作数。
 * @param [in] src0 源操作数（矢量）的起始地址。
 */
__aicore__ inline void asc_add(__ubuf__ float* dst, ...);
```

| 标签 | 含义 |
| --- | --- |
| `@ingroup` | 分组：`simd_c_api.<文档路径>` |
| `@override_id` | 该重载的确定性编号，由原型生成（如 `asc_add__PUf32_PUf32_PUf32_u32`） |
| `@archs` | 适用架构号：2201（A3/A2）、3510（A5）等 |
| `@param [方向] 名 说明` | 参数方向 `in`/`out`/`in,out` 与含义，来自文档参数表 |
| `ASC_DEPRECATED(v,date,replacement)` | 声明级废弃标记（若存在） |

#### examples 头（marker）

```cpp
//! [asc_add__PUf32_PUf32_PUf32_u32]
asc_add(z_local, x_local, y_local, block_length);
//! [asc_add__PUf32_PUf32_PUf32_u32]
```

上下两行同编号的 marker 包住一次调用，编号与 include 头的 `@override_id` 完全一致。

### 3.4 各种词表/配置文件的格式

#### `config/tag-taxonomy.json`（生产词表，唯一事实源）

```json
{
  "schema_version": 1,
  "taxonomy_id": "okf-core-v10",
  "status": "frozen",
  "provenance": "从哪一版怎么裁剪来的",
  "required_namespaces": ["roles"],
  "namespaces": {
    "roles":      {"description": "", "required": true,  "values": {"compute": "英文定义", ...}},
    "operations": {"description": "", "required": false, "values": {"add": "Addition", ...}},
    "traits":     {"description": "", "required": false, "values": {"atomic": "...", ...}}
  },
  "base_version": "core-v10-base-taxonomy.json",
  "approved_additions": [
    {"tag": "traits:aligned_transfer", "definition": "定义", "selection_rule": "什么时候选它",
     "evidence": [{"path": "文档路径", "excerpt": "正文逐字证据"}]}
  ],
  "approval": {"schema_version": 1, "purpose": "human_taxonomy_approval", "run_id": "core-v10",
               "decision": "approve", "approver": "谁批的", "approved_at": "时间", "comment": "备注"}
}
```

要点：`status` 必须是 `frozen` 才能被写入器使用；`values` 里的英文定义是给标注 Agent 和人工看的选值依据；
`approved_additions` 记录本版新增值及证据；`approval` 是放行记录。

#### `taxonomy/tag-rules.yaml`（填标规则）

| 键 | 含义 |
| --- | --- |
| `namespaces` | 固定的 3 个 namespace |
| `required_namespaces.always` | 任何实体页都必须有的 namespace（roles） |
| `required_namespaces.conditional.operations.when_roles_in` | roles 命中这些值时，operations 变必填 |
| `evidence.every_selected_value_requires_evidence` | 每个选中的值都必须有正文逐字证据 |
| `evidence.evidence_must_match_source` | 证据必须逐字匹配正文 |
| `derivation_boundary.rule` | 禁止为可推导事实建标签 |
| `derivation_boundary.derivable_dimensions` | 可推导维度清单（dtype、架构、位置、掩码等），这些不写进 tag |

#### `config/version-overrides.yaml`（version 人工裁决）

脚本推导不出来的个案登记在这里，每条要有原因和 Release Notes 原文摘录：

```yaml
floors:
  asc_fma:
    floor: 9.1.0
    reason: 为什么是 9.1.0（脚本正则识别不了）
    evidence: "Release Notes 原文"
```

#### `config/document-profile-registry.yaml`（页面种类规则）

| 键 | 含义 |
| --- | --- |
| `schema_version` / `status` | 注册表版本与启用状态（必须 active） |
| `taxonomy` | 指向词表文件（说明用） |
| `common_required_fields` / `common_fields` | 所有页面共同必填/允许的字段 |
| `field_order` | 写头时的字段顺序（输出契约） |
| `unknown_policy` | 未分类文档、越界字段、重生成时的处理策略 |
| `field_catalog` | 5 个字段各自含义与事实来源 |
| `profiles` | 每种 okftype 允许的 `fields` 和必填的 `required` |

#### `config/source-context-v1.yaml`（范围配置）

声明哪些目录属于源文档（`document_roots: [docs/zh/api]`）和文档族（`context.family: reference`），
用于 prepare 时判断文档在不在范围内。

#### `config/prototype-id/type-registry.yaml`（原型 ID 类型码表）

把 C 类型名映射成 `override_id` 里的类型码，例如 `float → f32`、`half → f16`、
`fp8_e4m3fn_t → f8e4m3`。它是 `override_id` 生成规则的输入，保证同一原型每次生成同一个编号。

### 3.5 运行产物格式

#### 文档打标的 run 目录（`tmp/<run-id>/`）

| 文件 | 内容 |
| --- | --- |
| `manifest.json` | 本次运行的范围、每篇文档的 sha256、分类与字段计划 |
| `version-map.json` | 每篇文档推导出的 version 区间 |
| `annotation-task.json` | 交给标注 Agent 的任务（要标哪些文档、需要哪些字段） |
| `classification-report.json` | 文档分类报告（哪篇被判成什么 okftype） |
| `annotations/*.json` | Agent 产出的语义标注（格式见下） |
| `validation.json` | validate 的校验结论（通过/阻断原因） |
| `write-result.json` | write 的写入结果 |

`annotations/*.json` 格式（人话版）：

```json
{
  "schema_version": 1,
  "purpose": "okf_semantic_annotations",
  "annotations": [{
    "document_path": "docs/zh/api/SIMD-API/c_api/.../asc_xxx.md",
    "document_sha256": "prepare 固化值，防止标注期间文档被改",
    "description": {"value": "摘要", "evidence": [{"source_path": "同文档", "excerpt": "正文逐字"}]},
    "tag": {"roles": ["data_movement"], "operations": ["load"], "traits": ["aligned_transfer"]},
    "tag_evidence": {"roles:data_movement": "正文逐字", "operations:load": "正文逐字"},
    "dependencies": [{"path": "配套文档", "kind": "requires", "condition": "可选条件",
                      "evidence": {"source_path": "本文档", "excerpt": "正文逐字"}}]
  }]
}
```

#### 词表调研的 run 目录（`runs/<run-id>/`）

| 文件/目录 | 内容 |
| --- | --- |
| `run-manifest.json` | 运行元信息：范围、词表基线、阈值、隔离级别、状态 |
| `inputs/discovery-{a,b}.json` | 分给两个子代理的文档清单（双盲输入） |
| `outputs/discovery-{a,b}.json` | 两个子代理各自的候选值及逐字证据 |
| `validation/discovery-{a,b}.json` | 对两份发现的确定性校验报告 |
| `reports/discovery-summary.json` | 合并后的候选汇总（哪些需要主代理裁决） |
| `main-agent-review.json` | 主代理裁决：每个候选 approve / defer / reject + 理由 |
| `validation/main-agent-review.json` | 对裁决的校验报告 |
| `provisional-taxonomy.json` | 通过校验的临时词表（未批准，不能用于写头） |
| `approval/release-bundle.json` | 给人审阅的发布包（含 diff 与完整性门禁） |
| `approval.json` | 人工批准记录；approve 后才会产出冻结版本到 `taxonomy/releases/` |

#### 源码标注的运行目录（`tmp/source-annotate/`）

| 文件 | 内容 |
| --- | --- |
| `signatures.json` | 全量原型解析缓存 |
| `include-report.json/.md` | include 逐头文件的 diff 预览、跳过原因、问题清单 |
| `examples-report.json` | 样例逐文件的 marker 计划与问题清单 |

#### 评测资产

- `testset/tasks.yaml`：评测任务集。字段包括 `version`、`baseline_commit`、`scope_roots`（计分语料）、
  `source_corpus`（源码辅助语料）、`acceptable_navigation`（合法导航页不计误报）、
  `tasks[]`（任务、required、alternative_groups、expected_gap、evidence 等）。
- `baseline/results/`：逐任务回答（`<ID>.answer.md`）、指标（`<ID>.metrics.json`）、汇总（`score.json`）；
  `baseline/基线报告.md` 是基线结论。

### 3.6 门禁与硬约束

- **词表先冻结再标注**：`provisional` 词表不得传给写入器；合法性以 `config/tag-taxonomy.json` 校验。
- **validate 不过不写**：哈希不一致、词表越界、证据不逐字、依赖目标缺失、version 冲突，
  任一失败都阻断写入；`--allow-partial` 只写已就绪文档。`requires` 允许成环
  （接口间存在真实互配关系），由使用者按 `condition` 判断调用顺序。
- **证据必须逐字**：description/tag/dependencies 的证据都取自目标文档正文的原始子串；
  目录、API 名、dtype、架构、头文件属性不能当语义证据。
- **只写该写的**：文档只写 front matter；include 只在声明上方补 Doxygen；样例只插 marker；
  正文和代码逻辑零改动；同一 front matter 串行写入。
- **可推导不写**：签名、架构、dtype、位置、样例、导航、掩码、废弃等由 `okf-search` 索引期推导。
- **运行隔离**：每次运行新建 `runs/<run-id>` 或 `tmp/<run-id>`，禁止复用他人工件。
- **文档问题另行登记**：源文档自身缺陷登记 `references/document-issues.md`，不靠临时值绕过。

### 3.7 当前状态

- **词表**：`core-v10` 已冻结（3 namespace / 78 值；新增 `aligned_transfer`、`datablock_granularity`；
  `masked`/`deprecated` 等改由索引期推导；`strided_transfer` 等候选记录在 `runs/core-v10/` 待复议）。
- **标注**：c_api 全树 838 篇已全部换 5 字段 slim 头（试点 140 + 全量 698），正文零改动。
- **遗留**：`unresolved` 的 derive-overrides 裁决（索引侧）、`strided_transfer` 复议、
  DOC-001/002/003/004 源文档修复（见 `references/document-issues.md`）。
