# OKF Search README

这是用户侧 skill `okf-search` 的全貌说明。看这一篇就能知道：它是干什么的、怎么让 AI 用它、
每个文件是干什么的、索引和配置里每个字段是什么意思。

## 1. 这个 skill 是干什么的

### 1.1 背景

写 Ascend C 算子要用 C API 时，面对的是 800 多篇文档，凭记忆按目录翻很慢。`okf-search` 把这些
文档整理成一个能检索的库：你把需求说给 AI，它返回候选接口文档的路径和用途。

### 1.2 它靠什么工作

靠文档里预先写好的三类"头"（由建设侧 skill `okf-build` 生产）：

| 头 | 在哪 | 给检索提供什么 |
| --- | --- | --- |
| 文档头 | 每篇 Markdown 顶部的 5 字段 YAML | 语义：做什么、什么操作、配套什么、哪个版本可用 |
| include 头 | `include/c_api/**/*.h` 声明上方的 Doxygen | 结构：重载编号、适用架构、参数方向 |
| examples 头 | 样例源码里的 `//! [override_id]` 成对标记 | 关联：样例和哪个重载对应 |

`okf-search` 把三类头推导成一份持久索引，查询时只查索引、按需打开候选文档，不需要现场全文搜索。

### 1.3 两个工作流

1. **建立索引**：三类头 → 门控化持久索引（CANN 版本 × 架构 × 层级）。索引是纯派生数据，
   校验不过不出索引，同样输入两次构建结果字节一致。
2. **使用索引（检索）**：理解需求 → 保障索引就绪 → 多轮查询 → 给出分阶段候选文档。

### 1.4 边界

- 只帮找文档，不生成算子代码、不替你写 kernel。
- 当前只覆盖 C API（`docs/zh/api/SIMD-API/c_api/`）；其他层级会如实标注"范围外"。
- 不改文档、不改索引构建产物；写头、改词表是 `okf-build` 的事，两个 skill 互不引用对方文件。

## 2. 怎么用（AI 使用方法）

这个 skill 是给 AI 用的：你只需要把需求说清楚，检索由 AI 按 `SKILL.md` 的流程完成。

### 2.1 接入方式

`okf-search` 已放在仓库 `.agent/skills/okf-search/`，opencode 等客户端通过 `.opencode/skills`
（软链到 `.agent/skills`）自动发现，无需额外接入。若工具不支持自动加载，在对话里补一句
"按 `.agent/skills/okf-search/SKILL.md` 的流程帮我找……"。

### 2.2 直接提问

把需求用自然语言说给 AI 即可，例如：

- "把 GM 上的数据搬到 UB，用哪个接口？"
- "想把 half 转成 fp8，有哪些接口？"
- "asc_xxx 这个接口要搭配什么接口一起用？"
- "9.1.0 起可用的拷贝接口有哪些？"
- "round_mode 这个参数哪些接口在用？"

一句话里可以同时带上阶段、数据类型、版本、产品、行为约束（如非对齐、填充），说得越具体候选越准。

示例：你说"在 A3 上想把两个 float 向量逐元素相加"，AI 会先按 A3 门控准备索引，再按
"操作=add、类型=float"查询，最后返回 `docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_add.md`
及一句话用途；如果需求有歧义，它会先反问你。

### 2.3 AI 会做什么

1. **索引前置保障**：按本机 CANN 版本 × 目标架构检查索引，缺失或不匹配就原地重建；
   换架构/版本等于换一套文档范围，AI 会以检查结果作为本次检索的边界。
2. **意图分析**：把需求拆成阶段、操作、类型、约束、版本、产品等可检索信号。
   如果需求有歧义（例如裸词"转换"分不清是类型转换还是位置搬运），AI 会先反问你（一次不超过 3 个问题、
   带选项），不会瞎猜着查。
