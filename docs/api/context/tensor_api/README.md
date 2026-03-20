# Tensor API

Tensor API 是一个基于 Ascend C 的 C++ 模板库，旨在为昇腾 AI 处理器提供类型安全、可组合的张量编程抽象。它通过 Layout​ 描述张量的内存排布格式，结合 Tensor​ 提供统一的数据访问接口，并利用 Atom​ 抽象封装底层硬件操作，使开发者能够在高层以声明式的方式表达计算逻辑。

Tensor API 的核心价值在于将复杂的硬件细节（分形内存布局、多级存储层次、指令参数配置）封装在底层，同时通过模板参数暴露足够的控制点，让高级用户能够对硬件行为进行精细调优。这种设计使得同一套代码可以在不同昇腾处理器架构上运行，同时保证性能。

该库特别适用于矩阵乘法及量化计算、以及需要充分利用 Cube 计算单元的自定义算子开发场景。通过 Traits 机制，用户可以灵活配置矩阵计算的分块大小、数据类型等参数，实现性能与精度的平衡。

## 平台支持

- Ascend A5 (Ascend 950)

## 需要包含的头文件

> [!NOTE]说明
> 包含 tensor.h 文件来调用 Tensor API 相应接口。如无特殊说明，包含该头文件即可满足接口调用需求。
> 若 API 文档中有特殊说明，则应遵循 API 的具体说明。

``` cpp
#include "tensor_api/tensor.h"
```

## 编程模型

Tensor API 采用分层抽象的编程模型，从底层的硬件指令到高层的算法封装，每一层都提供清晰的抽象边界：

### Arch Layer - 硬件抽象层

硬件抽象层直接映射到昇腾处理器的硬件指令，针对不同架构提供特化实现。

- **Mmad**: 矩阵乘累加指令，利用 Cube 计算单元执行高性能矩阵运算
- **LoadData**: 数据加载指令，将数据从 L1 缓存搬运到 L0 操作数寄存器
- **Fixpipe**: 后处理指令，对 L0C 累加器结果进行格式转换、量化、ReLU 等操作后搬出
- **DataCopy**: 通用数据搬运指令，支持不同存储层级之间的数据传输

### Atom Layer - 原子操作层

原子操作层将硬件指令封装为可复用的计算单元，分离操作语义和配置参数。

- **CopyAtom**: 数据搬运原子操作，封装 LoadData、Fixpipe 等搬运指令
- **MmadAtom**: 矩阵计算原子操作，封装 Mmad 指令
- **Traits**: 配置结构体，定义原子操作的具体行为参数

### Algorithm Layer - 算法封装层

算法封装层基于 Atom 组合实现常用计算模式，提供开箱即用的高层 API。

- **Copy**: 通用数据搬运算法
- **Mad**: 矩阵乘加算法

## 核心概念

### 数据结构

#### Layout

Layout 支持层次化表述法以处理复杂的内存排布情况，目的是将逻辑索引空间映射到内存中的物理偏移。Layout由以下两部分组成：

- **Shape**: 张量形状，描述各维度大小
- **Stride**: 内存步幅，描述相邻元素在内存中的间隔

详细文档请参考  [Layout和层次化表述法](struct/defenition/Layout和层次化表述法.md)。

Layout的通用构造方法是传入Shape和Stride，或只传入Shape，自动推导Stride，接口如下：

```CPP
template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride);
template <typename T>
__aicore__ inline constexpr auto MakeLayout(const T& shape);

```
不同地址空间对Layout的形状和布局有限制和要求，用户也可以根据计算的需求指定布局形状。Layout的构建方法提供了指定分型的方法，预定义格式如下表所示：

