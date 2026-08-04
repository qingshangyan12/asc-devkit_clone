# quantization算子样例介绍

## overview

本样例集介绍了quantization算子不同特性的典型用法，给出了对应的端到端实现。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [antiquant](./antiquant) |  本样例演示了基于AntiQuanthigh_level_api实现antiquant算子。样例按元素做伪量化计算 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [dequant](./dequant) |  本样例基于Kernel直调算子工程，介绍了调用Dequanthigh_level_api实现Dequant单算子，按元素做反量化计算，比如将int32_t数据类型反量化为half/float等数据类型 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [quant](./quant) |  本样例基于Kernel直调算子工程，介绍了调用Quanthigh_level_api实现QuantCustom单算子，按元素做量化计算，比如将half/float数据类型量化为int8_t数据类型 | Ascend 950PR/Ascend 950DT<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品<br>Atlas A2 训练系列产品/Atlas A2 推理系列产品 |