# Tensor API

Tensor API 是一套基于 Ascend C 的 C++ 模板抽象库，专为定义和操作层次化多维数据布局而设计。Tensor API 提供 **Layout** 和 **Tensor** 对象，将数据类型、形状、存储空间和内存布局紧凑地封装在一起，并为用户自动处理复杂的索引计算，让开发者能以直观、逻辑化的方式访问数据，而无需手动计算底层地址。

Tensor API 的核心抽象是**Layout（层次化多维布局）**，它可以与数据指针组合来表示张量。这种布局表达方式足够强大，能够表示实现高效稠密线性代数所需的几乎所有场景。在将复杂的硬件细节（分形内存布局、多级存储层次、指令参数配置）封装在底层的同时，通过模板参数暴露足够的控制点，让高级用户能够对硬件行为进行精细调优。Atom（原子操作）是Tensor API对底层硬件指令的抽象封装，它将操作类型和特征参数组合在一起，执行的最小单元的计算或数据搬运操作。

Tensor API当前仅支持Ascend 950PR/Ascend 950DT。

## 接口结构

<img class="eddx" id="image292993463310" src="figure/tensor_api_arch.png">

### 硬件指令

硬件抽象层直接映射到昇腾处理器的硬件指令，针对不同架构提供特化实现。

- **Operation**: 定义操作的类型，框架内置了一系列预设操作，用于指定计算或数据搬运的通路，如 `CopyGM2L1`、`CopyL12L0`、`MmadOperation`等
- **CopyTraits** / **MmadTraits**: 包含Operation，并提供自定义Trait类型，配置数据类型、布局格式、对齐要求等

### 逻辑抽象

**Atom** 封装CopyTraits或MmadTraits，统一搬运或计算操作：

- `CopyAtom`: 数据搬运指令
- `MmadAtom`: 矩阵乘加计算指令

### 接口调用

调用上层接口，传入Atom以及待搬运或计算的张量：

- **Copy**: 通用数据搬运算法
- **Mad**: 矩阵乘加算法

## 测试和样例

本仓库提供[测试用例](../../../../tests/api/tensor_api) 以及[示例代码](../../../../examples/02_features/05_tensor_api)，方便快速了解功能用法并进行验证。

## 文档

- [Layout](Layout和层次化表述法.md)：张量的内存排布描述，由 Shape（形状）和 Stride（步长）组成。支持层次化表达，可描述多重分形格式。
- [Tensor]()：包含指向张量位置的指针，也包含一个用于访问其元素的Layout。
- [Copy](数据搬运简介.md)：数据搬运算法，封装多级存储间的数据传输。包括 DataCopy（GM→L1）、LoadData（L1→L0A/L0B）、FixPipe（L0C→OUT）等通路。
- [Mad]()：矩阵乘加算法，封装 MMAD 指令。通过 Traits 配置数据类型、分块大小、偏置等参数。
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
