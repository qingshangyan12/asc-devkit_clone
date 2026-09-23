# OKF tag 最小集与可扩展候选集调研报告

调研日期：2026-09-23。源码提交：`c80ec6208abd982a69f7e9316284eacd961860a7`。

## 1. 结论与建议

建议将“九个 namespace 全部必填”改为 **两项核心字段、四项按需字段、独立候选池**：

```text
生产 tag
├─ 核心：roles、operations
│  └─ roles 对可调用 API 必填；operations 按行为适用性必填
└─ 已批准可选：execution_models、source_locations、destination_locations、traits
   └─ 只在有正文证据且有明确检索用途时填写

生产 tag 之外
├─ 派生维度：api_surfaces，由文档归属规则计算
└─ 待验证候选池：新字段、新值，以及 families/objects 的重新准入需求
```

**“两项核心”不等于所有文档强填两个值，也不等于把六个字段全部改成必填。** 普通计算 API 可以只有 `roles + operations`；纯配置 API 可以只有 `roles`；有方向的搬运 API 在搬运检索场景下补充来源和目的地；定义页和纯导航页可以不写 tag。

继续使用现有 `tag: object[]` 外形以及现有字段名称，不新增 `core_tags`、`optional_tags`、`candidate_tags` 等文档顶层字段。核心/可选属于词表规则；候选属于调研工件，不能随意写入生产文档。

本方案的“最小”是**满足当前操作查找、阶段导航、配置区分与按需消歧的最小独立维度集**，不是数学上对所有未来需求证明的全局最优解。它用现有字段整改，避免为了少一个键而创造复合编码。

主要证据：

- 实际语料为 838 篇 Markdown，只有 140 篇带 OKF 并进入索引；下文量化对照严格使用这 140 篇。
- 去掉 `api_surfaces/families/objects`，保留已有可选值，原始非空字段记录减少 **35.48%**，标签值记录减少 **31.41%**；现有 **9 类索引视图完全一致**，10 条 CLI 查询回放结果一致。
- 同一向量加法任务，推荐方案与现有方案都召回 10 个初始候选、覆盖 3/3 必需文档；只留 `operations` 为 13 个候选，简单描述关键词基线为 19 个。推荐方案没有相对现有九字段方案取得额外召回或精度提升。
- 来源、目的地、执行模型和特征有独立消歧作用，应该按需保留，而不应为了“最少字段”一并删除。

本报告及实验没有修改生产词表、索引、API 文档或生成器。以下 YAML、规则和接口适配均为整改建议。

## 2. 现状：规范、落地数据与消费者并不一致

### 2.1 项目链路与真实范围

当前链路为：词表调研 → 冻结词表 → tag 标注/OKF YAML 生成 → 索引构建 → 查询 → 正文核实/依赖展开。

| 检查项 | 本次读仓结果 | 对整改的影响 |
| --- | --- | --- |
| C API Markdown | 838 篇 | 不能沿用说明里的“800 篇”作统计分母 |
| 有 OKF / 已索引 | 140 / 140 篇 | 未标注的 698 篇属于覆盖缺口，不是“没有此 API” |
| 查询主文档 | 119 篇 | 140 篇另含 13 个普通导航页和 8 个架构页；4 个家族导航主页进入主文档集合 |
| 原始文档类型 | API 115、架构 8、索引 17 | 当前定量样本没有类型定义页，不能声称验证了全部文档类型 |
| 已索引文件哈希 | 140/140 与 build-meta 一致 | 已收录文档没有文件内容漂移；不表示整个 C API 已覆盖 |
| 冻结词表 | core-v9，9 个 namespace，117 个值 | 全部在 required_namespaces，且 namespace.required 均为 true |
| 生产词表副本 | tag-generate 与 yaml-generate 各一份 core-v9，哈希相同 | 索引实际读取 yaml-generate 副本，不能只改说明指定的那一份 |
| 旧评测范围 | vector_datamove + vector_compute | 同步、寄存器 API 的旧“范围外”判定不能直接用于全 C API |

资料依据：[标签体系说明](../../标签体系说明.md)、[字段规范](../../OKF字段规范.md)、[索引构建器](../../okf-index/scripts/build_index.py)、[检索流程](../../okf-retrieve/SKILL.md)、[旧评测集](../../okf-evaluate/testset/tasks.yaml)。输入文件哈希见 [results.json](results.json)。

### 2.2 必填规则的矛盾

三处行为不同：

