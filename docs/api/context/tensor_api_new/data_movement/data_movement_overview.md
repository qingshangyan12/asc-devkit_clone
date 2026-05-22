# 数据搬运导览

Tensor API数据搬运资料按矩阵计算流程分为搬入和搬出两部分。矩阵计算前，通常需要将输入矩阵从Global Memory搬入L1 Buffer、L0A Buffer、L0B Buffer等存储层级；矩阵计算后，需要将L0C Buffer中的计算结果搬出到Global Memory或Unified Buffer。

## 矩阵计算的搬入

矩阵计算的搬入用于准备`Mmad`的输入数据，主要覆盖Global Memory到L1 Buffer、L1 Buffer到L0A Buffer、L1 Buffer到L0B Buffer、L1 Buffer到L0ScaleA Buffer/L0ScaleB Buffer，以及BiasTable Buffer、Fixpipe Buffer、Unified Buffer等辅助搬运场景。

- [矩阵计算的搬入总体说明](matrix_data_in_movement_overview.md)
- [Global Memory到L1 Buffer数据搬运](gm_to_l1_data_movement.md)
- [L1 Buffer到L0A Buffer数据搬运](l1_to_l0a_data_movement.md)
- [L1 Buffer到L0B Buffer数据搬运](l1_to_l0b_data_movement.md)
- [L1 Buffer到L0ScaleA Buffer数据搬运](l1_to_l0scalea_data_movement.md)
- [L1 Buffer到L0ScaleB Buffer数据搬运](l1_to_l0scaleb_data_movement.md)
- [L1 Buffer到BiasTable Buffer数据搬运](l1_to_bias_table_buffer_data_movement.md)
- [L1 Buffer到Fixpipe Buffer数据搬运](l1_to_fixpipe_buffer_data_movement.md)
- [L1 Buffer到Unified Buffer数据搬运](l1_to_ub_data_movement.md)
- [Global Memory到Unified Buffer数据搬运](gm_to_ub_data_movement.md)
- [Unified Buffer到Global Memory数据搬运](ub_to_gm_data_movement.md)
- [Unified Buffer到L1 Buffer数据搬运](ub_to_l1_data_movement.md)

## 矩阵计算的搬出

矩阵计算的搬出是AscendC编程框架中用于数据搬运的核心接口系列，主要实现L0C Buffer与Global Memory、L1 Buffer、Unified Buffer之间的数据高效传输。该接口系列通常在`Mmad`接口后配合使用，用于处理写入L0C Buffer的矩阵计算结果；针对Ascend 950PR/Ascend 950DT，还支持L0C Buffer与Unified Buffer之间的数据传输。

矩阵计算的搬出接口支持多种随路搬运场景，包括随路量化、随路Relu、随路格式转换（NZ2ND/NZ2DN/NZ2NZ）、随路通道拆分（Channel Split）、随路通道合并（Channel Merge）和Unified Buffer双目标模式。开发者可通过参数结构体配置搬运路径、起始位置、搬运长度和随路能力，以利用硬件搬运能力提升算子整体执行性能。

- [矩阵计算的搬出总体说明](matrix_data_out_movement_overview.md)
- [L0C Buffer到Global Memory数据搬运](l0c_to_gm_data_movement.md)
- [L0C Buffer到Unified Buffer数据搬运](l0c_to_ub_data_movement.md)
- [随路量化](quant_pre.md)
- [随路Relu](relu_pre.md)
- [F32 Channel Split](f32_channel_split.md)
- [Int8/Int4 Channel Merge](int8_int4_channel_merge.md)
- [L0C Buffer到Unified Buffer双目标模式](l0c_to_ub_dual_dst.md)
