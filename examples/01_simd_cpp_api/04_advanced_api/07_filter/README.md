# data_filter算子样例介绍

## overview

本样例集介绍了data_filter算子不同特性的典型用法，给出了对应的端到端实现。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [dropout](./dropout) |  本样例介绍了调用DropOuthigh_level_api实现dropout算子，提供根据MaskTensor对SrcTensor（源操作数，输入Tensor）进行过滤的功能，得到DstTensor（目的操作数、输出Tensor） | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [select](./select) |  本样例基于Kernel直调算子工程，介绍了调用Selecthigh_level_api实现SelectCustom单算子，主要演示Selecthigh_level_api在Kernel直调工程中的调用 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |