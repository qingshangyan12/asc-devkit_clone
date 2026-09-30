# OKF 字段规范

OKF YAML 是 API Markdown 的检索元数据；接口细节以正文为准。


## 顶层格式

YAML front matter 位于 Markdown 开头，以 `---` 包围。

| 字段 | 类型 | 含义 |
| --- | --- | --- |
| `okftype` | string | 文档族与页面种类，格式为 `文档族.页面种类`。 |
| `title` | string | 页面标题。 |
| `resource` | string | 以 `asc-devkit/` 开头的仓内文档路径。 |
| `description` | string | 页面内容摘要。 |
| `tag` | object[] | 受控语义标签，每项对应一个 namespace 及其值列表。 |
| `signature` | object[] | 函数、重载或函数式宏的标识及适用信息。 |
| `archs` | string[] | 页面内签名共同适用的架构。 |
| `header` | string[] | 共同声明头文件或定义头文件，使用 `/include/...` 路径。 |
| `declaration` | object | 常量、枚举、记录、类型别名或对象式宏的声明摘要。 |
| `type_catalog` | object | 数据类型分组。 |
| `dependencies` | object[] | 关联文档及关系类型。 |
| `examples` | object[] | 相关样例工程或源码。 |
| `navigation` | object[] | 索引页链接的子文档。 |
| `version` | string | 接口原型的 CANN 版本生命周期区间，如 `9.0.0-` 或 `9.0.0-9.2.0`。 |


## 子字段

| 字段 | 含义 |
| --- | --- |
| `signature[].prototype_id` | 含 `<dtype>` 等文档占位符的泛化原型 ID。 |
| `signature[].override_id` | 具体函数重载 ID；跨架构冲突时可带 `@型号` 后缀。 |
| `signature[].macro_id` | 函数式宏 ID，包含宏名和形参名。 |
| `signature[].dtype` | 当前泛化原型支持的类型列表。 |
| `signature[].archs`、`signature[].header` | 当前签名适用的架构和声明头文件；共同值可上提到顶层。 |
| `declaration` | 实体声明可带 `symbol`、`value`、`type`、`target` 或 `record_kind`；对象式宏使用 `macros`。 |
| `type_catalog.groups` | 数据类型分组及各组的 `types`。 |
| `tag.traits: deprecated` | 当前页描述的实体本身已废弃；标注其他接口废弃的宏不属于此类。 |
| `dependencies[].kind` | `requires` 前置、`configures` 配置、`constrained_by` 受约束、`related` 相关、`replaced_by` 当前废弃实体指向替代目标。 |
| `examples[]` | `path`，以及可选的 `file`、`archs`、`override_id`。 |
| `navigation[]` | 子文档的 `path`。 |

字段适用范围和输出顺序见 [Profile 注册表](okf-yaml-generate/config/document-profile-registry.yaml)；
标签词表及证据约束见 [tag 规则](okf-tag-research/taxonomy/tag-rules.yaml)。


## 示例

```yaml
---
okftype: reference.api
title: asc_div
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/reg_compute/reg_arith/asc_div.md
description: 按掩码对源矢量寄存器逐元素求商。
tag:
  - api_surfaces: [c_api]
  - roles: [compute]
  - operations: [divide]
  - execution_models: [register_vector]
  - traits: [elementwise, masked]
signature:
  - prototype_id: "asc_div__Rv<dtype>_v<dtype>_v<dtype>_vb"
    dtype: [int16_t, uint16_t, half, int32_t, uint32_t, float]
  - prototype_id: "asc_div__v<dtype>_v<dtype>_vb"
    dtype: [int16_t, uint16_t, half, int32_t, uint32_t, float]
archs: ['3510']
header: [/include/c_api/reg_compute/compute/reg_arith.h]
examples:
  - path: /examples/02_simd_c_api/03_c_api/02_reg_vector_compute/div
    file: div.asc
    archs: ['3510']
    override_id: asc_div__Rvf32_vf32_vf32_vb
version: 9.0.0-
---
```

## okftype

第一段是文档四象限中的文档族，以用途区分：

| 文档族 | 对 Agent 的价值 |
| --- | --- |
| `tutorial` | 可抽取为复用的工作流模板。 |
| `guide` | 提供决策分支和兜底信号。 |
| `explanation` | 关键结论前置，辅助判断。 |
| `reference` | 将事实组织为可转换成工具调用的 schema。 |

当前生成器只支持 `reference`。第二段表示页面种类：

| 页面种类 | 内容 |
| --- | --- |
| `api` | 可调用接口。 |
| `constant`、`enum`、`macro`、`type`、`union` | `macro` 是预处理器宏，`type` 是数据类型目录。 |
| `index` | 导航页。 |
| `concept` | 概述或机制说明。 |
| `catalog`、`release` | 注册表预留的汇总和版本页。 |
