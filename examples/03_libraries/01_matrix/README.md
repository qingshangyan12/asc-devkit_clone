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
| [fixpipe_co12gm_quantization_f322f16](./fixpipe_co12gm_quantization_f322f16/) | 本样例介绍基础API Fixpipe将矩阵乘的结果从CO1搬入GM，并使能随路量化将矩阵乘后float类型数据量化为half类型。|