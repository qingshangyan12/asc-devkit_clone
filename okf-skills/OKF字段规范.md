# OKF 字段规范

## 目的

本规范用于描述算子检索所需的 C API 文档信息。YAML 头只提供检索入口，接口细节、限制和示例仍以原 Markdown 正文为准。

## 顶层格式

YAML front matter 放在 Markdown 文件第一行，以 --- 包围。只使用以下字段：

| 字段 | 类型 | 必需 | 适用范围 | 含义 |
| --- | --- | --- | --- | --- |
| okftype | string | 是 | 全部 | 文档角色和 API 语言类型：reference.api.cpp、reference.arch.cpp 或 reference.index.cpp。 |
| title | string | 是 | 全部 | 文档一级标题或 API 名称。 |
| resource | string | 是 | 全部 | 从 asc-devkit 开始的仓库相对路径，使用 /。 |
| description | string | API/架构页必需，索引页可省略 | API/架构页 | 从功能说明压缩得到的检索摘要，不得引入正文没有的能力。 |
| tag | object[] | 是 | API/架构页 | 受控检索标签，每项为 namespace 及其标签内容数组。 |
| signature | object[] | API/架构页可选 | API/架构页 | 调用模式或 overload 组。每项包含 prototype_id 或 override_id、archs、headers，可包含 dtype。 |
| dependencies | object[] | 含明确关联时填写 | 全部 | 有正文依据的关联文档，包含 path、kind，必要时包含 condition。 |
| examples | object[] | 含示例时填写 | API/架构页 | 示例位置和明确的示例架构，包含 path，可选 anchor、file、archs、override_id。 |
| timestamp | string | 是 | 全部 | 元数据生成日期。 |

空的可选数组或映射可以省略。不得添加未在本规范定义的顶层字段。

## 签名与架构

signature 必须按调用模式拆分，例如 contiguous、strided、scalar、mask、reduce 或 config。
prototypes 保存正文中的精确声明；dtypes 按声明中的参数名映射到数据类型数组，转换接口可以表达
不同输入输出类型。archs 只记录正文、文件名或编译命令明确给出的 dav-* 架构，不从产品状态推断。

headers 保存正文中的头文件字符串，例如 c_api/vector_compute/compute/vector_arith.h，不添加
/include 前缀。signature 中的 headers 使用真实声明文件路径，并保持与正文和 include 声明一致。

prototype_id 标识模板或泛化原型，override_id 标识具体类型重载。遇到无法由正文、头文件或样例唯一证明的
标识符、头文件、类型或架构时，保留可确认字段并省略不确定字段，不得猜测。

## 标签

标签必须使用唯一冻结词表中的受控命名空间和值。标签不预先在本文档中扩大枚举值。

每个 API 必须包含 taxonomy 中标记为 required: true 的 namespace，并且每个必填 namespace 至少有一个值。
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

文档内示例的 path 指向当前 Markdown，可用 anchor 标识调用示例；仓库示例的 path 指向示例目录或源码文件。
只有示例正文明确指定架构时才填写 archs。override_id 必须与 signature 和 include 中的具体声明一致。

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
        dtype:
          values: [half, float]
    dependencies:
      - path: docs/zh/api/SIMD-API/c_api/vector_compute/vector_mask_config/asc_set_vector_mask.md
        kind: requires
        condition: 使用显式向量掩码时参考
    examples:
      - path: docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_div.md
        anchor: call-example
        archs: ["3510"]
    timestamp: '2026-09-18'
    ---
