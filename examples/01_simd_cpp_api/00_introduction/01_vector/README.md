# Vector样例介绍

## 概述

基于Ascend C的向量计算样例，涵盖静态Tensor、TQue/TPipe以及RegBase/VF融合等不同实现方式。

## 样例列表

|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [add](./add) | 基于静态Tensor编程实现 |
| [gelu](./gelu) | 基于 LocalMemoryAllocator 和 RegBase/VF 融合的 GELU 计算实现 |
| [add_tpipe_tque](./add_tpipe_tque) | 基于TQue和TPipe编程实现 |
