# HCCL通信样例介绍
## 概述
本样例集介绍了HCCL Kernel侧通信接口的典型用法，给出了对应的端到端实现。

## 样例列表
| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [hccl_allreduce_ccu](./hccl_allreduce_ccu) | 本样例介绍了基于CCU服务端类型的HCCL AllReduce通信任务下发流程，涉及InitV2/SetCcTilingV2/AllReduce/Commit接口，给出了对应的端到端实现。 | Ascend 950PR/Ascend 950DT |
