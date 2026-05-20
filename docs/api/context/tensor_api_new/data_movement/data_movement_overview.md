# 数据搬运导览

Tensor API 数据搬运资料按矩阵计算流程分为搬入和搬出两部分。矩阵计算前，通常需要将输入矩阵从 GM 搬入 L1、L0A、L0B 等存储层级；矩阵计算后，需要将 L0C 中的计算结果搬出到 GM 或 UB。

## 矩阵计算的搬入

矩阵计算的搬入用于准备 `Mmad` 的输入数据，主要覆盖 GM 到 L1、L1 到 L0A、L1 到 L0B，以及 BiasTable Buffer、Fixpipe Buffer、UB 等辅助搬运场景。

- [矩阵计算的搬入总体说明](matrix_data_in_movement_overview.md)
- [GM到L1数据搬运](gm_to_l1_data_movement.md)
- [L1到L0A数据搬运](l1_to_l0a_data_movement.md)
- [L1到L0B数据搬运](l1_to_l0b_data_movement.md)
- [L1到BiasTable Buffer数据搬运](l1_to_bias_table_buffer_data_movement.md)
- [L1到Fixpipe Buffer数据搬运](l1_to_fixpipe_buffer_data_movement.md)
- [L1到UB数据搬运](l1_to_ub_data_movement.md)

## 矩阵计算的搬出

矩阵计算的搬出用于处理 `Mmad` 写入 L0C 后的结果数据，主要覆盖 L0C 到 GM、L0C 到 UB，以及随路量化、随路 Relu、Channel Split、Channel Merge、UB 双目标模式等关键特性。

- [矩阵计算的搬出总体说明](matrix_data_out_movement_overview.md)
- [L0C到GM数据搬运](l0c_to_gm_data_movement.md)
- [L0C到UB数据搬运](l0c_to_ub_data_movement.md)
- [随路量化](quant_pre.md)
- [随路Relu](relu_pre.md)
- [F32 Channel Split](f32_channel_split.md)
- [Int8/Int4 Channel Merge](int8_int4_channel_merge.md)
- [L0C到UB双目标模式](l0c_to_ub_dual_dst.md)
