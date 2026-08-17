# Tensor API（试验特性）

Tensor API是一套面向Ascend C算子开发的C++接口。接口使用Tensor和Layout统一描述数据的存储位置、数据类型、Shape和Stride，并基于这些信息完成数据搬运和矩阵计算，减少开发者手工组织硬件指令参数和数据排布的工作量。

Tensor API当前属于试验特性，不保证后续版本兼容性。Tensor API只覆盖部分常用硬件能力，不替代C API。开发者可以在同一个核函数中组合使用Tensor API和C API，以构建完整的算子流程。

![Tensor API逻辑架构](../../figures/tensor_api_logical_architecture.png "Tensor API逻辑架构")

## 接口分层

| 层级 | 作用 | 主要内容 |
| :--- | :--- | :--- |
| 数据结构 | 描述数据及其排布。 | Tensor、Pointer、Layout、Shape、Stride和Coord。 |
| Arch | 标识具体计算或搬运通路，并定义Trait和Params。 | `copy_gm_to_l1`、`copy_l1_to_l0a`、`mmad_operation`等。 |
| Atom | 将Operation、Trait和可选Params组合为可调用对象。 | `copy_atom`、`mmad_atom`。 |
| Algorithm | 提供面向Tensor的统一调用入口。 | `copy`、`make_copy`、`mmad`和`make_mmad`。 |

使用Tensor API时，需要包含统一入口头文件：

```cpp
#include "tensor_api/tensor.h"

```

## 能力范围

- [Layout](layout/layout_structure.md)：Layout、Shape、Stride和Coord的构造、查询、索引、切片及数据排布类型。
- [Tensor](tensor/tensor_structure.md)：Tensor和Pointer的创建、访问与视图操作。
- [Algorithm](algorithm/algorithm.md)：Operation、Trait、Atom以及`copy`、`mmad`的组合方式。
- [矩阵计算](matrix_compute/matrix_compute.md)：矩阵数据搬入、矩阵乘加、矩阵结果搬出和相关关键特性。
- [数据搬运](data_move/data_move.md)：按Global Memory、Unified Buffer、L1 Buffer和L0 Buffer之间的数据通路组织搬运接口。
- [工具接口](utils/tool_functions.md)：编译期整数、维度保留标记和Layout Pattern查询接口。
- [废弃接口](deprecated_interface/deprecated_interface.md)：保留用于兼容的PascalCase成员函数及其snake_case替代接口。

## 与C API配合使用

Tensor API和C API的产品支持范围及参数约束相互独立，混合使用时需要同时满足两类接口的要求。C API操作Tensor API正在使用的同一存储区域时，开发者需要保证操作顺序、数据可见性和缓存一致性。

Tensor API暂未完整覆盖的能力及对应C API资料如下。

| 能力 | C API用途 | 接口示例 | 相关资料 |
| :--- | :--- | :--- | :--- |
| 缓存控制 | Data Cache或Instruction Cache预加载、刷新和失效。 | `asc_datacache_preload`、`asc_dcci`、`asc_dci` | [缓存控制](../c_api/cache_ctrl/cache_control.md) |
| 同步控制 | 流水线、核间及子核间同步。 | `asc_sync`、`asc_sync_pipe`、`asc_sync_notify`、`asc_sync_wait` | [同步控制](../c_api/sync/sync_control.md) |
| 系统变量 | 查询核索引、核数量、系统Cycle等运行信息。 | `asc_get_arch_ver`、`asc_get_core_id`、`asc_get_system_cycle` | [系统变量](../c_api/sys_var/system_variables.md) |
| 原子操作 | 随路原子操作和指定Global Memory地址的Scalar原子更新。 | `asc_set_atomic_add`、`asc_set_atomic_none`、`asc_atomic_add` | [随路原子操作](../c_api/simd_atomic/atomic_operations.md)、[Scalar计算](../c_api/scalar_compute/scalar_compute.md) |
| 初始化 | 恢复可能由前序算子遗留的全局控制状态。 | `asc_init` | [asc_init](../c_api/misc/asc_init.md) |

Tensor API内部可能调用C API实现具体硬件指令。开发者只需要包含`tensor_api/tensor.h`，不应直接包含`impl/tensor_api`下的内部头文件。只有显式调用额外C API时，才需要按对应C API资料包含相关头文件。

## 使用约束

- Tensor、Layout、Operation、Trait和Params必须属于同一条受支持的计算或搬运通路。
- Tensor API不会自动补充所有流水线同步。复用片上存储或组合多条流水线时，需要按硬件执行关系调用同步接口。
- 各接口支持的数据类型、数据排布、产品型号和对齐约束以对应接口资料为准。
- `asc::te::detail`和`impl/tensor_api`下的内容属于内部实现，不作为对外接口使用。
