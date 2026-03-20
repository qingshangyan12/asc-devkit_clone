# Tensor API

## 概述

TensorTile API 是一个基于 Ascend C 的 C++ 模板库，为昇腾 AI 处理器提供类型安全、可组合的张量编程抽象。该库通过 Layout 描述张量的内存排布格式，结合 Tensor 提供统一的数据访问接口，利用 Atom 抽象封装底层硬件操作，使开发者能够在高层以声明式的方式表达计算逻辑。

TensorTile API 的核心价值在于将复杂的硬件细节（分形内存布局、多级存储层次、指令参数配置）封装在底层，同时通过模板参数暴露足够的控制点，让高级用户能够对硬件行为进行精细调优。这种设计使得同一套代码可以在不同昇腾处理器架构上运行，同时保证性能。

该库特别适用于矩阵乘法及量化计算、以及需要充分利用 Cube 计算单元的自定义算子开发场景。通过 Traits 机制，用户可以灵活配置矩阵计算的分块大小、数据类型等参数，实现性能与精度的平衡。

### 设计目标

- **可组合性**: Layout、Tensor、Atom 等组件可灵活组合，适应不同计算场景
- **高性能**: 灵活配置参数，
- **可移植性**: 统一的 API 接口，底层适配不同 NPU 架构

### 编程模型

TensorTile API 采用分层抽象的编程模型，从底层的硬件指令到高层的算法封装，每一层都提供清晰的抽象边界：

**Layer 1: Tensor Layer - 数据抽象层**

数据抽象层负责描述张量的内存排布和访问方式，是整个编程模型的基础。

- **Shape**: 描述张量各维度的大小，支持静态（编译期）和动态（运行时）两种形式
- **Stride**: 描述相邻元素在内存中的地址间隔，决定数据的访问顺序
- **Layout**: 描述张量在内存中的排布格式，由 Shape（形状）和 Stride（步幅）组成。支持层次化表述法，可表达复杂的分形内存布局
- **Coord**: 多维坐标，用于定位张量中的特定元素
- **Tile**: 分块描述，用于将大张量切分为小块进行分块计算
- **Tensor**: 结合数据指针（Engine）和 Layout，提供类型安全的张量访问接口

**Layer 2: Arch Layer - 硬件抽象层**

硬件抽象层直接映射到昇腾处理器的硬件指令，针对不同架构提供特化实现。

- **Mmad**: 矩阵乘累加指令，利用 Cube 计算单元执行高性能矩阵运算
- **LoadData**: 数据加载指令，将数据从 L1 缓存搬运到 L0 操作数寄存器
- **Fixpipe**: 后处理指令，对 L0C 累加器结果进行格式转换、量化、ReLU 等操作后搬出
- **DataCopy**: 通用数据搬运指令，支持不同存储层级之间的数据传输

**Layer 3: Atom Layer - 原子操作层**

原子操作层将硬件指令封装为可复用的计算单元，分离操作语义和配置参数。

- **CopyAtom**: 数据搬运原子操作，封装 LoadData、Fixpipe 等搬运指令
- **MmadAtom**: 矩阵计算原子操作，封装 Mmad 指令
- **Traits**: 配置结构体，定义原子操作的具体行为参数

**Layer 4: Algorithm Layer - 算法封装层**

算法封装层基于 Atom 组合实现常用计算模式，提供开箱即用的高层 API。

- **Copy**: 通用数据搬运算法
- **Mad**: 矩阵乘加算法
- **Gemm**: 完整的矩阵乘法算法（规划中）

## 需要包含的头文件

> [!NOTE]说明 
> 包含 tensor.h 文件来调用 Tensor API 相应接口。如无特殊说明，包含该头文件即可满足接口调用需求。
> 若 API 文档中有特殊说明，则应遵循 API 的具体说明。

```
#include "tensor_api/tensor.h"
```

## 核心概念

### 数据结构

#### Layout

Layout 用于描述张量的内存排布格式。由以下两部分组成：

- **Shape**: 张量形状，描述各维度大小
- **Stride**: 内存步幅，描述相邻元素在内存中的间隔

Layout 支持层次化表述法以处理复杂的内存排布情况。详细文档请参考  [Layout和层次化表述法](struct/defenition/Layout和层次化表述法.md)。


