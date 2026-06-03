# SIMD与SIMT混合编程样例介绍

## 概述

本目录提供SIMD与SIMT混合编程相关的最佳实践样例，覆盖SIMT灵活实现分支判断逻辑、访存合并优化以及使用UB提升离散内存访问效率等场景。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [matrix_transpose](./matrix_transpose) |  本样例以矩阵转置为例，介绍Ascend C SIMD与SIMT混合编程场景下的访存合并优化思路，展示通过UB中转优化GM读写访问模式的方法。 | Ascend 950PR/Ascend 950DT |
| [simt_and_simd_high_performance](./simt_and_simd_high_performance) |  本样例以FloorMod计算为例，介绍SIMD与SIMT混合编程场景下的性能调优方法，展示SIMT直接访问GM、SIMD RegBase计算、SIMT访问UB以及调整线程映射使Warp内连续访问UB优化的性能差异。 | Ascend 950PR/Ascend 950DT |
