# Vector样例介绍

## 概述

基于Ascend C的向量计算样例，涵盖静态Tensor、TQue/TPipe以及RegBase/VF融合等不同实现方式。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [add](./add) |  基于静态Tensor编程实现 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [add_tpipe_tque](./add_tpipe_tque) |  基于TQue和TPipe编程实现 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [gelu](./gelu) | 基于 LocalMemoryAllocator 和 RegBase/VF 融合的 GELU 计算实现 | Ascend 950PR/Ascend 950DT |