1. [write_tags.py](../../okf-tag-generate/scripts/write_tags.py) 的 `load_taxonomy/validate_annotation` 要求 tag 恰好覆盖全部 namespace，每项非空；其词表加载器还要求“全部 namespace = 必填 namespace”。仅把 JSON 中 `required` 改成 false 并不能实现可选字段。
2. [controller.py](../../okf-yaml-generate/scripts/controller.py) 的 `normalize_tag` 会为缺失 namespace 补 `[]`，无证据项可成为 unresolved 问题。
3. `build_index.py:load_raw_documents` 校验键齐全和值合法，但不拒绝空数组。因此现存文档虽然拥有九个键，很多键实际没有信息。

此外，字段规范已经为类型定义页允许多数 namespace 为空，与通用“全部非空”规则冲突。标签说明和调研 taxonomy README 仍残留 core-v8 路径。这些都属于整改时需要统一的契约问题。

### 2.3 字段实用性与消费关系

下表统计**原始 Markdown**，未将架构变体聚合后的标签冒充人工标注数量。

| namespace | 非空文档/140 | 非空率 | 标签值记录数 | 原始语料不同值数 | 当前检索消费 |
| --- | ---: | ---: | ---: | ---: | --- |
| api_surfaces | 140 | 100.0% | 140 | 1 | 没有查询视图消费；全部为 c_api |
| roles | 124 | 88.6% | 124 | 7 | by_role；参与 in/out/compute 阶段派生 |
| families | 41 | 29.3% | 44 | 4 | 不参与当前过滤；不是索引的 API family 字段 |
| operations | 106 | 75.7% | 120 | 31 | by_operation、conversion_pair |
| objects | 95 | 67.9% | 115 | 4 | 不参与当前过滤 |
| execution_models | 107 | 76.4% | 107 | 2 | 保存于索引；CLI 无直接过滤入口 |
| source_locations | 34 | 24.3% | 34 | 2 | 参与搬入/搬出阶段派生 |
| destination_locations | 34 | 24.3% | 34 | 3 | 参与搬入/搬出阶段派生 |
| traits | 97 | 69.3% | 234 | 10 | 保存于索引；CLI 无直接过滤入口 |

原始共有 1,260 个字段键，其中 482 个为空；非空字段为 778 个，值记录为 952 个。索引会把家族架构页标签聚合到主页，例如 `operations` 非空页由 106 变为 110，因此索引统计不能直接代表标注工作量。

“当前没被 CLI 使用”只证明短期删除不会改变现有过滤，不能证明语义无价值。执行模型与 traits 必须结合真实消歧场景判断；families 与 objects 也应允许未来凭证据重新申请。

## 3. 推荐字段及精确定义

### 3.1 字段取舍

| 字段 | 决策 | 填写与使用规则 | 为什么这样设计 |
| --- | --- | --- | --- |
| roles | 核心 | 可调用 API 必填；从现有 8 个受控角色中选择自身角色。通常一个，多角色须分别有证据 | 回答“执行、配置、查询还是控制”；操作值为空的配置 API 仍可被结构化发现 |
| operations | 核心，条件必填 | 计算、数据搬运、数据初始化页必须标注正文明确的操作；其他角色仅有直接行为证据时写。纯参数设置允许省略 | 对齐用户的“做什么”，支持近义表达统一；不能借下游行为给配置器虚构操作 |
| execution_models | 已批准可选 | 需要区分内存矢量、寄存器矢量等执行方式时填写；按正文/原型联合证据判定，不按目录猜测 | 同名同操作可以对应不同调用模型；比为每个操作创建 reg_add/memory_add 更独立 |
| source_locations | 已批准可选，与目的地组成方向维度 | 搬运接口若正文明确来源，必须记录；存在明确来源而漏标属于搬运标注不完整；无来源概念的 API 省略 | 区分 GM→UB、UB→UB 等；保持位置词表可复用 |
| destination_locations | 已批准可选，与来源组成方向维度 | 搬运接口若正文明确目的地，必须记录；不把“配置器将来影响的目的地”当作配置器自身输出 | 与来源共同表达有向关系，不能合并成无方向 locations |
| traits | 已批准可选 | 只记录直接改变候选选择的稳定能力，如 unaligned_transfer、padding、elementwise；每个值必须有独立证据和消费用例 | 不为每一种特征增加新字段，也不把每个参数都标签化 |
| api_surfaces | 从手工 tag 移出 | 由 resource 与注册的文档归属映射派生为索引维度；当前 c_api 范围映射为 c_api | 现存 140 个值完全相同，且可由既有元数据确定；跨 API 检索仍能保留此过滤能力 |
| families | 不进入首版生产最小结构，留在历史/候选池 | 需要粗粒度浏览时优先复用目录导航或操作分组；不能将目录 category 当作与 families 语义完全等价 | 当前无直接消费，新增人工标注的收益未被证明 |
| objects | 不进入首版生产最小结构，留在历史/候选池 | 对象类型若未来确实不能由描述、执行模型或精确声明解决，提交独立查询用例再准入 | vector/scalar/matrix 常与其他信息重叠；但如进位/掩码对象仍可能有独立用途 |

