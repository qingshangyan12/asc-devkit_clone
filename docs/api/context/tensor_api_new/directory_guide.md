# Tensor API资料目录导览

- [基础数据结构](overview.md)

- [矩阵计算](overview.md)
    - [概述](matrix_computation/overview.md)
        - [矩阵计算单元](matrix_computation/matrix_compute_unit.md)
        - [矩阵计算流程](matrix_computation/matrix_computation_flow.md)
            - [普通场景矩阵计算流程](matrix_computation/normal_matrix_compute_flow.md)

    - [矩阵计算分形介绍](matrix_computation/fractal_format_overview.md)
        - [背景与核心概念](matrix_computation/fractal_format_concepts.md)
        - [关键分形格式详解](matrix_computation/key_fractal_formats.md)
        - [辅助矩阵分形格式详解](matrix_computation/auxiliary_fractal_formats.md)

    - [矩阵计算的搬入](data_movement/matrix_data_in_movement.md)
        - [总体说明](data_movement/matrix_data_in_movement_overview.md)
        - [矩阵数据搬入至L1 Buffer](data_movement/gm_to_l1_data_movement.md)
            - [GM到L1数据搬运](data_movement/gm_to_l1_data_movement.md)

        - [矩阵数据搬入至L0 Buffer](data_movement/l1_to_l0a_data_movement.md)
            - [L1到L0A数据搬运](data_movement/l1_to_l0a_data_movement.md)
            - [L1到L0B数据搬运](data_movement/l1_to_l0b_data_movement.md)
            - [L1到L0ScaleA数据搬运](data_movement/l1_to_l0scalea_data_movement.md)
            - [L1到L0ScaleB数据搬运](data_movement/l1_to_l0scaleb_data_movement.md)
            

        - [辅助数据搬运](data_movement/l1_to_ub_data_movement.md)
            - [L1到BiasTable Buffer数据搬运](data_movement/l1_to_bias_table_buffer_data_movement.md)
            - [L1到Fixpipe Buffer数据搬运](data_movement/l1_to_fixpipe_buffer_data_movement.md)
        
        - [矩阵数据搬入至UB](data_movement/l1_to_ub_data_movement.md)
            - [L1到UB数据搬运](data_movement/l1_to_ub_data_movement.md)

    - [Mmad计算](matrix_computation/mmad计算.md)
        - [普通场景矩阵计算流程](matrix_computation/normal_matrix_compute_flow.md)
        - [MX矩阵计算](matrix_computation/mx_matrix_computation.md)

    - [矩阵计算的搬出](data_movement/matrix_data_out_movement.md)
        - [总体说明](data_movement/matrix_data_out_movement_overview.md)
        - [L0C到GM数据搬运](data_movement/l0c_to_gm_data_movement.md)
        - [L0C到UB数据搬运](data_movement/l0c_to_ub_data_movement.md)
        - [关键特性说明](data_movement/matrix_data_out_movement_key_fratures.md)
            - [随路量化](data_movement/quant_pre.md)
            - [随路Relu](data_movement/relu_pre.md)
            - [F32 Channel Split](data_movement/f32_channel_split.md)
            - [Int8/Int4 Channel Merge](data_movement/int8_int4_channel_merge.md)
            - [L0C到UB双目标模式](data_movement/l0c_to_ub_dual_dst.md)