##### 创建函数

| API | 说明 |
|-----|------|
| `MakeShape` | 构造 Shape |
| `MakeStride` | 构造 Stride |
| `MakeLayout` | 构造 Layout |
| `MakeCoord` | 构造 Coord |
| `MakeTile` | 构造 Tile |

##### 预定义 Layout 格式

| 格式 | 说明 | 构造函数 |
|------|------|----------|
| NZ | 内层 Z 字形，外层 N 字形分形 | `MakeNzLayout` |
| ZN | 内层 N 字形，外层 Z 字形分形 | `MakeZnLayout` |
| ND | 行优先格式 | `MakeNDLayout` |
| DN | 列优先格式 | `MakeDNLayout` |
| ZZ | 双 Z 字形分形 | `MakeZzLayout` |
| NN | 双 N 字形分形 | `MakeNnLayout` |
| L0C | L0C 输出格式 | `MakeL0CLayout` |

##### Layout 操作

| API | 说明 |
|-----|------|
| `GetShape` | 获取 Shape |
| `GetStride` | 获取 Stride |
| `Size` | 返回 Shape 总大小 |
| `Rank` | 返回 Layout 秩 |
| `Capacity` | 返回张量容量 |
| `Coshape` | 返回实际 Shape 空间 |
| `Cosize` | 返回实际占用内存 |
| `Get` | 获取子 Layout |
| `Select` | 选择特定维度子 Layout |
| `Crd2Idx` | 坐标转索引 |

#### Tensor

Tensor 是 TensorTile API 的核心数据容器，结合数据引擎（Engine）和内存排布（Layout），提供类型安全的张量访问接口。Tensor 将数据的物理存储位置与其逻辑排布解耦，用户可以通过统一的接口访问数据，无需关心底层的复杂内存布局。

##### 核心类型

```
LocalTensor<TensorAttribute<Engine, Layout>>
    │              │              │
    │              │              └── 描述内存排布格式
    │              └── 数据引擎，封装存储位置和访问方式
    └── 张量属性，组合 Engine 和 Layout
```

**Iterator 类型**:
- `GMmemPtr<T>`: Global Memory 指针
- `UBmemPtr<T>`: Unified Buffer 指针
- `L1memPtr<T>`: L1 Buffer 指针
- `L0AmemPtr<T>`: L0A Buffer 指针
- `L0BmemPtr<T>`: L0B Buffer 指针
- `L0CmemPtr<T>`: L0C Buffer 指针
- `BiasmemPtr<T>`: Bias Table Buffer 指针
- `FixbufmemPtr<T>`: Fixpipe Buffer 指针

#### 属性

| 方法 | 返回类型 | 说明 |
|------|----------|------|
| `Tensor()` | `LocalTensor&` | 返回自身引用 |
| `Engine()` | `Engine&` | 返回数据引擎 |
| `Layout()` | `Layout&` | 返回内存排布 |
| `Data()` | `Iterator` | 返回数据指针（Engine().Begin()） |
| `Shape()` | `Shape` | 返回张量形状 |
| `Stride()` | `Stride` | 返回张量步幅 |
| `Size()` | `int` | 返回张量总元素数 |
| `Capacity()` | `int` | 返回张量容量 |

#### 元素访问

| 操作符 | 说明 |
|--------|------|
| `operator[](coord)` | 通过坐标访问元素，返回元素引用 |
| `operator()(coord)` | 通过坐标创建子 Tensor 视图 |
| `operator()(c0, c1, cs...)` | 多参数坐标访问，等价于 `operator()(MakeCoord(c0,c1,cs...))` |
| `operator()(coord, info)` | 带 TileShape 或 Tensor 信息创建子 Tensor |

#### 操作

| 方法 | 说明 |
|------|------|
| `Compose(layouts...)` | 组合多个 Layout，返回新的 Tensor 视图 |
| `Tile(layouts...)` | 通过 Tile 切分，返回子 Tensor 视图 |
### Atom 抽象

Atom 是对底层硬件操作的抽象封装。每个 Atom 由 **Operation**（操作类型）和 **Traits**（特征参数）组成，提供统一的可调用接口。

