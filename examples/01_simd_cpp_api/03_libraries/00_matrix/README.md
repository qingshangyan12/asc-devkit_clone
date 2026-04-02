# Matmul API样例介绍
## 概述
本样例集介绍了Matmul API不同特性的典型用法，给出了对应的端到端实现。

## 样例开发样例
| 目录名称                                                                                       |  功能描述                                              |
|--------------------------------------------------------------------------------------------| ---------------------------------------------------- |
| [batch_matmul](./batch_matmul)                                                             | 本样例介绍批量处理Matmul计算的Batch Matmul样例 |
| [batch_matmul_bias_reuse](./batch_matmul_bias_reuse)                                       | 本样例介绍批量处理Matmul计算时，复用同一个Bias矩阵的Batch Matmul样例 |
| [batch_matmul_iterate_n_batch](./batch_matmul_iterate_n_batch)                             | 本样例介绍多次批量处理Matmul计算的Batch Matmul样例 |
| [matmul](./matmul)                                                                         | 本样例介绍调用Matmul API实现matmul样例 |
| [matmul_a2b2_share](./matmul_a2b2_share)                                                   | 本样例介绍开启 A2(L0A Buffer) 和 B2(L0B Buffer) 全局管理的Matmul样例。 |
| [matmul_async_iterate](./matmul_async_iterate)                                             | 本样例介绍异步场景下的Matmul样例，实现方式为调用Iterate和GetTensorC输出到VECIN |
| [matmul_async_iterate_all](./matmul_async_iterate_all)                                     | 本样例介绍异步场景下的Matmul样例，实现方式为调用IterateAll输出到GM |
| [matmul_callback](./matmul_callback)                                                       | 本样例介绍Matmul API模板参数MatmulCallbackFunc的自定义使用方式 |
| [matmul_channelsplit_output](./matmul_channelsplit_output)                                 | 本样例介绍矩阵乘输出Channel拆分功能的Matmul样例 |
| [matmul_co1_output](./matmul_co1_output)                                                   | 本样例介绍用户自主管理CO1（L0C Buffer）的Matmul样例 |
| [matmul_constant_tiling](./matmul_constant_tiling)                                         | 本样例介绍Tiling常量化的Matmul样例，在tiling参数固定的场景下，减少运行时的Scalar开销 |
| [matmul_format_column_major](./matmul_format_column_major)                                 | 本样例介绍输入输出矩阵为COLUMN_MAJOR（列优先）格式排布的Matmul样例 |
| [matmul_format_gemv](./matmul_format_gemv)                                                 | 本样例介绍实现矩阵向量乘GEMV（General Matrix-Vector multiplication）的Matmul样例 |
| [matmul_format_nd_align](./matmul_format_nd_align)                                         | 本样例介绍在输入矩阵的N方向非对齐场景下，矩阵乘输出时使能N方向对齐的Matmul样例 |
| [matmul_fp8](./matmul_fp8)                                                                 | 本样例介绍A、B矩阵为hifloat8、fp8_e4m3fn、fp8_e5m2数据类型输入的Matmul样例 |
| [matmul_fused](./matmul_fused)                                                             | 本样例介绍分核AIC、AIV融合编程实现，主要介绍了Matmul高阶API的MIX模式，Matmul API内部会自动控制AIC、AIV的核间同步 |
| [matmul_fused_mannul](./matmul_fused_mannul)                                               | 本样例介绍分核AIC、AIV融合编程实现，主要介绍了Matmul高阶API的纯Cube模式，需要调用相关接口实现手动控制AIC、AIV的核间同步 |
| [matmul_ibshareAB](./matmul_ibshareAB)                                                     | 本样例介绍调开启IBShare功能，复用L1 Buffer上相同的A矩阵或者B矩阵数据的样例，本样例为A矩阵和B矩阵同时复用场景 |
| [matmul_ibshareB](./matmul_ibshareB)                                                       | 本样例介绍调开启IBShare功能，复用L1 Buffer上相同的A矩阵或者B矩阵数据的样例，本样例为仅B矩阵复用场景 |
| [matmul_int4](./matmul_int4)                                                               | 本样例介绍A、B矩阵为int4b_t数据类型输入的Matmul样例 |
| [matmul_k_reorder_load](./matmul_k_reorder_load)                                           | 本样例介绍使能K轴错峰加载数据特性的Matmul样例，减少多核对Global Memory地址访问冲突概率 |
| [matmul_l0cache](./matmul_l0cache)                                                         | 本样例介绍使能L0缓存特性的Matmul样例，减少MTE1重复搬运 |
| [matmul_l2cache](./matmul_l2cache)                                                         | 本样例介绍使能L2 Cache切分特性的Matmul样例，提高L2 Cache的利用率 |
| [matmul_mixdualmaster](./matmul_mixdualmaster)                                             | 本样例介绍使能双主模式（MixDualMaster）的Matmul样例，AIC和AIV独立运行代码，不依赖消息驱动，用于提升性能 |
| [matmul_mn_double_buffer](./matmul_mn_double_buffer)                                       | 本样例介绍M/N轴方向的流水并行的Matmul样例 |
| [matmul_mx_norm_even](./matmul_mx_norm_even)                                               | 本样例介绍在Mx数据格式下，Scale的K方向为偶数的带有量化系数的矩阵乘法，即MxMatmul计算场景 |
| [matmul_mx_norm_odd](./matmul_mx_norm_odd)                                                 | 本样例介绍在Mx数据格式下，Scale的K方向为奇数的带有量化系数的矩阵乘法，即MxMatmul计算场景 |
| [matmul_mx_scalea_trans](./matmul_mx_scalea_trans)                                         | 本样例介绍在Mx数据格式下，scaleA开启转置、scaleB不开启转置的带有量化系数的矩阵乘法，即MxMatmul计算场景 |
| [matmul_mx_scaleb_trans](./matmul_mx_scaleb_trans)                                         | 本样例介绍在Mx数据格式下，scaleA不开启转置、scaleB开启转置的带有量化系数的矩阵乘法，即MxMatmul计算场景 |
| [matmul_mx_typepara](./matmul_mx_typepara)                                                 | 本样例介绍在Mx数据格式下，左量化系数矩阵scaleA载入L1时，scaleA的K方向上开启多倍缓存，从而实现带有量化系数的矩阵乘法，即MxMatmul计算场景。 |
| [matmul_mx_ub_tscm_nz](./matmul_mx_ub_tscm_nz)                                             | 本样例介绍在Mx数据格式下，A、B矩阵内存逻辑位置使用VECOUT，scaleA、scaleB矩阵内存逻辑位置使用TSCM，4个输入矩阵都是NZ格式的带有量化系数的矩阵乘法 |
| [matmul_nbuffer33](./matmul_nbuffer33)                                                     | 本样例介绍Matmul API实现NBuffer33算法的样例，以及介绍模板参数MatmulPolicy中NBuffer33MatmulPolicy的使用方式 |
| [matmul_partial_output](./matmul_partial_output)                                           | 本样例介绍Matmul高阶API实现开启Partial Output功能的样例 |
| [matmul_preload](./matmul_preload)                                                         | 本样例介绍调用Matmul MDL模板实现使能M或N方向预加载功能的样例 |
| [matmul_quant](./matmul_quant)                                                             | 本样例介绍Matmul API实现int8类型输入、half类型输出的Matmul反量化场景的样例，支持同一系数的反量化模式和向量的反量化模式 |
| [matmul_sparse](./matmul_sparse)                                                           | 本样例介绍使用Matmul API实现左矩阵A为稀疏矩阵，右矩阵B为4:2稠密化后的矩阵的Sparse Matmul场景的矩阵乘计算 |
| [matmul_splitk](./matmul_splitk)                                                           | 本样例介绍调用Matmul高阶API实现多核切K场景下的样例。多核切K的应用场景为矩阵乘的M、N较小，不能在M、N方向开启多核，需要切K满足将切分后的矩阵分配到更多核上并行处理。|
| [matmul_splitm](./matmul_splitm)                                                           | 本样例介绍Matmul API使用SplitM模板策略的场景 |
| [matmul_triangle](./matmul_triangle)                                                       | 本样例通过使用Matmul模板参数MatmulPolicy中TrianUpperMatmulPolicy（上三角模板策略）和TrianLowerMatmulPolicy（下三角模板策略），实现了上下三角矩阵计算的样例 |
| [matmul_tscm](./matmul_tscm)                                                               | 本样例介绍Matmul API中用户自定义TSCM输入的使用方式 |
| [matmul_tscm_src_vecout](./matmul_tscm_src_vecout)                                         | 本样例介绍Matmul API使用数据来源为VECOUT的用户自定义TSCM的输入 |
| [matmul_multi_core_unaligned](./matmul_multi_core_unaligned)                               | 本样例介绍Matmul高阶API实现多核非对齐切分的样例 |
| [matmul_unitflag](./matmul_unitflag)                                                       | 本样例介绍Matmul API实现MDL模板开启UnitFlag功能的样例 |
| [matmul_vecout](./matmul_vecout)                                                           | 本样例介绍Matmul API输入矩阵为VECOUT的场景 |