首版沿用保留字段的现有冻结值，不一边精简字段一边批量重命名操作值。六个保留 namespace 对应当前 101 个值，但**不是要求每篇选满六项，更不是要求每篇选择全部相关词**。枚举值是否继续保留按第 5 节独立治理，未在这 140 篇出现的值不因零频率直接删除。

角色核心覆盖是此版设计的全量入口要求，操作核心覆盖是按行为适用性要求。当前 123 个可调用/架构页都有角色，但其中 22 页没有 operation，主要是配置、查询和控制。这些页不能为了满足“两项核心”而填 `unknown`、`other` 或虚构 `copy`。

### 3.2 文档类型与缺失值

| 文档类别 | 核心要求 | 缺失的处理 |
| --- | --- | --- |
| 函数 API / 架构变体 | roles 必填；执行计算/搬运/初始化时 operations 必填；其他角色按行为证据填写 | 应适用但证据不足：标注审计中标为 unresolved，进入补证；基础文档检索仍可收录，不能声称已通过完整标注验收 |
| 纯配置/查询 API | roles 必填；不强制用下游计算操作填 operations | 用 description 描述设置项，用 dependencies 表达配套目标 |
| 家族导航主页 | 无需手工 tag；索引从变体汇总候选能力 | 汇总用于发现，实际组合约束回到同一个变体核实 |
| 普通索引页 | 无必填 tag | title、resource、目录关系承担导航 |
| 类型/常量/枚举/对象宏定义页 | 无必填 tag | okftype、title、description、declaration/type_catalog 承担定义检索；函数宏按可调用语义处理 |

统一省略不适用的键，不写 `source_locations: []` 之类的空占位；整个 tag 无信息时省略 tag。`unknown` 与“不适用”只记录在标注审计中，不成为标签值。正文未写某能力，不代表它不支持该能力。

查询时区分“证据匹配候选”和“属性未知、待核对候选”；可选字段缺失不能在最终推荐中自动解释成不支持。明确匹配优先，未知项在证据匹配不足或用户要求穷尽时结合描述/正文补查。报告实验的 facet 表只计算明确正匹配，不宣称其已经实现完整的未知项回退。

### 3.3 不应再放入 tag 的内容

- dtype、架构、头文件、重载条件：继续放在 signature，防止页级 tag 抹平重载差异。
- 配置目标、前置关系、同步条件：使用 dependencies 及其 condition，不能靠并列标签表达调用顺序。
- API 名称、路径、精确参数：由 title/resource/正文承担；描述文本仍负责表达标签不适合承载的细节。
- `in/out/compute`：保留为查询派生视图，不增加手工 stage 字段。位置对比依赖操作上下文，不把“搬入”写死为 API 固有方向。
- `api_surfaces` 的派生规则只证明文档归属，不得顺带从目录推断普通语义标签；迁移路径必须更新受控映射，未知归属列为待处理，不猜测。

多来源、多目的地以及架构聚合须特别注意：`source_locations: [A, B]` 与 `destination_locations: [C, D]` 不证明 A→D 成立；多个 tag 共存也不证明一个重载同时支持全部特征。位置和 traits 只先用于候选召回，再核实同一变体/重载。如果真实用例反复受此限制，再评估 `transfer_paths` 等关系结构，首版不为假设需求预先增加字段。

## 4. 真实文档的标注示例

### 4.1 普通计算：九个键缩成两个核心键

[内存矢量 asc_add](../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_add.md) 当前 tag：

```yaml
tag:
  - api_surfaces: [c_api]
  - roles: [compute]
  - families: [arithmetic]
  - operations: [add]
  - objects: [vector]
  - execution_models: [memory_vector]
  - source_locations: []
  - destination_locations: []
  - traits: [count_limited, elementwise, high_dimensional]
```

满足基础操作发现的最小标注为：

```yaml
tag:
  - roles: [compute]
  - operations: [add]
  - execution_models: [memory_vector]
```

