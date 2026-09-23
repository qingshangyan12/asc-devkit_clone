# OKF 字段规范

## 目的

本规范用于描述算子检索所需的 C API 文档信息。YAML 头只提供检索入口，接口细节、限制和示例仍以原 Markdown 正文为准。

## 顶层格式

YAML front matter 放在 Markdown 文件第一行，以 --- 包围。只使用以下字段：

| 字段 | 类型 | 必需 | 适用范围 | 含义 |
| --- | --- | --- | --- | --- |
| okftype | string | 是 | 全部 | 文档角色和 API 语言类型：reference.api.cpp、reference.arch.cpp、reference.index.cpp 或 reference.type.cpp。 |
| title | string | 是 | 全部 | 文档一级标题或 API 名称。 |
| resource | string | 是 | 全部 | 从 asc-devkit 开始的仓库相对路径，使用 /。 |
| description | string | API/架构页必需，索引页可省略 | API/架构页 | 由 description Agent 基于功能说明等正文证据生成的检索摘要，不得引入正文没有的能力。 |
| tag | object[] | 是 | API/架构页 | 受控检索标签，每项为 namespace 及其标签内容数组。 |
| signature | object[] | API/架构页可选 | API/架构页 | 调用模式或 overload 组。函数项包含 prototype_id 或 override_id、archs、headers；含 `<dtype>` 的 prototype_id 在同一条目中添加 dtype；函数宏项只包含 macro_id。 |
| dependencies | object[] | 含明确关联时填写 | 全部 | 有正文依据的关联文档，包含 path、kind，必要时包含 condition。 |
| examples | object[] | 含示例时填写 | API/架构页 | 样例工程或源码位置和明确的示例架构，包含样例 path，可选 file、archs、override_id。 |
| headers | string[] | 可选 | 非 callable 定义页 | 唯一确认的真实声明头文件，使用 `/include/...` 路径。 |
| declaration | object | 定义页可选 | enum、record、constant、type_alias、对象宏 | 从正文定义代码块提取的受限声明结构。 |
| type_catalog | object | 类型目录页可选 | `c_api/defs/type` | 正文表格中的数据类型分组。 |
| timestamp | string | 是 | 全部 | 元数据生成日期。 |

空的可选数组或映射可以省略。不得添加未在本规范定义的顶层字段。

## 定义页

`c_api/defs` 的实体页不是函数 API。enum、struct/union、constant 和 `using` 类型别名仅在正文定义与唯一
声明 header 同时确认时写 `declaration` 和 `headers`。`declaration` 至少包含
`symbol`、`cpp`；enum 包含 `members`，record 包含 `fields` 和 `record_kind`，constant 可包含 `type`、`value`，
type alias 包含 `target`。数据类型目录使用 `type_catalog.groups`，不要求 header。

函数宏仍使用 `signature` 中唯一的 `macro_id`，不附加 header；对象宏使用 `headers` 和
`declaration.macros`。定义页没有函数原型、不可调用样例或 Doxygen 目标是正常情况，不能由此伪造 `signature`、
example marker 或函数 Doxygen。

## 签名与架构

signature 必须按调用模式拆分，例如 contiguous、strided、scalar、mask、reduce 或 config。
正文明确给出 `<dtype>` 占位符类型列表时，在对应 signature 条目中以与 `prototype_id` 同级的单行数组写入，
例如 `dtype: [half, float]`；不得在函数原型前插入 YAML 代码块。不同原型的 dtype 集合必须分别保留，
不得按整页合并。archs 只记录正文、文件名或编译命令明确给出的 dav-* 架构；产品支持区中机器可读的
`<!-- npu="..." -->` 支持标记可按既有架构映射使用。不得从其他产品状态文案推断架构。

headers 保存真实声明文件路径，例如 /include/c_api/vector_compute/compute/vector_arith.h。文档给出的
聚合头只是查找范围；只有在该头或其递归 include 闭包中唯一匹配的声明 header 才能写入 signature。

对已验证且带命名参数的具体函数声明，include Doxygen 必须从本页“参数说明”表提取每个参数的方向和描述，
并写为 `@param [in/out]`。只有一张标准表完整覆盖该声明的全部命名参数时才写入；表缺失、覆盖不全或方向无法
确定时，生成器必须报告 `include_doxygen` 问题，不能静默生成仅含 ID 的注释。

标识符分类沿用生成器 `identifier_field` 规则：保留文档占位符时使用 prototype_id，表示模板或泛化原型；
没有文档占位符时使用 override_id，表示具体类型重载。C++ 模板参数 T、S、U 不视为文档占位符。类型无关的函数宏
使用 macro_id（形式为 `宏名__形参名...`），不混入函数 ID。遇到无法由正文、
头文件或样例唯一证明的标识符、头文件、类型或架构时，保留可确认字段并省略不确定字段，不得猜测。

同目录的同一 API 若以 `_arch_<型号>` 区分文档，且两个及以上型号文档包含相同 concrete override_id，
每个冲突项必须在 ID 后追加该文档型号，例如 `asc_copy_gm2ub__PUv_PGv_u32@2201` 和
`asc_copy_gm2ub__PUv_PGv_u32@3510`。后缀仅用于已证实的跨型号冲突，`prototype_id`、`macro_id` 和不冲突的
`override_id` 不追加；它不是新增 YAML 字段。该 ID 用于 signature 和对应的 example marker。若两个记录命中
同一物理 header 声明，include 按旧规则合并为一个 canonical `@override_id` 并汇总 `@archs`；若命中不同物理声明，
include 则保留各自带 `@型号` 后缀的 ID。

