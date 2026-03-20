# Tensor API

Tensor API 提供一套基于 Ascend C 的 C++ 模板抽象库，专为定义和操作层次化多维的数据布局而设计。用户可以专注于算法的逻辑表达而无需关心底层的内存布局问题，适用于昇腾 AI 处理器上的高性能计算场景，提供与业界一致的开发体验。有关Tensor API编程接口的支持范围和约束，请参考XXX。本项目支持的AI处理器包括：Ascend 910C、Ascend 910B。

## 需要包含的头文件

> [!NOTE]说明 
> 包含 tensor_api_impl.h 文件来调用 Tensor API 相应接口。如无特殊说明，包含该头文件即可满足接口调用需求。
> 若 API 文档中有特殊说明，则应遵循 API 的具体说明。

```
#include "tensor_api/tensor_api_impl.h"
```

## 核心概念

### 数据结构

#### Layout

Layout 用于描述张量的内存排布格式。由以下两部分组成：

- **Shape**: 张量形状，描述各维度大小
- **Stride**: 内存步幅，描述相邻元素在内存中的间隔

Layout 支持层次化表述法以处理复杂的内存排布情况。详细文档请参考  [Layout和层次化表述法](struct/defenition/Layout和层次化表述法.md)。


#### Tensor

Tensor 结合了数据指针（Engine）和 Layout，提供类型安全的张量访问接口。

### Atom 抽象

Atom 是对底层硬件操作的抽象封装。每个 Atom 由 **Operation**（操作类型）和 **Traits**（特征参数）组成，提供统一的可调用接口。

**设计理念**:
- **Operation**: 定义操作的类型，如 `CopyGM2L1`、`CopyL12L0`、`MmadOperation`
- **Traits**: 定义操作的特征参数，如数据类型、布局格式、对齐要求等
- **Atom**: 封装 Operation 和 Traits，提供统一的调用接口

**Atom 类型**:

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
| `CopyL12L0B` | L1 → L0B |  L1 到矩阵单元 A |
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
include/experimental/tensor_api/   
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

impl/experimental/tensor_api/
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
