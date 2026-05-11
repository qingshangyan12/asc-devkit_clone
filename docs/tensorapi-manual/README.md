# Ascend C TensorAPI 开发手册

## 关于本手册

本手册面向昇腾 NPU 开发者，系统介绍 **TensorAPI** 的编程模型、数据结构、API 接口与开发实践。TensorAPI 是 Ascend C 中面向高阶算子开发的核心抽象层，提供**基于 Layout + Tensor 的统一编程范式**，使开发者能以直观的方式操作 NPU 上的多维数据，并在 GM、L1、UB、L0A/B/C 等多级存储层次间进行高效数据搬运与矩阵计算。

读者完成阅读后，应能独立基于 TensorAPI 进行 Ascend C 算子开发。

## 目录结构

| 章节 | 内容 |
|------|------|
| [第一章  概述](01-overview/) | TensorAPI 定位、设计理念与核心理念 |
| [第二章  核心数据结构](02-data-structures/) | Shape / Stride / Layout / LocalTensor / Pointer 类型体系 |
| [第三章  内存层次](03-memory-hierarchy/) | NPU 多级存储架构：GM → L1 → UB / L0A / L0B / L0C |
| [第四章  Layout 分形格式](04-layout-reference/) | NZ / ND / DN / ZZ / ZN / NN / ScaleA / ScaleB 等分形数据格式 |
| [第五章  API 参考](05-api-reference/) | 构造 API、查询 API、Tensor 操作 API、Copy 与 MMAD 算法 API |
| [第六章  编程实战](06-programming-guide/) | 快速上手、数据搬运、矩阵计算、最佳实践 |
| [附录](appendix/) | 术语表、类型速查表 |

## 快速导航

- **第一次接触**：从 [第一章 §1.1 TensorAPI 简介](01-overview/01-what-is-tensorapi.md) 开始
- **理解数据抽象**：阅读 [第二章 §2.2 Layout](02-data-structures/02-layout.md)
- **了解 NPU 存储**：阅读 [第三章 内存层次](03-memory-hierarchy/01-memory-model.md)
- **快速上手**：跳转到 [第六章 §6.1 快速上手](06-programming-guide/01-quick-start.md)

## 适用对象

- 具备 C++ 模板编程基础，希望使用 Ascend C 进行 NPU 算子开发的工程师
- 从 Ascend C 基础 API (Basic API) 迁移到 TensorAPI 的开发者
- 需要深入理解 NPU 数据布局与分形格式的架构师

## 命名空间

本手册中所有 API 均位于 `AscendC::Te` 命名空间下。使用方式：

```cpp
#include "include/tensor_api/tensor.h"
using namespace AscendC::Te;
```
