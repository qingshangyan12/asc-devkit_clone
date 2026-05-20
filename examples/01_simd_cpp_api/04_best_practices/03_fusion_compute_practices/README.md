# Fusion Compute Practices样例介绍

## 概述

本目录提供融合计算相关的最佳实践样例，覆盖QuantGroupMatmul分组量化矩阵乘、Matmul+GELU的Cube-Vector融合实现，以及SIMT与SIMD混合编程的高性能优化方法。

## 样例列表

| 目录名称 | 功能描述 |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [grouped_matmul](./grouped_matmul) | 介绍QuantGroupMatmul算子的高性能实现，支持分组量化矩阵乘与GELU激活计算。 |
| [matmul_gelu_high_performance](./matmul_gelu_high_performance) | 展示Cube-Vector融合的高性能实现，将Matmul矩阵乘法与GELU激活函数融合到同一AI Core中并行执行。 |
| [simt_and_simd_high_performance](./simt_and_simd_high_performance) | 以FloorMod计算为例，展示SIMT与SIMD混合编程场景下的性能调优方法。 |
