# SIMD与SIMT混合编程样例介绍

## 概述

本目录提供SIMD与SIMT混合编程相关的最佳实践样例，覆盖SIMT灵活实现分支判断逻辑、访存合并优化以及使用UB提升离散内存访问效率等场景。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [matrix_transpose](./matrix_transpose) |  本样例以矩阵转置为例，介绍Ascend C SIMD与SIMT混合编程场景下的访存合并优化思路，展示通过UB中转优化GM读写访问模式的方法。 | Ascend 950PR/Ascend 950DT |
| [simt_and_simd_floor_mod](./simt_and_simd_floor_mod) |  本样例以FloorMod算子为例，展示SIMT与SIMD混合编程的算子开发方式：使用SIMD DataCopy完成数据搬入搬出，float场景使用SIMD计算，int32_t场景使用SIMT计算。 | Ascend 950PR/Ascend 950DT |
| [simt_and_simd_high_performance](./simt_and_simd_high_performance) |  本样例以FloorMod计算为例，介绍SIMD与SIMT混合编程场景下的性能调优方法，展示SIMT直接访问GM、SIMD RegBase计算、SIMT访问UB以及Warp内访存合并优化的性能差异。 | Ascend 950PR/Ascend 950DT |
| [simt_gather_with_ub](./simt_gather_with_ub) |  本样例以Gather算子为例，展示SIMD与SIMT混合编程下使用UB提升离散内存访问效率的方法，对比SIMT直接访问GM与预先搬运到UB后再访问的性能差异。 | Ascend 950PR/Ascend 950DT |