正文“执行矢量加法运算”及其公式支持这两项。在跨内存/寄存器的目录混合检索中，加 `execution_models: [memory_vector]`；需要按逐元素特征检索时再加 `traits: [elementwise]`。普通基本用例可以只写两项，但既有选用这些扩展的业务视图迁移时不能擅自丢值。

[寄存器 asc_add](../../../docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_add.md) 具有相同名称和操作，但正文明确“本接口为Reg矢量计算接口”，故对应例子为：

```yaml
tag:
  - roles: [compute]
  - operations: [add]
  - execution_models: [register_vector]
```

此页尚未进入当前索引。两个真实同名页面构成独立的执行模型验证样本，不能把它算进 140 篇基线。

### 4.2 搬运：只为真正需要方向的 API 添加位置

[asc_copy_gm2ub_align_arch_2201](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_2201.md) 正文明示 GM→UB、非对齐搬运和填充，建议：

```yaml
tag:
  - roles: [data_movement]
  - operations: [copy]
  - source_locations: [global_memory]
  - destination_locations: [unified_buffer]
  - traits: [unaligned_transfer, padding]
```

此例五个键是有用途的局部扩展，不是全项目都填五个键。其手动填充值的前置调用必须留在 dependencies.condition 中，不能用 `padding` 代替。

### 4.3 配置：不伪装成执行搬运

[asc_set_copy_pad_val](../../../docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_copy_pad_val.md) 是设置填充值的接口，当前 operations 本来就是空数组。

```yaml
tag:
  - roles: [data_movement_configuration]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md
    kind: configures
    condition: 该接口与asc_copy_gm2ub_align配合使用，用于设置连续搬运数据块左右两侧的填充值。
```

不新增 `operations: [copy]`，也不为该设置器填 GM→UB。这样配置接口仍能通过角色和依赖查到，却不会混入直接执行拷贝的候选。

### 4.4 定义页：允许没有 tag

[asc_ndim_pad_count_config](../../../docs/zh/api/SIMD-API/c_api/defs/union/asc_ndim_pad_count_config.md) 正文定义的是 union，不是执行 padding 的函数。以下仅示意相关检索字段，完整 YAML 仍遵守其他元数据规范：

```yaml
okftype: reference.type.cpp
title: asc_ndim_pad_count_config
description: 为多维搬运配置各维度左右侧的 padding 元素个数。
# 无 tag；不因为用途涉及 padding 而给类型定义标成执行 API。
```

同步补充样本 [asc_sync_pipe](../../../docs/zh/api/SIMD-API/c_api/sync/intra_core_sync/asc_sync_pipe.md) 明确写“完成同一流水内的同步控制”，足以支持现有 `operations:synchronization`。但当前 roles:control 的定义侧重 runtime/cache state；实施时应在新词表中明确其包括执行顺序控制并复核已有 control 页面，不能静默改变冻结定义或为了该页另创一个重复的 synchronization 角色。此处是必要的语义澄清提案，不是假称当前页已有完整 tag。

## 5. 扩展规则：可选集和候选池分开

### 5.1 三种状态

| 状态 | 是否可写生产 tag | 进入条件 |
| --- | --- | --- |
| 核心 | 是，按适用性要求填写 | 构成稳定的通用发现入口，无法用既有元数据无损代替 |
| 已批准可选 | 是，按用例和正文证据填写 | 语义清晰、存在消费者和对照收益；不要求所有文档都有值 |
| 候选 proposed/deferred | 否 | 有真实需求，但证据、复用性、消费者或收益尚不足 |

本报告只是对批准层级的建议，不生成审批记录、不发布新 frozen 版本。旧冻结版本保留供回退和审计；移出的 families/objects 历史值也不直接销毁。

### 5.2 先判断问题属于哪一层

按以下顺序处理新增需求：

1. **同义表达**：例如 GM/global memory/全局内存指同一概念，增加查询侧别名，不能增加三个标签值。
2. **现有值组合**：寄存器上的加法用 `add + register_vector`，不新建 `register_add`；搬运路径用 copy + source + destination，不枚举 `copy_gm2ub` 等复合操作值。
3. **现有字段的新值**：如果遇到新的、正文明确的存储位置，可提议相应 location 值；source/destination 只增加实际存在的那一侧，不机械成对扩容。
4. **全新字段**：只有无法通过既有字段、描述、signature 或 dependencies 表达的独立检索问题，才申请 namespace；必须说明为什么不是某个 trait、查询别名或关系属性。

### 5.3 候选记录与准入门槛

候选工件保存在调研报告/run 目录，不放入 API front matter。至少记录：`kind`（namespace/value/alias）、`proposal`、`definition`、`selection_rule`、`query_need`、`evidence`（路径、正文片段、源文件哈希）、`consumer`、`comparison`、`decision`。记录内状态用于治理，不成为新的生产 tag 字段。

