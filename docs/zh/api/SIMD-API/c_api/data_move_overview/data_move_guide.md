# 数据搬运导览

本文面向Ascend 950PR/Ascend 950DT（NPU架构为DAV_3510），介绍如何根据源、目的存储单元和数据排布选择C API数据搬运接口。使用C API时，开发者需要显式管理地址、搬运参数和流水同步；除接口文档另有说明外，可统一包含头文件`c_api/asc_simd.h`。

C API与基础API最终使用相同的NPU搬运通路。基础API以Tensor和参数结构体提供封装，C API则以地址、字节数、DataBlock和硬件配置参数直接表达搬运，更适合需要精确控制硬件行为的场景。

## 与基础API抽象的对应关系

| 基础API抽象 | DAV_3510 C API中的对应入口 |
| --- | --- |
| `DataCopy`、`DataCopyPad` | 按方向选择`asc_copy_gm2ub`、`asc_copy_ub2gm`、`asc_copy_gm2l1`及其`*_align`等接口。 |
| NDDMA `DataCopy` | 使用`asc_ndim_copy_gm2ub`及配套配置接口。 |
| `LoadData` | 使用`asc_copy_gm2l1`、`asc_copy_l12l0a`或`asc_copy_l12l0b`系列接口。 |
| L0C搬出和`Fixpipe`能力 | 使用`asc_copy_l0c2gm`、`asc_copy_l0c2l1`或`asc_copy_l0c2ub`，并按需配置随路转换。 |
| Reg数据加载、存储 | 使用`asc_load*`和`asc_store*`接口，在Vector Function内显式操作寄存器。 |

以上为能力语义映射，并非函数原型的一一对应关系。基础API导览中面向其他产品的通路不一定适用于DAV_3510，应以本文的数据通路和C API接口产品支持情况为准。

## 阅读路线

1. 阅读[数据搬运概览](overview/overview.md)，了解C API分类及PIPE_MTE1、PIPE_MTE2、PIPE_MTE3、PIPE_FIX和PIPE_V的分工。
2. 阅读[数据搬运概念](overview/data_move_concept.md)，根据连续、高维切分、非对齐、分形转换或NDDMA场景选择接口形态。
3. 阅读[数据通路](overview/data_path.md)，确认DAV_3510支持的源、目的存储单元组合。
4. 编码前检查[总体约束说明](overview/overall_constraints.md)，并以具体接口文档中的参数单位、对齐和取值范围为准。

## 按数据通路选择

| 数据通路 | 典型用途 | 详细说明 |
| --- | --- | --- |
| GM与L1/L0 | 为矩阵计算准备输入，或将L0C结果写回GM | [GM与L1/L0数据搬运](gm_l1_or_l0_data_move.md) |
| GM与UB | 为矢量计算搬入输入、搬出结果 | [GM与UB数据搬运](gm_ub_data_move.md) |
| L1与L0 | 将矩阵数据装载到L0A/L0B，或将L0C结果搬到L1 | [L1与L0数据搬运](l1_l0_data_move.md) |
| L1/L0C与UB | 在Cube侧和Vector侧片上存储间交换数据 | [L1/L0C与UB数据搬运](l1_or_l0c_ub_data_move.md) |
| L1到BiasTable | 为Mmad准备bias | [L1到BiasTable数据搬运](l1_bias_table_data_move.md) |
| L1到Fixpipe Buffer | 装载随路量化或ReLU参数 | [L1到Fixpipe Buffer数据搬运](l1_fixpipe_data_move.md) |
| UB与UB | UB中连续或高维切分复制 | [UB与UB数据搬运](ub_ub_data_move.md) |
| Reg与UB | Vector Function加载和存储寄存器数据 | [Reg与UB数据搬运](reg_ub_data_move.md) |

> [!NOTE]说明
> 建议在Kernel入口首先调用[asc_init](../utils/sys_init/asc_init.md)，再进行数据搬运和计算。不同流水之间存在数据依赖时，需要使用C API同步接口显式建立执行顺序。
