# Tensor API

Tensor API是一套面向Ascend C算子开发的基础编程接口，使用Tensor和Layout统一描述数据的存储位置、数据类型、尺寸和排列规则，并基于这些信息完成数据搬运、矩阵计算和矢量计算，减少开发者手工组织硬件指令参数和数据排布的工作量。

Tensor API提供基于Tensor的数据搬运、矩阵计算和矢量计算能力。开发者可以在同一个核函数中组合使用Tensor API和C API，以构建完整的算子流程。

![Tensor API逻辑架构](../../figures/tensor_api_logical_architecture.png "Tensor API逻辑架构")

## 接口分层

| 层级 | 作用 | 主要内容 |
| :--- | :--- | :--- |
| 数据结构 | 描述数据的存储位置、数据类型、尺寸和排列规则。 | Tensor、Pointer、Layout、Shape、Stride和Coord。 |
| Arch | 提供数据搬运和矩阵计算的Operation、矢量计算接口，以及用于描述编译期特性和运行时参数的Trait和Params。 | Operation：`copy_gm_to_l1`、`mmad_operation`<br>Trait：`gm_to_l1_trait`<br>Params：`l0c_to_gm_params`<br>矢量计算：`select`、`fill`。 |
| Atom | 将Operation、Trait和可选Params组合为数据搬运或矩阵计算的原子对象。 | `copy_atom`、`mmad_atom`。 |
| Algorithm | 提供面向Tensor的数据搬运和矩阵计算的统一调用入口。 | `copy`、`make_copy`、`mmad`和`make_mmad`。 |

使用Tensor API时，需要包含统一入口头文件：

```cpp
#include "tensor_api/tensor.h"
```

## 能力范围

- [Layout](layout/layout_structure.md)：提供Layout、Shape、Stride和Coord的构造、数据排布描述及相关属性查询能力。
- [Tensor](tensor/tensor_structure.md)：提供Tensor和Pointer的创建、访问与切片能力。
- [Algorithm](algorithm/algorithm.md)：Operation、Trait、Atom以及`copy`、`mmad`的组合方式。
- [矩阵计算](matrix_compute/matrix_compute.md)：矩阵数据搬入、矩阵乘加、矩阵结果搬出和相关关键特性。
- [数据搬运](data_move/data_move.md)：支持多种数据搬运通路，包括Global Memory与Unified Buffer之间的双向搬运，Global Memory到L1 Buffer的搬运，Unified Buffer到L1 Buffer的搬运，L1 Buffer到Unified Buffer及各类L0 Buffer的搬运，L0C Buffer到Global Memory、Unified Buffer及L1 Buffer的搬运，以及Unified Buffer内部的数据搬运。
- [工具接口](utils/tool_functions.md)：提供编译期整数、维度保留标记和Layout Pattern查询接口，以及Layout、Tensor、Coord、Shape和量化参数判断、Trait类型提取和Pointer元素类型提取等工具。
- [废弃接口](deprecated_interface/deprecated_interface.md)：保留用于兼容的大驼峰风格接口。

## 与C API配合使用

Tensor API提供基于Tensor和Layout的数据组织、数据搬运、矩阵计算和矢量计算能力，适合构建算子的主要数据处理流程。缓存控制、同步控制、系统变量、原子操作和初始化等底层控制能力由C API提供。开发者可以在同一个核函数中组合使用Tensor API和C API，以构建完整的算子功能。

Tensor API和C API的产品支持范围及参数约束相互独立，混合使用时需要同时满足两类接口的要求。

Tensor API尚未完整覆盖的能力及对应C API资料如下。

| 能力 | C API用途 | 接口示例 | 相关资料 |
| :--- | :--- | :--- | :--- |
| 缓存控制 | Data Cache或Instruction Cache预加载、刷新和失效。 | `asc_datacache_preload`、`asc_dcci`、`asc_dci` | [缓存控制](../c_api/cache_ctrl/cache_control.md) |
| 同步控制 | 流水线同步和核间同步。 | `asc_sync`、`asc_sync_pipe`、`asc_sync_notify`、`asc_sync_wait` | [同步控制](../c_api/sync/sync_control.md) |
| 系统变量 | 查询核索引、核数量、系统Cycle等运行信息。 | `asc_get_arch_ver`、`asc_get_core_id`、`asc_get_system_cycle` | [系统变量](../c_api/sys_var/system_variables.md) |
| 原子操作 | 随路原子操作和指定Global Memory地址的Scalar原子更新。 | `asc_set_atomic_add`、`asc_set_atomic_none`、`asc_atomic_add` | [随路原子操作](../c_api/simd_atomic/atomic_operations.md)、[Scalar计算](../c_api/scalar_compute/scalar_compute.md) |
| 初始化 | 将全局控制状态恢复为初始值。 | `asc_init` | [asc_init](../c_api/misc/asc_init.md) |
