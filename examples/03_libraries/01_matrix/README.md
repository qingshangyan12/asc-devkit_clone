# Matmul API样例介绍
## 概述
本样例集介绍了Matmul API不同特性的典型用法，给出了对应的端到端实现。

## 算子开发样例
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [matmul_splitm](./matmul_splitm) | 本样例介绍Matmul API使用SplitM模板策略的场景 |
| [matmul_splitk](./matmul_splitk) | 本样例介绍调用Matmul高阶API实现多核切K场景下的单算子。多核切K的应用场景为矩阵乘的M、N较小，不能在M、N方向开启多核，需要切K满足将切分后的矩阵分配到更多核上并行处理。|
| [matmul_vecout](./matmul_vecout) | 本样例介绍Matmul API输入矩阵为VECOUT的场景 |
| [matmul_bias_bf16_tscm](./matmul_bias_bf16_tscm) | 本样例介绍Matmu API Bias输入为TSCM bfloat16类型的场景 |
| [matmul_tscm_mdl_setorgshape](./matmul_tscm_mdl_setorgshape) | 本样例介绍Matmul API MDL模板 A矩阵输入为TSCM的场景|
| [fixpipe_co12gm_quantization_f322f16](./fixpipe_co12gm_quantization_f322f16/) | 本样例介绍基础API Fixpipe将矩阵乘的结果从CO1搬入GM，并使能随路量化将矩阵乘后float类型数据量化为half类型 |
| [matmul_perf](./matmul_perf) | 本样例介绍Matmul API实现三种性能优化特性（纯Cube模式、MDL模板、UnitFlag）的单算子|
| [fixpipe_co12gm_tensor_quantization_s322f16](./fixpipe_co12gm_tensor_quantization_s322f16) | 本样例介绍基础API Fixpipe将矩阵乘的结果从CO1搬入GM，并使能随路量化将矩阵乘后int32_t类型数据量化为half类型 |
| [load_data_with_transpose_b8](./load_data_with_transpose_b8) | 本样例介绍基础API LoadDataWithTranspose b8数据类型下的使用样例 |
| [load_data_with_transpose_b16](./load_data_with_transpose_b16) | 本样例介绍基础API LoadDataWithTranspose b16数据类型下的使用样例 |
| [load_data_with_transpose_b32](./load_data_with_transpose_b32) | 本样例介绍基础API LoadDataWithTranspose b32数据类型下的使用样例 |
| [mmad](./mmad) | 本样例介绍基于基础API实现矩阵乘Mmad样例 |
| [mmad_with_bias](./mmad_with_bias) | 本样例介绍基于基础API实现带Bias的矩阵乘Mmad样例 |
| [mmad_with_sparse](./mmad_with_sparse) | 本样例介绍基础API MmadWithSparse调用样例 |
| [matmul_triangle](./matmul_triangle) | 本样例通过使用Matmul模板参数MatmulPolicy中TrianUpperMatmulPolicy（上三角模板策略）和TrianLowerMatmulPolicy（下三角模板策略），实现了上下三角矩阵计算的单算子 |
| [matmul_partial_output](./matmul_partial_output) | 本样例介绍Matmul高阶API实现开启Partial Output功能的单算子 |
| [matmul_unitflag](./matmul_unitflag) | 本样例介绍Matmul API实现MDL模板开启UnitFlag功能的单算子 |
| [matmul_unaligned](./matmul_unaligned) | 本样例介绍Matmul高阶API实现多核非对齐切分的单算子 |
| [matmul_channelsplit](./matmul_channelsplit) | 本样例介绍Matmul API实现矩阵乘输出Channel拆分功能的单算子 |
| [matmul_quant](./matmul_quant) | 本样例介绍Matmul API实现int8类型输入、half类型输出的Matmul反量化场景的算子，支持同一系数的反量化模式和向量的反量化模式 |
| [matmul_int4](./matmul_int4) | 本样例介绍Matmul API实现int4数据类型输入，int32数据类型输出的单算子 |
| [matmul_tscm_src_vecout](./matmul_tscm_src_vecout) | 本样例介绍Matmul API使用数据来源为VECOUT的用户自定义TSCM的输入 |
| [matmul_nz](./matmul_nz) | 本样例介绍Matmul API输入矩阵内轴非256B对齐的场景下，在AIV核上使用DataCopyPad实现ND转换NZ格式的单算子 |
| [matmul_nbuffer33](./matmul_nbuffer33) | 本样例介绍Matmul API实现NBuffer33算法的单算子，以及介绍模板参数MatmulPolicy中NBuffer33MatmulPolicy的使用方式 |
| [matmul_k_reorder_load](./matmul_k_reorder_load) | 本样例介绍Matmul API使能K轴错峰加载数据的场景 |
