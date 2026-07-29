# C API公共概述

本目录提供C API的存储、流水、计算和同步总体说明，用于在阅读单个接口前建立统一认知。C API以地址空间限定数组、裸指针和显式函数参数表达存储位置及操作数。

## 导航

- [数据搬运导览](数据搬运导览.md)：存储层级、数据通路、流水、对齐和通路到C API的选择关系。
- [矢量计算概述](矢量计算概述.md)：Memory/Reg两类矢量编程模型、掩码、计算及搬运入口。
- [矩阵计算概述](矩阵计算概述.md)：GM、L1 Buffer、L0A Buffer/L0B Buffer/L0C Buffer、BiasTable Buffer/Fixpipe Buffer的矩阵计算链路。
- [同步、原子与缓存概述](同步原子缓存概述.md)：异步流水依赖、原子配置和缓存可见性。
- [矢量子分类概览](矢量子分类概览.md)：Memory/Reg矢量计算的全部子分类到C API章节的映射。
- [矩阵子分类与关键特性概览](矩阵子分类与关键特性概览.md)：矩阵搬入、Mmad、搬出及关键特性到C API的映射。
- [特殊寄存器访问概览](特殊寄存器访问概览.md)：特殊寄存器访问到C API系统变量接口的映射。
- [实现索引附录](../../implementation_indexes/dav_3510/README.md)：`dav-3510` CCE指令名称级索引、置信度和复合实现说明。

产品支持、参数、数据类型和约束均以每个C API接口文档为准。

## C API章节入口

| 使用目标 | C API章节 |
|---|---|
| 设备端数据结构和通用约束 | [数据结构](../struct/数据结构.md)、[通用说明和约束](../通用说明和约束.md) |
| GM/UB数据搬运与多维搬运 | [矢量数据搬运](../vector_datamove/矢量数据搬运.md) |
| GM/L1 Buffer/L0 Buffer/专用Buffer矩阵搬运 | [矩阵数据搬运](../cube_datamove/矩阵数据搬运.md) |
| UB数组矢量计算 | [Memory矢量计算](../vector_compute/Memory矢量计算.md) |
| 寄存器矢量计算 | [reg矢量计算](../reg/reg.md) |
| 标量、矩阵、同步、原子、缓存 | [标量计算](../scalar_compute/标量计算.md)、[矩阵计算](../cube_compute/矩阵计算.md)、[同步控制](../sync/同步控制.md)、[原子操作](../simd_atomic/原子操作.md)、[缓存控制](../cache_ctrl/缓存控制.md) |