3. **多轮查询**：初查 → 用维度收窄；查不到时按降级链换关键词、换分类，最后用缺口登记确认是否范围外。
4. **配套展开**：对候选接口展开依赖、配置关系，反查参数用到的枚举/宏，家族变体一起保留。
5. **按需核对**：只读候选文档里支撑推荐所需的章节，区分"已确认适用 / 条件待确认 / 范围内未覆盖"。

### 2.4 AI 给什么结果

固定格式，不落盘：

```text
## 候选
- 路径: docs/zh/api/SIMD-API/c_api/xxx/yyy.md | API: asc_yyy | 用途: 一句话
## 文档范围内未覆盖
- 能力描述（及源码或范围外线索，若有）
```

必要时附"待确认"事项。想深入看某个接口细节或算子实现，直接追问即可。

## 3. 内部实现方案

### 3.1 整体架构

```text
三类头（文档 slim 头 / include Doxygen / examples marker）
   │
   ├─ build_index.py：解析 → 推导 → 门控（CANN 版本 × 架构 × 层级）→ 视图/依赖图/defs → 落盘
   │       产物：index/c_api/{index.json, build-meta.json, gaps.json, 概览.md}
   │
   ├─ ensure_index.py：检索前检查索引是否匹配本机环境、指纹是否新鲜；不匹配就原地重建
   │
   └─ query.py：唯一检索入口，按维度查视图，输出 路径 | API | 命中原因
```

设计原则：能在构建期算一遍的，绝不在查询期现算；查询只做索引查表和按需读文档。

### 3.2 目录与文件作用

```text
okf-search/
├── SKILL.md                        # 给 AI 的入口：建索引 + 检索的完整流程与输出规范
├── README.md                       # 本文件：给人的全貌说明
├── config/                         # 运行配置（脚本直接读取，随包分发）
│   ├── tag-taxonomy.json           # 冻结词表：构建时校验文档头 tag 是否合法
│   └── product-map.yaml            # 架构/产品映射：A5↔3510、标记→产品支持
├── references/                     # 规则与流程文档（按需阅读）
│   ├── README.md                   # reference 导航
│   ├── index.md                    # 建立索引流程、构建门控、查询参数全集
│   ├── retrieve.md                 # 检索流程：意图分析、多轮查询、输出格式
│   └── 索引推导契约.md              # 推导规则唯一事实源：推导维度、兜底边界、门控
├── scripts/                        # 全部可执行脚本（见下表）
│   └── derive/                     # 推导引擎（被 build_index/query 复用）
├── tests/
│   ├── test_index.py               # 门控/推导/确定性/ensure 决策/dryrun 回归
│   └── dryrun.yaml                 # 查询清单（A5/A3 双实例）
├── index/                          # 索引实例（构建产物，运行期生成，不提交）
└── derive-overrides/               # 人工/LLM 裁决残留（如有，构建确定性消费）
```

脚本作用：

| 脚本 | 作用 |
| --- | --- |
| `scripts/build_index.py` | 构建索引：解析三类头、推导、门控、生成视图与依赖图；`--check` 做文档漂移检测 |
| `scripts/ensure_index.py` | 前置保障：检测本地 CANN/架构门控与指纹新鲜度，按需原地重建 |
| `scripts/query.py` | 查询 CLI：按维度过滤、家族卡、依赖遍历、参数/枚举反查、缺口检索 |
| `derive/slim_header.py` | 解析文档 slim 头、加载冻结词表并校验 tag 合法性 |
| `derive/include_db.py` | 解析 include 声明与 Doxygen：签名、架构、dtype、位置、参数、掩码、废弃 |
| `derive/docparse.py` | 解析文档正文：标题、导航链接、产品支持标记 |
| `derive/examples_db.py` | 解析样例 marker，建立重载 ↔ 样例关联 |
| `derive/gate.py` | 门控：CANN 版本检测链、架构标签映射、版本区间判定 |
| `derive/overrides.py` | derive-overrides 兜底：读取人工裁决并确定性应用 |

