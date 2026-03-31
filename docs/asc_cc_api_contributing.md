# CC API贡献指南

## 概述

集合通信库（Huawei Collective Communication Library，简称HCCL）是基于昇腾AI处理器的高性能集合通信库，为计算集群提供高性能、高可靠的通信方案，具备以下核心功能：

- 提供单机、多机环境中的高性能集合通信和点对点通信。
- 支持AllReduce、Broadcast、AllGather、ReduceScatter、AlltoAll等集合通信原语。
- 支持Ring、Mesh、Recursive Halving-Doubling（RHD）等通信算法。
- 支持HCCS、RoCE、PCIe等高速通信链路。
- 支持单算子和图模式两种执行模式。

HCCL是CANN的核心组件，对上支持多种AI框架，对下使能多款昇腾AI处理器之间的通信能力，其软件架构如下图所示：

<img src="./cc_api/figures/architecture.png" alt="hccl-architecture" style="width: 65%;  height:65%;" />

HCCL包含HCCL集合通信库与HCOMM（Huawei Communication）通信基础库：

- HCCL集合通信库：包含内置通信算子和扩展通信算子，提供对外的通信算子接口。
- [HCOMM通信基础库](https://gitcode.com/cann/hcomm)：采用分层解耦的设计思路，将通信能力划分为控制面和数据面两部分。

## 学习教程

HCCL提供了使用指南、通信算子开发指南、技术文章、培训视频，详细可参见 [HCCL 参考资料](./cc_api/README.md)。