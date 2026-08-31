# 矩阵计算类Tensor API样例介绍

## 概述

本路径下包含矩阵计算相关的Tensor API样例，展示GM、L1、L0A、L0B、L0C和UB之间的数据搬运，以及`mmad`矩阵乘加、卷积和量化等计算流程。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| --- | --- | --- |
| [matmul](./matmul) | 基于Tensor API静态编程方式实现矩阵乘计算。 | Ascend 950PR/Ascend 950DT |
| [copy_in](./copy_in) | 实现带Bias的动态Shape矩阵乘，展示GM到L1、L1到L0的数据搬入接口和`mmad`计算。 | Ascend 950PR/Ascend 950DT |
| [copy_out](./copy_out) | 实现带Bias的动态Shape矩阵乘，展示L0C到GM、L0C到UB的数据搬出接口和`mmad`计算。 | Ascend 950PR/Ascend 950DT |
| [batch_matmul](./batch_matmul) | 基于Tensor API实现带Bias的Batch Matmul计算。 | Ascend 950PR/Ascend 950DT |
| [conv2d_forward](./conv2d_forward) | 基于Tensor API实现Conv2D Forward计算，支持NC1HWC0、NCHW和NHWC三种GM输入输出格式。 | Ascend 950PR/Ascend 950DT |
| [mmad](./mmad) | 基于Tensor API实现动态Shape矩阵乘和随路量化计算。 | Ascend 950PR/Ascend 950DT |
| [matmul_mxfp4_high_performance](./matmul_mxfp4_high_performance) | 基于静态Tensor编程实现MxFP4 Matmul，展示多核切分、大包搬运和双缓冲流水优化。 | Ascend 950PR/Ascend 950DT |
