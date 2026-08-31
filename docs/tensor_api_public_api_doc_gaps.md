# Tensor API对外接口资料缺失项梳理

## 审计说明

- 代码基准：本地`master`分支提交`fd0ba0af6`中的`include/tensor_api`公共头文件。
- 资料范围：`docs/zh/api/SIMD-API/tensor_api`。
- 本文仅统计snake_case接口，不统计deprecated大驼峰兼容接口。
- SFINAE辅助模板参数以及仅用于内部类型约束的模板不作为资料补充项。

## 完全缺失的内容

### Vector侧Copy通路（不属于矩阵资料补充范围）

以下Vector侧数据搬运Operation、参数和Trait属于Vector数据搬运能力，不在`matrix_data_movement`目录中补充。

| 类别 | 暂未补充内容 |
| :--- | :--- |
| Global Memory到Unified Buffer | `copy_gm_to_ub`、`gm_to_ub_params`、`gm_to_ub_trait`、`gm_to_ub_trait_default` |
| Unified Buffer到Global Memory | `copy_ub_to_gm`、`ub_to_gm_trait`、`ub_to_gm_trait_default` |
| Unified Buffer到L1 Buffer | `copy_ub_to_l1`、`ub_to_l1_trait`、`ub_to_l1_trait_default` |
| Unified Buffer到Unified Buffer | `copy_ub_to_ub`、`ub_to_ub_trait`、`ub_to_ub_trait_default` |

对应公共头文件：

- `include/tensor_api/arch/vector/copy_op.h`
- `include/tensor_api/arch/vector/copy_trait.h`

### Cube侧Copy通路（已补充）

以下Cube侧数据搬运通路已补充对应接口文件。

| 通路 | 已补充内容 |
| :--- | :--- |
| L1 Buffer到Unified Buffer | `copy_l1_to_ub`、`l1_to_ub_trait`、`l1_to_ub_trait_default` |
| L0C Buffer到L1 Buffer | `copy_l0c_to_l1`、`l0c_to_l1_params`、`l0c_to_l1_trait`、`l0c_to_l1_trait_default` |

对应公共头文件：

- `include/tensor_api/arch/cube/copy_op.h`
- `include/tensor_api/arch/cube/copy_trait.h`

### local_tensor寄存器搬运能力

`local_tensor`结构体和构造函数已有资料，但以下成员函数及其配套枚举尚未说明。

| 类别 | 缺失内容 |
| :--- | :--- |
| 寄存器搬入 | `load`、双目标`load`、`load_broadcast` |
| 寄存器搬出 | 单源`store`、双源`store` |
| 搬入模式 | `load_sideband_mode`及全部枚举值 |
| 搬出模式 | `store_sideband_mode`及全部枚举值 |
| 广播模式 | `broadcast_mode`及全部枚举值 |

对应公共头文件：`include/tensor_api/tensor/local_tensor.h`。

### 实验性Vector接口

公共入口头文件已经包含以下实验接口，但当前Tensor API概述仍将实验目录标记为“暂时为空”。

| 类别 | 缺失内容 |
| :--- | :--- |
| 寄存器Tensor | `reg_tensor`、`reg_tensor<bool>`、`reg_pair` |
| 寄存器Tensor成员 | `reg_tensor::operator=`、`reg_tensor::with_mask` |
| 数据重排 | `interleave`、`deinterleave` |
| Mask构造 | `make_mask`、`all_mask`、`none_mask`、`update_mask` |
| Mask模式 | `mask_pattern`及全部枚举值 |

对应公共头文件：

- `include/tensor_api/arch/vector/experimental/reg_tensor.h`
- `include/tensor_api/arch/vector/experimental/data_reorder.h`
- `include/tensor_api/arch/vector/experimental/MaskReg_compute.h`

### 基础工具接口和类型（已补充）

| 缺失内容 | 当前资料情况 |
| :--- | :--- |
| `crd2idx` | 已补充Tensor API接口自身的函数原型、参数和返回值说明。 |
| 公共头中定义的编译期整型别名 | 已补充完整定义和可用值列表。 |
| `keep_mark` | 已补充结构体说明。 |
| 常量`_` | 已补充统一类型和用途说明。 |
| `location::invalid`、`location::ssbuf` | `make_mem_ptr`资料的location列表中未体现，需要确认是否作为用户接口开放。 |

对应公共头文件：

- `include/tensor_api/tensor/layout_interface.h`
- `include/tensor_api/utils/utils.h`

## 已有使用说明但定义不完整

### Copy Operation、Atom和Trait（已补充）

- 已统一说明Copy Operation、`copy_traits`、`copy_atom`和Trait之间的关系及推荐调用流程。
- 已说明`copy_traits`和`copy_atom`的公开定义及成员。
- 已在中央表格列出所有Copy Operation及其对应的Trait、Default Trait、Params和通路资料链接。
- 已在各通路资料中列出具体Operation、Trait和Default Trait定义。
- Operation原型仅展示public静态`copy`成员，不展示private实现。

对应公共头文件：`include/tensor_api/atom/cube/copy_atom.h`。

### Mmad Atom和Trait（已补充统一说明）