准入采用以下可执行门槛：

- **真实性**：逐条证据可在目标正文定位；仅同词出现不够，必须证明标签语义。例如调用示例提及另一个 API 不证明本 API 执行同一行为。
- **独立性**：候选不是资源路径、现有值别名或稳定规则可派生的信息。需要语义归并的字段不得以“每页手工派生”规避维护成本。
- **复用性**：通常至少两篇不同 API 的正文支持，不能仅用同一家族的两个架构页面凑数；至少两个查询场景，其中一个不作为候选发现样本。稀有单 API 能力先留候选，通过描述/正文查找。
- **收益**：固定相同语料与任务，至少一个场景减少需要核对的候选，或补回原来缺失的必需项；所有既有验收任务的必需项和替代组覆盖不得下降。不强设“减少 20%”等无业务依据的阈值。
- **落地**：说明哪一个查询入口读取该字段、缺值如何回退、谁维护词表和回归用例；没有消费者就不升级到生产可选集。
- **发布**：复用现有独立证据复核和人工发布机制，发布新版本而非改写 core-v9；变更生产选择器和批量重标注作为明确的迁移步骤。不能将本报告或脚本结果当成人工批准。

可选字段升核心的门槛更高：在计算、搬运、配置/查询/控制三组任务中分别证明其必需性；若只能在某一组证明，改为该组的条件要求，不升级为全局必填。类型定义页与导航页始终按文档角色单独规定。

每次发布重新查看值的查询使用与消融结果。没有出现不等于应该删除；有频率也不等于应该保留。例如当前索引中 `traits:high_dimensional` 覆盖 99/140 页，不能单凭覆盖广就每页必标，必须有“需要高维切分”的消费用例。

### 5.4 首批候选如何处理

| 候选 | 本次建议 | 理由与下一步证据 |
| --- | --- | --- |
| families / objects | deferred | 保留旧词表与标注快照；需要粗分类或特殊对象检索的真实漏检案例后，再验证是否可选准入 |
| traits:intra_core_synchronization | deferred | 现有遗留候选；asc_sync_pipe 支持其动机，但还需独立 API、查询区分收益及消费者 |
| traits:returns_old_value | deferred | 现有遗留候选；应证明会改变原子 API 的选择，不能仅因返回值存在就准入 |
| transfer_paths 关系结构 | deferred | 当前 140 篇原始位置都是单值，没有实测多路径误配；先保存变体出处并正文核实 |
| dtype / archs namespace | reject 为 tag 扩展 | 已有 signature 是更精确的重载级载体 |
| GM、UB 等缩写新值 | reject 为词表值；可作为查询别名 | 增加值不能增加语义信息，反而割裂索引 |

扩展到 C++、SIMT、高阶 API 时，先引入新语料快照和独立任务，验证现有核心适用性、更新归属映射；不要把“在 140 篇 C API 成立”写成跨全部体系的实测结论。现有 api_surfaces 词表也不是所有未来体系的完整列表。

## 6. 对照实验与真实查找轨迹

### 6.1 方法与边界

实验脚本为 [reproduce.py](reproduce.py)，全部结果为 [results.json](results.json)。

实验包括四部分：全量文件/字段统计、原始标注的字段投影、现有索引/CLI 回放、同一候选集合上的过滤与消融。脚本不重建或改写生产索引；只在内存中移除三个字段并构建视图。

对照方案：

| 方案 | 实验定义 |
| --- | --- |
| 现有九字段 | 使用现有生产索引与 CLI；另用相同 facet 函数验证九字段的可表达能力 |
| 仅描述文本 | title + description 子串检索，分别报告直译关键词和扩展关键词；不包括全文检索、BM25、向量检索或 LLM 重排 |
| 仅 operations | 保留操作过滤；无法表达的角色/方向/特征条件跳过，返回更宽候选；允许后续读正文，不能把宽候选当最终误报 |
| 推荐三层 | roles + operations + 当前有证据的四类可选字段；同一数据、同一分词之外的 facet 条件；候选池不参与 |

没有通过重写 description、补充全量标注或改换 gold 来制造 tag 收益。未做端到端代理耗时/token A/B，因此不提供虚构的速度倍数或 token 降幅。

### 6.2 查找轨迹：真实向量加法需求