#### 结构说明

- **Operation**: 定义操作的类型，如 `CopyGM2L1`、`CopyL12L0`、`MmadOperation`
- **Traits**: 定义操作的特征参数，包括数据类型、布局格式、对齐要求等，如 `MmadTrait`、`LoadDataTrait`、`DataCopyTrait`
- **Atom**: 封装 Operation 和 Traits，提供统一的调用接口

#### 类型

| Atom | 说明 | 对应操作 |
|------|------|----------|
| `CopyAtom` | 数据搬运原子操作 | `CopyGM2L1`, `CopyL12L0A`, `CopyL12L0B`, `CopyL0C2Out` |
| `MmadAtom` | 矩阵乘累加原子操作 | `MmadOperation` |

## 接口说明

### 数据搬运接口

数据搬运类 API，支持不同存储层级之间的数据传输。


| API | 路径 | 说明 |
|-----|------|------|
| `Copy` | 通用 | 通用搬运接口 |
| `CopyGM2L1` | GM → L1 | 全局内存到 L1 |
| `CopyL12L0A` | L1 → L0A | L1 到矩阵单元 A |
| `CopyL12L0B` | L1 → L0B |  L1 到矩阵单元 B |
| `CopyL12BT`  | L1 → BT |  L1 到Bias Table Buffer|
| `CopyL12BT`  | L1 → FB| L1 到Fixpipe Buffer |
| `CopyL0C2GM` | L0C → GM | 输出到全局内存 |
| `CopyL0C2UB` | L0C → UB | 输出到 Unified Buffer |
| `LoadData` | L1 → L0 | L1 加载到 L0 |
| `FixPipe` | L0C → Out | 矩阵结果搬出 |


### 矩阵计算接口

| API | 说明 |
|-----|------|
| `Mad` | 矩阵乘加的高层封装 |


## 关键字

| 名称           | 描述                                                          |
| -------------- | ------------------------------------------------------------ |
| \_\_aicore\_\_ | 执行空间限定符。表示该函数只能在Ai Core上执行。                     |
| \_\_gm\_\_     | 存储空间定义修饰符，表示被修饰的变量位于Global Memory地址空间。      |
| \_\_ubuf\_\_   | 存储空间定义修饰符，表示被修饰的变量位于Unified Buffer地址空间。     |
| \_\_cbuf\_\_   | 存储空间定义修饰符，表示被修饰的变量位于L1 Buffer地址空间。          |
| \_\_ca\_\_     | 存储空间定义修饰符，表示被修饰的变量位于L0A Buffer地址空间。         |
| \_\_cb\_\_     | 存储空间定义修饰符，表示被修饰的变量位于L0B Buffer地址空间。         |
| \_\_cc\_\_     | 存储空间定义修饰符，表示被修饰的变量位于L0C Buffer地址空间。         |
| \_\_biasbuf\_\_     | 存储空间定义修饰符，表示被修饰的变量位于Bias Table Buffer地址空间。 |
| \_\_fbuf\_\_   | 存储空间定义修饰符，表示被修饰的变量位于Fixpipe Buffer地址空间。     |


## 流水类型

NPU内部有不同的计算单元，在计算时往往需要把计算数据搬运到计算单元上。不同计算单元上的计算过程、数据搬运过程可划分为不同的流水线。在AI Core上执行的操作会被分配到不同的流水线(PIPE)上执行，包括以下几种：
- PIPE_S：标量流水线，负责指令分发和标量计算。
- PIPE_V：矢量计算流水线。
- PIPE_M：矩阵计算流水线。
- PIPE_MTE1：搬运操作。包括从L1 Buffer到L0A Buffer或L0B Buffer，从L1 Buffer到UB的搬运操作和L0A Buffer或L0B Buffer的初始化操作。
- PIPE_MTE2：搬运操作。包括从GM到L1 Buffer、从GM到L0A Buffer或L0B Buffer、从GM到UB的搬运操作和L1 Buffer的初始化操作。
- PIPE_MTE3：搬运操作。包括从UB到GM，从UB到L1 Buffer的操作。
- PIPE_FIX：Fixpipe流水线。
- PIPE_ALL：所有流水线。

