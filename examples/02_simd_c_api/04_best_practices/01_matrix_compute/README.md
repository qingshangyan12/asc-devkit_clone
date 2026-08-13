# Matrix Compute Practices样例介绍

## 概述

本目录提供基于Ascend C C API的Matrix计算性能优化样例，重点介绍GM、L1、L0与Cube计算之间的流水优化方法。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| --- | --- | --- |
| [matmul_c_api_high_performance](./matmul_c_api_high_performance) | Matmul C-API高性能样例，通过相同矩阵规格下的单缓冲基线与L1/L0 Ping-Pong双缓冲场景，对比MTE2、MTE1和Cube流水重叠带来的性能变化。 | Ascend 950PR/Ascend 950DT |