需求来自旧测试集 A1：实现 `z = x + y`，float 数据从 GM 搬入，在 UB 计算，再写回 GM。它的三个必需目标由 [真实样例](../../../examples/02_simd_c_api/00_introduction/01_add/c_api_sync_add/c_api_add.asc) 中的调用支持。

在仓库根目录实际执行以下查询（完整 JSON 输出保存在 results.cli_trace）：

```bash
python3 okf_skills/okf-index/scripts/query.py --stage in --json
python3 okf_skills/okf-index/scripts/query.py --operation add --json
python3 okf_skills/okf-index/scripts/query.py --operation add --kw 两个源矢量 --json
python3 okf_skills/okf-index/scripts/query.py --stage out --json
```

| 步骤 | 实际候选 | 数量 | 解释 |
| --- | --- | ---: | --- |
| 搬入 | asc_copy_gm2ub、asc_copy_gm2ub_align、asc_ndim_copy_gm2ub | 3 | roles + source/destination 派生 stage；支持相同方向不等于都同样适合简单线性搬运 |
| 加法 | asc_add、asc_add_scalar、asc_add_relu、asc_pair_reduce_sum、asc_repeat_reduce_sum | 5 | add 本身不足以区别向量加、标量加、融合激活及带加法的归约 |
| 限定两个源矢量 | asc_add | 1 | `--kw` 与 operation 取交集；该短语是针对当前描述的具体化查询，不是通用语义理解能力 |
| 搬出 | asc_copy_ub2gm、asc_copy_ub2gm_align | 2 | 搬出目的地是 GM，不能仅检索 copy |
| 正文核实 | 上述 6 个剩余家族/API 的功能说明和约束；搬运家族读具体架构页 | 6 篇 | 选普通搬入/搬出与 asc_add 作为三项主候选；对齐相关接口保留为可选，多维搬运不列简单任务必需项 |

三个主候选的精确路径：

1. `docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub.md`
2. `docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_add.md`
3. `docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm.md`

这里记录的是 API 候选定位，不是可直接运行的完整算子方案。正式使用还需核实同一架构/重载、长度与地址约束及初始化/同步。旧任务把初始化和同步列为两个目录之外，而当前 C API 目录已经存在同步文档；这两种范围不能混淆。

独立回放正文核实阶段共读取 6 个文件，完整文件大小合计 55,032 字节，提取功能/约束文本 10,060 字节；路径和片段保存在 `a1_verification_reads`。这是该回放的实测阅读量，不是各方案都测过的阅读成本。全仓统计扫描成本不混入这段检索阅读量，也不把候选数直接宣称为节省的阅读次数。

### 6.3 相同 A1 任务的初始候选对比

此表在正文核实和“两个源矢量”细化前统计三阶段候选并集，各方案均做三阶段检索（相同的 copy 查询可复用），正文后的最终答案精度不在本表测量。

| 方案 | 初始候选并集 | 必需项覆盖 | 不在 A1 required ∪ optional 的候选 |
| --- | ---: | ---: | ---: |
| 现有九字段 CLI | 10 | 3/3 | 5 |
| 推荐核心 + 按需字段 | 10 | 3/3 | 5 |
| 仅 operations | 13 | 3/3 | 8 |
| 描述文本：加法 / 搬运 | 19 | 3/3 | 14 |
| 描述文本：加法、相加、求和 / 搬运 | 26 | 3/3 | 21 |

“不在任务目标集合”是需要继续甄别的候选，不是经过人工终审的最终误报。推荐方案相对仅操作字段减少 23.08% 的初始候选，相对本次简单文本基线减少 47.37%；这些结果不能外推成优于所有文本检索算法。

也有文本检索表现很好的反例：单查“加法”恰好只返回 asc_add；将路径措辞扩展成“从GM搬运到UB”或“从Global Memory搬运到Unified Buffer”，能与方向字段一样得到 3 个候选。字面 `GM→UB` 则为 0。结构化维度的收益是规范化后表达更稳定、条件可组合，并不意味着每一次都比精调文本查询少候选。

### 6.4 搬运细化与配置关联

补充需求：“查找 GM→UB 的非对齐搬运接口，并找到手动填充值的配套配置。”

在同一 119 个主文档上，推荐字段的精确正匹配实验得到：

```text
operations=copy                                  8
  + source_locations=global_memory
  + destination_locations=unified_buffer         3
  + traits=unaligned_transfer                    1 → asc_copy_gm2ub_align
```

当前 CLI 没有直接的 traits/source/destination 参数，上面是实验脚本对已有标签的过滤，**不是声称生产已有这些参数**。现有 CLI 实际执行 `--stage in --kw 非对齐` 也得到同一个候选。

