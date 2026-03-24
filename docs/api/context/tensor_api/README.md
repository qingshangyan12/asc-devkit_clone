# Tensor API

Tensor API 是一个基于 Ascend C 的 C++ 模板库，旨在为昇腾 AI 处理器提供类型安全、可组合的张量编程抽象。它通过 Layout​ 描述张量的内存排布格式，结合 Tensor​ 提供统一的数据访问接口，并利用 Atom​ 抽象封装底层硬件操作，使开发者能够在高层以声明式的方式表达计算逻辑。

Tensor API 的核心价值在于将复杂的硬件细节（分形内存布局、多级存储层次、指令参数配置）封装在底层，同时通过模板参数暴露足够的控制点，让高级用户能够对硬件行为进行精细调优。这种设计使得同一套代码可以在不同昇腾处理器架构上运行，同时保证性能。

该库特别适用于矩阵乘法及量化计算、以及需要充分利用 Cube 计算单元的自定义算子开发场景。通过 Traits 机制，用户可以灵活配置矩阵计算的分块大小、数据类型等参数，实现性能与精度的平衡。

## 产品支持情况

| 产品                                        | 是否支持 |
| ------------------------------------------- | :------: |
| Ascend 950PR/Ascend 950DT                   |    √     |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 |    x     |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 |    x     |
| Atlas 200I/500 A2 推理产品                  |    x     |
| Atlas 推理系列产品AI Core                   |    x     |
| Atlas 推理系列产品Vector Core               |    x     |
| Atlas 训练系列产品                          |    x     |
| Atlas 200/300/500 推理产品                  |    x     |

## 接口结构

<img class="eddx" id="image292993463310" src="figure/tensor_api_arch.png">

### 硬件指令

硬件抽象层直接映射到昇腾处理器的硬件指令，针对不同架构提供特化实现。

- **Operation**: 定义操作的类型，如 `CopyGM2L1`、`CopyL12L0`、`MmadOperation`
- **Traits**: 定义操作的特征参数，包括数据类型、布局格式、对齐要求等，如 `MmadTrait`、`LoadDataTrait`、`DataCopyTrait`
- **Atom**: 封装 Operation 和 Traits，提供统一的调用接口

### 逻辑抽象

基于 Atom 组合实现常用计算模式，提供开箱即用的高层 API。

- **Copy**: 通用数据搬运算法
- **Mad**: 矩阵乘加算法

## 测试和样例

本仓库提供[测试用例](../../../../tests/api/tensor_api) 以及[示例代码](../../../../examples/02_features/05_tensor_api)，方便快速了解功能用法并进行验证。

## 文档

- [Layout](Layout和层次化表述法.md)：张量的内存排布描述，由 Shape（形状）和 Stride（步长）组成。支持层次化表达，可描述多重分形格式。
- [Tensor]()：包含指向张量位置的指针，也包含一个用于访问其元素的Layout。
- [Copy](数据搬运简介.md)：数据搬运算法，封装多级存储间的数据传输。包括 DataCopy（GM→L1）、LoadData（L1→L0A/L0B）、FixPipe（L0C→OUT）等通路。
- [Mmad]()：矩阵乘加算法，封装 MMAD 指令。通过 Traits 配置数据类型、分块大小、偏置等参数。
- [接口列表](tensor_api_list.md)：介绍项目提供的api信息，方便快速查询。
- [算子开发指南]()：介绍基于tensor api的算子开发基本流程。

## 目录结构

``` text
include/tensor_api/
├── tensor.h                              # 总入口头文件
├── algorithm/                            # 高层算法接口，包括Copy和Mad
├── atom/                                 # 原子操作接口，包括CopyAtom 搬运原子操作和MmadAtom 矩阵计算原子操作
├── arch/                                 # 架构相关接口和实现
├── tensor/                               # 张量类型：Layout、Tensor、Pointer等核心类型
└── utils/                                # 工具类型接口
```
