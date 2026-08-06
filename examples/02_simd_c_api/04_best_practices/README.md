# BestPractices样例介绍

## 概述

本目录提供基于Ascend C C API的性能优化实践。各样例包含相同计算规格下的基线场景和优化场景，用于说明缓冲区设计与流水同步对性能的影响。

## 样例列表

| 目录名称 | 功能描述 |
| --- | --- |
| [00_vector_compute](./00_vector_compute) | Vector计算优化样例，介绍GM/UB数据搬运与Vector计算的双缓冲流水方法。 |
| [01_matrix_compute](./01_matrix_compute) | Matrix计算优化样例，介绍GM、L1、L0与Cube计算之间的双缓冲流水方法。 |