随后实际执行：

```bash
python3 okf_skills/okf-index/scripts/query.py --deps-of asc_copy_gm2ub_align --kind configures --reverse --json
```

返回 4 个配置 API：`asc_set_copy_pad_val`、`asc_set_gm2ub_loop1_stride`、`asc_set_gm2ub_loop2_stride`、`asc_set_gm2ub_loop_size`。读取各边 condition，即可识别填充值配置为第一个。正文进一步证明：手动填充前调用该接口；不调用时硬件使用 dummy 值。这一前置条件由关系证据承担，不应继续扩充 tag 来代替。

### 6.5 逐字段消融：再删一项会怎样

下表移除某个维度后，跳过无法表达的过滤条件，而不是返回错误或人为置零。其余输入保持相同。

| 被删除字段 | 具体查询 | 完整字段候选数 → 删除后 | 含义 |
| --- | --- | ---: | --- |
| operations | 加法 | 5 → 119 | 失去稳定的操作入口；可退回文本，但已经是另一种检索策略 |
| roles | 搬运配置 | 10 → 119 | 无操作值的设置器不能通过操作集合细分；正文可补救，但角色入口丢失 |
| source_locations | copy + GM→UB | 3 → 5 | UB→UB 等来源不同的搬运也进入候选 |
| destination_locations | copy + UB→GM | 2 → 5 | UB→UB、UB→L1 等目的地不同的搬运也进入候选 |
| traits | 非对齐 GM→UB | 1 → 3 | 无法在方向一致的候选中仅保留已证实支持非对齐的接口 |
| execution_models | 同名 add，要求寄存器模型 | 1 → 2 | 来自两个真实页面的独立小样本；不与 140 篇指标混算 |
| api_surfaces | 本次各过滤任务 | 不变 | 此范围内所有页面同属 c_api；跨体系由派生维度承担 |
| families / objects | 本次各过滤任务 | 不变 | 当前没有证明其独立收益；不是宣称永远无用 |

现有操作标签有粒度问题：add 也标在归约页上，因此 elementwise_add 的候选数为 3，去掉 elementwise 为 5。该发现支持按需保留 traits，但尚不足以直接清洗旧 operations 标注。本轮不把重新标注带来的收益混进字段删除实验。

### 6.6 存储与维护信息量

| 指标：140 篇原始 Markdown | 当前 | 推荐结构的保守投影 | 变化 |
| --- | ---: | ---: | ---: |
| 全部字段键（含空占位） | 1,260 | 502 | -60.16% |
| 非空字段 | 778 | 502 | -35.48% |
| 标签值记录 | 952 | 653 | -31.41% |
| 平均非空字段/篇 | 5.56 | 3.59 | -1.97 |

这里“保守投影”只去掉三项字段并省略空数组，**保留其余所有现有可选值**，尚未按新门槛逐项清理 traits；所以可复现且不依赖主观重标注。502 不代表建议未来每篇都写所有可选字段。仅留两项核心会降至 230 个非空字段、244 个值，但会失去上面的方向等消歧能力，因此不作为完整迁移方案。

这证明的是需维护的声明数量下降，而不是已测量的人力节省。没有逐项标注耗时或错误率实验，不能把 31.41% 值减少等同于 31.41% 人工/token 节省。

## 7. 相比其他设计的收益与代价

| 方案 | 优点 | 具体不足 | 本方案的选择 |
| --- | --- | --- | --- |
| 九字段全部必填 | 外形统一，过滤潜力丰富 | 482 个空占位；规则互相冲突；定义页和设置器被迫填不适用概念 | 按文档角色和检索用途规定条件，不强制信息不存在的字段 |
| 九字段全部选填 | 改动较小，省掉空值 | 缺乏稳定操作/角色入口；保留三类当前无消费或可派生的手工维度 | 用两项核心锚定入口，其余必须证明用途 |
| 只留 operations | 最容易标注 | 方向/模型/特征和配置角色丢失；A1 候选 13，而推荐为 10 | 用可选维度补回实际需要的区分能力 |
| 只留 description | 表达自由，个别精确短语效果很好 | 同义词、方向和执行/配置混杂；本次简单关键词基线 19 个候选 | 保留 description，与稳定受控维度配合，而非替换描述 |
| 单个扁平 keywords | 键最少、扩展容易 | compute/copy/GM 混为同级词；无法校验来源与目的地；少键不等于少信息 | 保持 namespace 和受控值的正交关系 |
| 复合值，如 copy_gm2ub | 看似只需一个操作字段 | 每个操作 × 路径 × 模型需要新值；难以只查所有 copy 或所有 GM 来源 | 使用独立维度组合，避免枚举乘积 |
| 一个无方向 locations 数组 | 少一个 namespace | GM→UB 与 UB→GM 相同，直接损失方向 | 保留 source/destination 两项局部字段 |

