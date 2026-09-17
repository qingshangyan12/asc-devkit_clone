# OKF 字段规范

## 目的

本规范用于描述算子检索所需的 C API 文档信息。YAML 头只提供检索入口，接口细节、限制和示例仍以原 Markdown 正文为准。

## 顶层格式

YAML front matter 放在 Markdown 文件第一行，以 `---` 包围。只使用以下字段：

| 字段 | 类型 | 必需 | 适用范围 | 含义 |
| --- | --- | --- | --- | --- |
| `type` | string | 是 | 全部 | 文档角色和 API 语言类型：`reference.api.Cpp`、`reference.arch.Cpp` 或 `reference.index.Cpp`。 |
| `title` | string | 是 | 全部 | 文档一级标题或 API 名称。 |
| `resource` | string | 是 | 全部 | 仓库根目录相对路径，使用 `/`，必须位于本期范围内。 |
| `description` | string | API/架构页必需，索引页可省略 | API/架构页 | 从“功能说明”压缩得到的检索摘要，不得引入正文没有的能力。 |
| `tags` | string[] | 是 | 全部 | 受控检索标签，格式为 `namespace:value`。 |
| `products` | map | API/架构页可选 | API/架构页 | 只允许 `A5`、`A3`、`A2` 三个键，值为 `supported` 或 `unsupported`。 |
| `signatures` | object[] | API/架构页可选 | API/架构页 | 调用模式或 overload 组。每项包含 `id`、`prototypes`、`dtypes`、`archs`、`headers`。 |
| `dependencies` | object[] | 含明确关联时填写 | 全部 | 有正文依据的关联文档，包含 `path`、`kind`，必要时包含 `condition`。 |
| `examples` | object[] | 含示例时填写 | API/架构页 | 示例位置和明确的示例架构，包含 `path`，可选 `anchor`、`file`、`archs`。 |
| `status` | string | 是 | 全部 | OKF 知识状态：`unconfirmed`、`confirmed` 或 `stale`。不表示产品支持状态。 |

空的可选数组或映射可以省略。不得添加未在本规范定义的顶层字段。

## 签名与架构

`signatures` 必须按调用模式拆分，例如 `contiguous`、`strided`、`scalar`、`mask`、`reduce` 或 `config`。`prototypes` 保存正文中的精确声明；`dtypes` 按声明中的参数名映射到数据类型数组，转换接口可以表达不同输入输出类型。`archs` 只记录正文、文件名或编译命令明确给出的 `dav-*` 架构，不从产品状态推断。

`headers` 保存正文中的头文件字符串，例如 `c_api/vector_compute/compute/vector_arith.h`，不添加 `/include` 前缀。

## 标签

标签必须使用受控命名空间。阶段0定义命名空间，不预先扩大枚举值：

`stage`、`category`、`operation`、`mode`、`memory`、`pipeline`、`behavior`、`dtype`、`sync`。

阶段2负责调研并固化各命名空间的候选值。标签不能代替 `signatures` 中的精确接口事实。

## 关联与示例

`dependencies.kind` 使用 `requires`、`configures`、`constrained_by` 或 `related`。`related` 和其他关联只用于检索导航，不表示固定调用顺序。

文档内示例的 `path` 指向当前 Markdown，可用 `anchor` 标识“调用示例”；仓库示例的 `path` 指向示例目录或源码文件。只有示例正文明确指定架构时才填写 `archs`。

## 示例

```yaml
---
okftype: reference.api.Cpp
title: asc_div
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_div.md
description: 执行矢量除法运算。
tags:
  - stage:compute
  - category:arithmetic
  - operation:divide
  - behavior:elementwise
  - pipeline:PIPE_V
archs: ["3510"]
signatures:
  - id: asc_div.contiguous
    prototypes:
      - "__aicore__ inline void asc_div(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)"
      - "__aicore__ inline void asc_div(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)"
    dtypes:
      dst: [half, float]
      src0: [half, float]
      src1: [half, float]
    archs: []
    headers:
      - c_api/vector_compute/compute/vector_arith.h
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/vector_compute/vector_mask_config/asc_set_vector_mask.md
    kind: related
    condition: 使用显式向量掩码时参考
examples:
  - path: docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_div.md
    anchor: call-example
    archs: []
status: unconfirmed
---
```
