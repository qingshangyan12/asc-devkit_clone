# BestPractices样例介绍

## 概述

基于Ascend C SIMT的算子开发实践，展示SIMT特性在实际算子开发中的运用。

## 样例列表

| 目录名称                                               | 功能描述                                                                                        |
| ------------------------------------------------------ | ----------------------------------------------------------------------------------------------- |
| [00_memory_optimizations](./00_memory_optimizations)   | 介绍SIMT编程方式下全局访存合并、片上缓存中转复用、bank冲突规避及短向量调优等访存优化方法。 |
| [02_simt_grid_config](./02_simt_grid_config)   | 样例以Gather+Adds融合计算为例，介绍Ascend C SIMD与SIMT混合编程方式下的gridDim配置和vf函数调用优化思路。 |