### 3.3 三类头（索引输入）的字段含义

#### 文档头（5 字段）

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

| 字段 | 检索里的用途 |
| --- | --- |
| `okftype` | 区分接口页/常量页/导航页等；导航页不会当候选推荐 |
| `description` | 关键词匹配的来源之一（`--kw`） |
| `tag` | 三组语义标签：`roles`（流程角色）、`operations`（操作）、`traits`（稳定特性）；对应 `--stage/--operation/--feature` 等查询 |
| `dependencies` | 配套文档与关系；`--deps-of` 遍历依赖图时使用 |
| `version` | CANN 生命周期区间；构建期门控参与过滤，`--version` 可反查 |

`dependencies[].kind` 含义：`requires` 要配合它、`configures` 用它配置、`constrained_by` 受它约束、
`related` 相关、`replaced_by` 被它取代（废弃接口指向替代目标）。

#### include 头

| 标签 | 推导出什么 |
| --- | --- |
| `@override_id` | 重载级别标识；样例 marker 和参数反查都挂靠它 |
| `@archs` | 每个重载适用架构，决定门控是否收录 |
| `@param [方向]` | 参数名、类型、方向（in/out/in,out）→ `--param` 反查 |
| `@ingroup` | API 分类（api_class）佐证 |
| `ASC_DEPRECATED(...)` | 推导废弃状态与替代目标 |

#### examples 头

成对的 `//! [override_id]` 包住一次样例调用，id 与 include 头的重载编号一致，用于建立
"接口 ↔ 可运行样例"的关联。

### 3.4 索引与配置文件格式

#### `config/tag-taxonomy.json`（本 skill 自带的冻结词表）

构建索引时用它校验文档头 tag 的合法性（namespace 是否固定 3 个、值是否在册）。

| 键 | 含义 |
| --- | --- |
| `taxonomy_id` / `status` | 词表版本号；必须 `frozen` |
| `required_namespaces` | 必填 namespace（roles） |
| `namespaces.<name>.values` | 合法值及英文定义（roles / operations / traits） |
| `approval` | 人工批准记录（谁、何时、批了什么） |

#### `config/product-map.yaml`（架构与产品映射）

```yaml
archs:
  "3510":
    labels: [A5]                  # 检索用代际标签
    soc_versions: [Ascend950]     # SoC 版本名
    npu_markers: ["950"]          # 文档正文 <!-- npu="950" --> 标记
    products: [...]               # 对应产品线
```

用途：`--arch A5` 先映射成架构号 3510 再做门控；文档正文的 npu 标记反推产品支持矩阵；
`--product 950,A3` 按标记过滤。

#### `index/c_api/index.json`（索引实例）

| 段 | 内容 |
| --- | --- |
| `schema` / `meta` | 格式版本；`surface`、`cann_version`、`arch_label`、`arch`、`doc_count`、`filtered_by_version/arch`、`pending_no_okf`、`unresolved_count`、`source_commit` |
| `documents` | 每篇文档一个记录：原始 5 字段 + 推导出的 api_class、execution_model、signatures、dtypes、locations、masked、deprecated、headers、examples、enum_types、family、category、product_support、applied_overrides |
| `views` | 15 个倒排视图：by_stage、by_role、by_operation、by_category、by_api_class、by_dtype、by_dtype_pair、by_override_id、by_header、by_api、by_param、by_enum、by_feature、by_version、by_product |
| `graph` | 依赖图：`edges`（from/to/kind/condition）与反查用 `reverse` |
| `defs` | include defs 目录：enums、constants、ASC_ 宏、类型别名等 |

#### `index/c_api/build-meta.json`（新鲜度指纹）

记录 `source_commit`、`taxonomy_sha256`、`product_map_sha256`、`gaps_sha256`、逐文档
`document_sha256`。`ensure_index.py` 用它在毫秒级判断索引是否过期。