选择的代价是校验器需要支持适用性、查询器需要正确处理未知项和变体出处，治理也要维护候选证据。这些工作值得做，是因为现有“一律填九项”已经在真实数据中被空数组绕开；继续只修改文案无法解决契约冲突。

## 8. 后续整改落地顺序与验收

以下为实施建议，本次不执行生产迁移。

1. **冻结证据与发布契约**：保留 core-v9 及现有标注快照，发布新的 schema/词表版本；定义 roles 全局可调用要求、operations 条件要求和四类扩展选择规则。核心/可选元数据只在词表维护，API YAML 外形保持不变。
2. **先改消费者和验证器**：加载“全部允许字段”与“适用时必需字段”两个不同集合；取消 namespaces 必须等于 required_namespaces 的假设。controller 不再自动补空；索引以缺省空集合安全读取可选字段，但未知值、重复 namespace 和重复值仍报错。`load_raw_documents` 对不要求 description/tag 的纯索引页不能直接下标取值。
3. **查询与派生**：保留 stage/operation/category/dtype/convert/api/deps/gap 语义。新增通用重复参数 `--tag namespace:value`，同 namespace 多值 OR、不同 namespace AND；仅允许生产受控字段。过滤先在同一个真实变体内联合判断，再折叠家族，避免跨变体能力拼接。输出区分命中证据和待确认项。api_surfaces 作为资源归属派生维度继续可查，不写回手工 tag。
4. **小范围迁移再扩展**：先对这 140 篇做保守投影并验收，再逐字段按需清理；698 篇未标注文档另行补证入库，单独报告覆盖收益，不计入 tag 精简收益。保留旧索引与版本选择方式以便回退。
5. **同步规范与入口**：字段规范、标签说明、taxonomy 路径、索引/检索说明、评测范围统一为同一版本；消除两个词表副本的事实源歧义。候选调研脚本不得将 provisional 直接发布。

兼容与验收应覆盖：

- 旧九字段文档可由迁移适配器读取；新字段省略不会 KeyError；未知字段/值仍不能无声通过。
- 普通计算页缺 operations、可调用页缺 roles 应被标为不完整；配置器不需要虚构 operations；定义/索引页不要求 tag。
- 已知方向正匹配、位置缺失回退、反向搬运、单边位置、变体跨组合、非对齐和填充条件分别验证。
- 原有全部视图、查询结果和依赖条件保持；新通用 tag 过滤测试角色、执行模型和 traits；不复用本次未经批准的候选值。
- A1 三项必需候选和旧任务替代组不丢；新增同步、寄存器和定义页用当前正文单独建立 gold；缺口输出区分“未入索引”与“C API 没有文档”。
- 词表冻结/候选拒收、逐字证据匹配、发布版本与回滚路径均可验证。角色 control 的同步语义澄清必须有审核和回归，不能静默覆盖原定义。

## 9. 复现与限制

在仓库根目录执行：

```bash
python3 okf_skills/reports/tag-minimal-set/reproduce.py --output okf_skills/reports/tag-minimal-set/results.json
```

依赖为项目已有的 Python/PyYAML。脚本只读取生产资产，只允许将结果写到本报告目录的 JSON 文件；不指定 `--output` 时输出到标准输出。结果包含 838 篇文件与关键输入的 SHA-256、原始/聚合统计、10 条实际 CLI 查询、六组 facet 消融、文本基线、A1 候选、正文读取片段及四条补充证据。脚本校验旧/新 9 类视图、10 条查询结果一致，并校验补充正文证据确实存在。

读者可据以下限制使用结论：

- 标签缩减统计和结果等价在本地真实 140 篇语料上成立；未验证全 DevKit 的最终最小集合。
- 六组 facet 查询是机制实验，A1 是一个有既有 gold 的组合任务；不构成完整的 20 任务端到端评测。
- 只用两个 add 页面验证执行模型，类型定义/同步样本用于覆盖例外设计，不能据此报告全类别召回率。
- 扩展后筛选收益以现有标签正确为前提；本次没有逐条重新审定 952 条旧标签证据。
- 既有 CLI 本身也可以通过 stage 与描述实现部分相同效果；因此本次整改最可靠的收益是**在保持已有检索行为的前提下减少维护声明，并为按需扩展建立明确准入规则**。
