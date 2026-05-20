# 概述

矩阵计算章节介绍 Tensor API 中矩阵计算相关的硬件单元、基础计算流程和 Mmad 计算接口。矩阵计算通常以 L0A Buffer 中的左矩阵、L0B Buffer 中的右矩阵作为输入，将矩阵乘加结果写入 L0C Buffer，并通过矩阵计算搬出接口将结果写回 GM 或搬运至 UB。

- [矩阵计算单元](matrix_compute_unit.md)
- [矩阵计算流程](matrix_computation_flow.md)
  - [普通矩阵计算流程](normal_matrix_compute_flow.md)
- [普通矩阵计算](normal_matrix_computation.md)
- [MX矩阵计算](mx_matrix_computation.md)