#### `index/c_api/gaps.json`（缺口登记，可选）

| 段 | 字段 | 含义 |
| --- | --- | --- |
| `scope_out_gaps[]` | `id` / `keywords` / `conclusion` / `hints` / `evidence` | 范围内未覆盖的能力：关键词、结论、范围外线索、证据 |
| `relation_gaps[]` | `from` 等 | 关系维度的已知缺口 |

缺失视为空台账、不阻断构建；存在时校验 hint/evidence 指向的文件真实存在；`--gap` 查询命中即返回。

#### `derive-overrides/<surface>.json`（人工/LLM 裁决）

脚本推不出来的残留（`unresolved.json`）经单轮裁决后固化在此，每条含
`document` / `field` / `value` / `reason`，构建时确定性应用，可审查、可回放；构建过程本身不调 LLM。

### 3.5 查询维度（query.py）

维度之间是 AND，逗号分隔的多个值是 OR：

| 参数 | 含义 |
| --- | --- |
| `--stage in,out,compute` | 分阶段候选（roles + 推导位置判定） |
| `--operation copy,divide` | 操作语义 |
| `--category` / `--api-class` | 文档目录分类 / include 子目录分类 |
| `--dtype` / `--convert src dst` | 数据类型 / 类型转换对（别名归一：f32→float、e4m3→fp8_e4m3fn） |
| `--feature low_precision` | 特性族（低精度、int4 等） |
| `--kw 非对齐,填充` | 标题 + 描述子串 |
| `--version '>=9.1.0'` / `--product 950,A3` | 按 version 区间精确匹配 / 产品标记 |
| `--api asc_copy_gm2ub` | 家族卡：主页 + 同族成员 + 分类页 |
| `--deps-of <api\|path> [--kind K] [--reverse]` | 依赖图遍历 |
| `--param` / `--enum` | 参数名或枚举类型 / 枚举名或枚举值反查使用方 |
| `--gap 同步,寄存器` | 缺口登记检索 |
| `--json` | 机器可读输出 |

输出统一为 `路径 | API 名 | 命中原因`。

### 3.6 门控与前置保障

- CANN 版本检测链：显式 `--cann-version` > `$ASCEND_HOME_PATH` 安装目录 > 仓 `version.cmake`。
- 架构：`--arch`（默认 A5）经 `product-map.yaml` 映射架构号。
- 只索引同时满足三条件的 API：`version` 覆盖本地 CANN 版本 ∧ 推导架构含目标架构 ∧ 属于目标层级；
  被过滤的 API 计入 `meta.filtered_by_version/arch`。
- `ensure_index.py` 检查门控与指纹：匹配且新鲜 → 零动作；缺失、不匹配或漂移 → 原地重建。
  轻量指纹为毫秒级，`--strict` 走全量重建比对。输出 JSON 就是本次检索的上下文
  （`surface`、`cann_version`、`arch_label`、`arch`、`categories`、`doc_count`、`actions`）。
- 换架构或版本门控 = 换索引实例与文档范围，不能把门控外的缺失说成"仓库不支持"。

### 3.7 维护规则与当前状态

- **自含性**：只依赖三类头与本 skill 的 `config/`，不 import 或读取其他 skill 的文件。
- 建设侧重新打标或 include/examples 变化后：重跑 `build_index.py --scope c_api` 并提交索引 diff；
  `--check` 报漂移属预期报警；检索触发 ensure 重建产生的本地 diff 只在维护确认后提交。
- `derive-overrides` 只登记脚本无法裁决、经人工核对的事实，每条附 reason；
  新增推导规则可消除的条目应删除。
- 当前实例：c_api 全量，SCOPES categories=None（838 篇 slim 头中 592 篇过 A5 门控）；
  历史遗留见建设侧 `references/document-issues.md` 与 unresolved 裁决记录。
