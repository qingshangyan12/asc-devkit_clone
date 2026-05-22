# 数据搬运导览

Tensor API 数据搬运资料按矩阵计算流程分为搬入和搬出两部分。矩阵计算前，通常需要将输入矩阵从 GM 搬入 L1、L0A、L0B 等存储层级；矩阵计算后，需要将 L0C 中的计算结果搬出到 GM 或 UB。

## 矩阵计算的搬入

矩阵计算的搬入用于准备 `Mmad` 的输入数据，主要覆盖 GM 到 L1、L1 到 L0A、L1 到 L0B、L1 到 L0ScaleA/L0ScaleB，以及 BiasTable Buffer、Fixpipe Buffer、UB 等辅助搬运场景。

- [矩阵计算的搬入总体说明](matrix_data_in_movement_overview.md)
- [GM到L1数据搬运](gm_to_l1_data_movement.md)
- [L1到L0A数据搬运](l1_to_l0a_data_movement.md)
- [L1到L0B数据搬运](l1_to_l0b_data_movement.md)
- [L1到L0ScaleA数据搬运](l1_to_l0scalea_data_movement.md)
- [L1到L0ScaleB数据搬运](l1_to_l0scaleb_data_movement.md)
- [L1到BiasTable Buffer数据搬运](l1_to_bias_table_buffer_data_movement.md)
- [L1到Fixpipe Buffer数据搬运](l1_to_fixpipe_buffer_data_movement.md)
- [L1到UB数据搬运](l1_to_ub_data_movement.md)
- [GM到UB数据搬运](gm_to_ub_data_movement.md)
- [UB到GM数据搬运](ub_to_gm_data_movement.md)
- [UB到L1数据搬运](ub_to_l1_data_movement.md)

## 矩阵计算的搬出

矩阵计算的搬出是 AscendC 编程框架中用于数据搬运的核心接口系列，主要实现 L0C 与 Global Memory、L1、UB 之间的数据高效传输。该接口系列通常在 `Mmad` 接口后配合使用，用于处理写入 L0C 的矩阵计算结果；针对 Ascend 950PR/Ascend 950DT，还支持 L0C 与 UB 之间的数据传输。

矩阵计算的搬出接口支持多种随路搬运场景，包括随路量化、随路 Relu、随路格式转换（NZ2ND/NZ2DN/NZ2NZ）、随路通道拆分（Channel Split）、随路通道合并（Channel Merge）和 UB 双目标模式。开发者可通过参数结构体配置搬运路径、起始位置、搬运长度和随路能力，以利用硬件搬运能力提升算子整体执行性能。

- [矩阵计算的搬出总体说明](matrix_data_out_movement_overview.md)
- [L0C到GM数据搬运](l0c_to_gm_data_movement.md)
- [L0C到UB数据搬运](l0c_to_ub_data_movement.md)
- [随路量化](quant_pre.md)
- [随路Relu](relu_pre.md)
- [F32 Channel Split](f32_channel_split.md)
- [Int8/Int4 Channel Merge](int8_int4_channel_merge.md)
- [L0C到UB双目标模式](l0c_to_ub_dual_dst.md)
