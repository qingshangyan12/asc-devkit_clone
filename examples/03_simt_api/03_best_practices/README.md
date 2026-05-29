# BestPractices样例介绍

## 概述

基于Ascend C SIMT的算子开发实践，展示SIMT特性在实际算子开发中的运用。

## 样例列表

| 目录名称                                               | 功能描述                                                                                        |
| ------------------------------------------------------ | ----------------------------------------------------------------------------------------------- |
| [00_overhead_reduction](./00_overhead_reduction)   | 介绍SIMT编程方式下降低kernel启动、调度同步及冗余指令等额外开销的调优方法。 |
| [01_memory_optimizations](./01_memory_optimizations)   | 介绍SIMT编程方式下全局访存合并、片上缓存中转复用、bank冲突规避及短向量调优等访存优化方法。 |
| [02_execution_conf_optimizations](./02_execution_conf_optimizations)   |介绍SIMT编程方式下通过调整block/grid划分等执行配置提升并行度与资源利用率的调优方法。 |