| 格式 | 说明 | 构造函数 |
|------|------|----------|
| NZ | 内层 Z 字形，外层 N 字形分形 | `MakeNzLayout` |
| ZN | 内层 N 字形，外层 Z 字形分形 | `MakeZnLayout` |
| ND | 行优先格式 | `MakeNDLayout` |
| DN | 列优先格式 | `MakeDNLayout` |
| ZZ | 双 Z 字形分形 | `MakeZzLayout` |
| NN | 双 N 字形分形 | `MakeNnLayout` |
| L0C | L0C 输出格式 | `MakeL0CLayout` |

#### Tensor

Tensor 是 TensorTile API 的核心数据容器，既包含指向张量起始位置的指针，也包含一个用于访问其元素的和内存排布（Layout），提供类型安全的张量访问接口。Tensor 将数据的物理存储位置与其逻辑排布解耦，用户可以通过统一的接口访问数据，无需关心底层的复杂内存布局。

``` text
LocalTensor<TensorAttribute<Engine, Layout>>
    │              │              │
    │              │              └── 描述内存排布格式
    │              └── 数据引擎，封装存储位置和访问方式
    └── 张量属性，组合 Engine 和 Layout
```

[AI Core中包含多级存储](https://www.hiascend.com/document/detail/zh/canncommercial/850/opdevg/Ascendcopdevg/atlas_ascendc_10_0008.html#ZH-CN_TOPIC_0000002534415713__section123639375417)，对于不同存储位置，提供对应的指针构造方法：

- `GMmemPtr<T>`: Global Memory 指针
- `UBmemPtr<T>`: Unified Buffer 指针
- `L1memPtr<T>`: L1 Buffer 指针
- `L0AmemPtr<T>`: L0A Buffer 指针
- `L0BmemPtr<T>`: L0B Buffer 指针
- `L0CmemPtr<T>`: L0C Buffer 指针
- `BiasmemPtr<T>`: BiasTable Buffer 指针
- `FixbufmemPtr<T>`: Fixpipe Buffer 指针

#### 场景举例

如需创建位于 Global Memory 且包含完整布局信息的 Tensor，可按以下示例：

```cpp
    // 1. 为张量数据所在的内存位置创建一个指针
    auto gmPtrA = MakeGMmemPtr(x);
    
    // 2. 结合该指针与指定的形状、步长（Layout）信息，构造完整的 Tensor
    auto gmA = MakeTensor(gmPtrA, MakeLayout(MakeShape(m, k), MakeStride(k, AscendC::Std::Int<1>{})));

```

### Atom 抽象

Atom 是对底层硬件操作的抽象封装。每个 Atom 由 **Operation**（操作类型）和 **Traits**（特征参数）组成，提供统一的可调用接口。

#### 结构说明

- **Operation**: 定义操作的类型，如 `CopyGM2L1`、`CopyL12L0`、`MmadOperation`
- **Traits**: 定义操作的特征参数，包括数据类型、布局格式、对齐要求等，如 `MmadTrait`、`LoadDataTrait`、`DataCopyTrait`
- **Atom**: 封装 Operation 和 Traits，提供统一的调用接口

#### Atom 类型

| Atom | 说明 |
|------|------|
| `CopyAtom<Op, Traits>` | 数据搬运原子操作，支持 GM→L1、L1→L0、L0C→Out 等路径 |
| `MmadAtom<Op, Traits>` | 矩阵乘累加原子操作，封装 Cube 单元的 MMAD 指令 |

#### 基于Atom的数据搬运

数据搬运原子操作，支持 GM→L1、L1→L0、L0C→Out 等路径

| 函数签名 | 说明 |
|---------|------|
| `Copy(atom, src, dst)` | 使用 CopyAtom 执行数据搬运 |
| `MakeCopy(operation)` | 创建 CopyAtom，支持 CopyGM2L1、CopyL12L0A、CopyL12L0B、CopyL0C2Out 等操作 |

**支持的搬运路径**:

| Atom Operation | 路径 | 说明 |
|----------------|------|------|
| `CopyGM2L1` | GM → L1 | 全局内存到 L1 |
| `CopyL12L0A` | L1 → L0A | L1 到矩阵单元 A |
| `CopyL12L0B` | L1 → L0B | L1 到矩阵单元 B |
| `CopyL12BT` | L1 → BT | L1 到 BiasTable Buffer |
| `CopyL12FB` | L1 → FB | L1 到 Fixpipe Buffer |
| `CopyL0C2GM` | L0C → Out | 矩阵结果搬出到 GM |
| `CopyL0C2UB` | L0C → Out | 矩阵结果搬出到 UB |

#### 基于Atom的矩阵乘加

 矩阵乘累加原子操作，封装 Cube 单元的 MMAD 指令

| 函数签名 | 说明 |
|---------|------|
| `Mad(atom, a, b, c, params)` | 使用 MmadAtom 执行矩阵乘加 |
| `MakeMad(operation)` | 创建 MmadAtom |

## 关键字

| 名称           | 描述                                                          |
| -------------- | ------------------------------------------------------------ |
| \_\_aicore\_\_ | 执行空间限定符。表示该函数只能在Ai Core上执行。                     |
| \_\_host\_aicore\_\_ | 执行空间限定符。表示该函数只能在Ai Core上执行。                     |
| \_\_gm\_\_     | 存储空间定义修饰符，表示被修饰的变量位于Global Memory地址空间。      |
| \_\_ubuf\_\_   | 存储空间定义修饰符，表示被修饰的变量位于Unified Buffer地址空间。     |
| \_\_cbuf\_\_   | 存储空间定义修饰符，表示被修饰的变量位于L1 Buffer地址空间。          |
| \_\_ca\_\_     | 存储空间定义修饰符，表示被修饰的变量位于L0A Buffer地址空间。         |
| \_\_cb\_\_     | 存储空间定义修饰符，表示被修饰的变量位于L0B Buffer地址空间。         |
| \_\_cc\_\_     | 存储空间定义修饰符，表示被修饰的变量位于L0C Buffer地址空间。         |
| \_\_biasbuf\_\_     | 存储空间定义修饰符，表示被修饰的变量位于BiasTable Buffer地址空间。 |
| \_\_fbuf\_\_   | 存储空间定义修饰符，表示被修饰的变量位于Fixpipe Buffer地址空间。     |

## 流水类型

TensorTile API 的底层操作会映射到昇腾处理器的不同流水线执行。关于流水类型的详细说明，请参考 [C API - 流水类型](../c_api/README.md#流水类型)。

> 可通过 C API 的 [同步控制](../c_api/c_api_list.md#同步控制) 类 API 控制同一流水线内的运行顺序和不同流水线间的执行顺序。

## 目录结构

``` text
include/tensor_api/
├── tensor.h                              # 总入口头文件
├── algorithm/                            # 高层算法接口（Copy、Mad）
├── atom/                                 # 原子操作接口（CopyAtom、MmadAtom）
├── arch/                                 # 架构 / trait 定义
├── tensor/                               # 张量类型接口（Layout、LocalTensor、Pointer）
└── utils/                                # 工具类型接口

impl/tensor_api/
├── algorithm/                            # 算法实现
├── atom/                                 # 原子操作实现
├── tensor/                               # 张量实现（Coord、Layout、MakeTensor）
├── utils/                                # 工具实现
└── arch/                                 # 架构底层实现
    ├── cube_compute/                     # 矩阵计算（MMAD 指令）
    ├── cube_datamove/                    # 矩阵搬运
    │   ├── data_copy/                    # DataCopy（GM→L1）
    │   ├── load_data/                    # LoadData（L1→L0）
    │   └── fixpipe/                      # FixPipe（L0C→OUT）
    ├── vector_compute/                   # 向量计算
    └── vector_datamove/                  # 向量搬运
```

<!-- ## 文档

- [接口列表](tensor_api_list.md)：介绍项目提供的api信息，方便快速查询。
- [算子开发指南]()：介绍基于tensor api的算子开发基本流程。 -->