## 目录结构

```
include/tensor_api/   
├── tensor.h                       # 总入口头文件
├── tensor/
│   ├── layout.h                   # Layout API 声明
│   ├── layout_struct.h            # Layout 结构体
│   ├── local_tensor.h             # Tensor API 声明
│   └── pointer.h                  # Pointer API 声明
├── atom/
│   └── atom.h                     # Atom 声明 (CopyAtom, MmadAtom)
├── arch/
│   ├── arch.h                     # Arch 总入口
│   ├── trait_struct.h             # Traits 结构体定义
│   └── cube_datamove/
│       ├── load_data.h            # LoadData API 声明
│       └── fixpipe.h              # Fixpipe API 声明
├── algorithm/
│   ├── algorithm.h                # Algorithm 总入口
│   ├── copy.h                     # Copy 算法
│   ├── mad.h                      # Mad 算法
│   └── gemm.h                     # GEMM 算法
└── utils/
    └── utils.h                    # 工具函数

impl/tensor_api/
│
├── tensor/                       # 数据结构定义
│   ├── layout_definition.h       # Layout 核心定义
│   ├── layout_method.h           # Layout 构造函数
│   ├── layout_impl.h             # Layout 实现
│   ├── layout_struct.h           # Layout 格式结构体
│   ├── layout_fractal.h          # 分形 Layout 构造
│   ├── layout_static_fractal.h   # 静态分形 Layout
│   ├── layout_dispatch.h         # Layout 运行时分发
│   ├── layout_size.h             # Layout 大小计算
│   ├── local_tensor_impl.h       # Tensor 实现
│   ├── engine_impl.h             # Engine 实现
│   ├── pointer_impl.h            # 指针构造函数
│   ├── pointer_mem_impl.h        # 内存指针实现
│   ├── pointer_adaptor_impl.h    # 指针适配器
│   ├── tuple_impl.h              # Tuple 工具
│   ├── tuple_fractal.h           # 分形 Tuple
│   └── coord_index.h             # 坐标索引转换
│
├── atom/                         # 原子操作抽象
│   ├── copy_atom_impl.h          # CopyAtom 定义
│   ├── copy_traits_impl.h        # Copy Traits 定义
│   ├── mad_atom_impl.h           # MmadAtom 定义
│   ├── mad_traits_impl.h         # Mad Traits 定义
│   ├── cube_compute/             # 矩阵计算原子操作
│   ├── cube_datamove/            # 矩阵搬运原子操作
│   ├── vector_compute/           # 向量计算原子操作
│   └── vector_datamove/          # 向量搬运原子操作
│
├── arch/                         # 架构相关实现
│   ├── cube_compute/             # 矩阵计算底层
│   │   └── mmad/                 # MMAD 指令实现
│   ├── cube_datamove/            # 矩阵搬运底层
│   │   ├── data_copy/            # DataCopy 指令
│   │   ├── load_data/            # LoadData 指令
│   │   │   ├── npu_arch_2201/    # 2201 架构
│   │   │   └── npu_arch_3510/    # 3510 架构
│   │   └── fixpipe/              # FixPipe 指令
│   │       ├── npu_arch_2201/
│   │       └── npu_arch_3510/
│   ├── vector_compute/           # 向量计算底层
│   ├── vector_datamove/          # 向量搬运底层
│   └── utils/                    # 架构工具
│       ├── check_format.h
│       ├── is_format.h
│       └── arch_utils.h
│
├── algorithm/                    # 高层算法封装
│   ├── copy_impl.h               # Copy 算法
│   ├── mad_impl.h                # Mad 算法
│   └── gemm_impl.h               # GEMM 算法
│
└── utils/                        # 通用工具
    ├── utils_impl.h              # 工具入口
    ├── constant_impl.h           # 常量
    ├── macro_impl.h              # 宏定义
    └── extra_impl.h              # 扩展工具
```

## API列表

| 目录                               |
| ---------------------------------- |
| [数据结构](tensor_api_list.md#数据结构) |
| [数据接口](tensor_api_list.md#数据接口) |
| [数据搬运](tensor_api_list.md#数据搬运) |
| [矩阵计算](tensor_api_list.md#矩阵计算) |