## 类型定义页（reference.type.cpp）

适用 `defs/` 下的常量、枚举、宏、类型别名、联合体定义页。规则：

- description 取自功能说明或定义说明（常量定义/宏定义/枚举类具体定义/类型别名具体定义/结构体具体定义）章节，压缩为一句话检索摘要。
- tag 的 `api_surfaces` 和 `roles` 全局必填；api_surfaces 值由稳定资源路径证明。objects、families 等其他 namespace 仅在有逐字正文证据时填写，没有证据时省略。
- 不写 signature（无函数原型）；dependencies/examples 按正文证据正常处理。
- 枚举值详解中的产品差异注释不作为 archs 证据；架构支持情况不写入 YAML（products 字段本期不引入）。

## 标签

标签必须使用唯一冻结词表中的受控命名空间和值。标签不预先在本文档中扩大枚举值。

tag 允许使用 taxonomy 中声明的 namespace。`api_surfaces` 和 `roles` 是全局必填；当 roles 包含 `compute`、`data_movement` 或 `data_initialization` 时，`operations` 必填；当 roles 包含 `compute` 时，`execution_models` 必填。其余 namespace 仅在正文证据和实际检索用途同时成立时填写。不适用 namespace 省略，不得使用空数组、unknown 或 other 逃避条件要求。
每个标签都必须有目标 Markdown 正文中的逐字证据；api_surfaces 等允许由稳定路径证明的导航标签可以使用路径证据。
没有明确证据的 API 不得按目录、名称、dtype、架构、头文件属性或样例注释推断标签。

标签使用主题加内容的形式，每个 namespace 只出现一次，值使用数组表示：

    tag:
      - api_surfaces: [c_api]
      - roles: [compute]
      - families: [arithmetic]
      - operations: [divide]
      - objects: [vector]
      - execution_models: [register_vector]
      - source_locations: [vector_register]
      - destination_locations: [vector_register]
      - traits: [elementwise, masked]

同一 namespace 的多个值必须放在同一个数组中，例如 traits: [elementwise, masked]。不得重复书写同一
namespace，也不得使用 src、dst 等位置标签缩写。位置标签必须使用 source_locations 和
destination_locations 主题。

traits:deprecated 仅用于目标 Markdown 明确声明该 API 已废弃的情况。文件名中的 _deprecated、
替代接口链接、头文件属性和样例注释都不是充分证据。

## 关联与示例

dependencies.kind 使用 requires、configures、constrained_by 或 related：

- requires：使用当前 API 前必须满足的 API 前置关系。
- configures：配置 API 与目标执行 API 的关联。
- constrained_by：当前 API 受另一 API、模式或约束文档限制。
- related：相关 API 的检索导航，不表示固定调用顺序。

只有正文或样例明确说明“必须先调用”时才使用 requires。related 和其他关联只用于检索导航，
不表示固定调用顺序。

dependencies.path 使用 DevKit 根目录相对路径，并且必须指向已存在的 .md 文件。condition 只保留
正文中的适用条件，不由工具扩写。

examples.path 必须指向 DevKit 根目录下的样例工程目录或样例源码文件，不能指向当前 API Markdown。
路径使用仓库根目录形式，通常以 `/examples/` 开头。file 是相对于该样例 path 的源码文件名；同一个样例工程
包含多个重载时，可以使用多个 examples 项分别关联具体源码文件和 override_id。只有示例正文明确指定架构时才填写
archs。override_id 必须与 signature 和样例源码中的调用标记对应；跨型号消歧时二者均使用相同的 `@型号` 后缀。
同一物理 include 声明由多个架构页共享时，Doxygen 使用合并的 canonical ID 和 `@archs`，不重复写声明注释。
样例调用标记必须为成对的 ASCII `//! [override_id]`，直接包裹调用所在源码行；不得使用自定义 marker 文本、
占位符 ID 或对同一调用编造多个标签。

## 示例

    ---
    okftype: reference.api.cpp
    title: asc_div
    resource: asc-devkit/docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_div.md
    description: 执行矢量除法运算。
    tag:
      - api_surfaces: [c_api]
      - roles: [compute]
      - families: [arithmetic]
      - operations: [divide]
      - objects: [vector]
      - execution_models: [register_vector]
      - source_locations: [vector_register]
      - destination_locations: [vector_register]
      - traits: [elementwise, masked]
    signature:
      - prototype_id: "asc_div__Rv<dtype>_v<dtype>_v<dtype>_vb"
        archs: ["3510"]
        headers:
          - /include/c_api/reg_compute/compute/reg_arith.h
        dtype: [half, float]
    dependencies:
      - path: docs/zh/api/SIMD-API/c_api/vector_compute/vector_mask_config/asc_set_vector_mask.md
        kind: requires
        condition: 使用显式向量掩码时参考
    examples:
      - path: /examples/02_simd_c_api/03_c_api/02_reg_vector_compute/div
        file: div.asc
        archs: ["3510"]
        override_id: "asc_div__Rvf32_vf32_vf32_vb"
    timestamp: '2026-09-18'
    ---