- 已统一说明`mmad_operation`、`mmad_atom`和`mmad_trait_default`之间的关系及推荐调用流程。
- 已说明`mmad_atom::call`和`mmad_atom::with`公共成员。
- `mmad_operation`原型仅展示public静态`mmad`成员，不展示private实现。
- `mmad_traits`和`mmad_unpack`作为Atom内部展开机制，暂不写入用户资料。

对应公共头文件：

- `include/tensor_api/atom/cube/mmad_atom.h`
- `include/tensor_api/arch/cube/mmad_op.h`
- `include/tensor_api/arch/cube/mmad_trait.h`

## 已基本覆盖的内容

以下内容在现有资料中已经基本覆盖，不作为主要补充项：

- `layout`及其snake_case成员函数。
- `base_tensor`、`global_tensor`及其snake_case成员函数。
- `shape`、`stride`、`coord`。
- 当前所有Layout Pattern。
- `layout_trait`、`layout_trait_default`和`frame_layout_format`。
- `make_mem_ptr`的三种重载。
- `mmad`、`make_mmad`、`mmad_params`和`mmad_trait`。
- `mmad_type`、`unit_flag_mode`、`round_mode`和`dual_dst_mode`。
- `img2col_params`。
- 现有Cube搬入、搬出资料中的主要`copy`和`make_copy`原型。

## Tensor API资料引用的基础API文档

当前`docs/zh/api/SIMD-API/tensor_api`共直接引用8份基础API文档。引用内容主要用于补充通用数据类型、矩阵计算基础概念和Buffer内存结构，避免在Tensor API资料中重复维护相同的硬件基础知识。

### 通用数据结构

| 基础API文档 | Tensor API引用位置 | 用途 |
| :--- | :--- | :--- |
| [内置数据类型](zh/api/SIMD-API/basic_api/data_structures/builtin_data_types.md) | `basic_data_structure/basic_data_structure.md` | 说明Ascend C支持的数据类型及其基本含义。 |  后续添加通用文档 831
| [精度转换](zh/api/SIMD-API/basic_api/data_structures/precision_conversion.md) | `basic_data_structure/basic_data_structure.md` | 说明不同数据类型间的精度转换规则。 | 复用CAPI已有资料

### 矩阵计算基础概念

| 基础API文档 | Tensor API引用位置 | 用途 |
| :--- | :--- | :--- |
| [矩阵计算单元](zh/api/SIMD-API/basic_api/cube_compute_ISASI/overview/cube_compute_unit.md) | `matrix_computation/overview/overview.md` | 补充矩阵计算单元的硬件组成和基础能力。 | 使用最外层
| [背景与核心概念](zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_fractal_intro/background_and_core_concepts.md) | `matrix_computation/cube_compute_fractal_intro/cube_compute_fractal_intro.md` | 介绍矩阵分形的背景和核心概念。 | 待确认CAPI是否补充
| [关键分形格式详解](zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_fractal_intro/key_fractal_format_details.md) | `matrix_computation/cube_compute_fractal_intro/cube_compute_fractal_intro.md` | 介绍矩阵计算使用的关键分形格式。 |    待确认CAPI是否补充
| [辅助矩阵分形格式详解](zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_fractal_intro/aux_cube_fractal_format_details.md) | `matrix_computation/cube_compute_fractal_intro/cube_compute_fractal_intro.md` | 介绍矩阵计算使用的辅助矩阵分形格式。 |   待确认CAPI是否补充

### Buffer内存结构

| 基础API文档 | Tensor API引用位置 | 用途 |
| :--- | :--- | :--- |
| [L1 Buffer、L0A Buffer/L0B Buffer内存结构介绍](zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/L1_L0A_B_memory_structure_intro.md) | `matrix_data_movement/cube_compute_load/cube_compute_load.md` | 说明矩阵搬入涉及的L1 Buffer、L0A Buffer和L0B Buffer内存结构。 |   待确认CAPI是否补充
| [L0C Buffer内存结构介绍](zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/L0C_memory_structure_intro.md) | `matrix_data_movement/cube_compute_store/cube_compute_store.md` | 说明矩阵搬出涉及的L0C Buffer内存结构。 |  待确认CAPI是否补充

上述8份文档均为通用概念或硬件结构资料，可以继续作为Tensor API的公共背景资料引用。Tensor API资料不再直接引用基础API接口资料。

## 建议处理顺序

1. 已补充L1 Buffer到Unified Buffer、L0C Buffer到L1 Buffer两条Cube通路；Vector侧Copy通路不属于矩阵资料补充范围，`local_tensor`寄存器搬运接口仍待补充。
2. 在实验目录中补充`reg_tensor`、Mask和数据重排接口，并更新Tensor API概述中的实验能力说明。
3. 已补充`crd2idx`、编译期整型别名、`keep_mark`和常量`_`等基础工具资料。
4. 已统一说明operation、Atom和default Trait的使用方式，其中operation仅展示public内容。
5. 确认`location::invalid`和`location::ssbuf`是否属于正式用户接口；若仅供内部使用，建议从公共接口契约中排除，而不是补充为用户接口资料。`mmad_unpack`按内部展开机制处理，不写入用户资料。
