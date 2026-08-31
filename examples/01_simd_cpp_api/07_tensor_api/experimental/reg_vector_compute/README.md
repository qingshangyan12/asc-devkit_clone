# 寄存器向量计算类Tensor API样例介绍

## 概述

本路径下包含实验性寄存器Tensor API样例，展示Tensor在GM与UB之间搬运后，通过寄存器加载、计算和存储接口完成向量处理的编程方式。

## 样例列表

| 目录名称 | 功能描述 | 支持的产品 |
| --- | --- | --- |
| [copy_vector](./copy_vector) | 展示GM到UB、UB到GM和UB到UB的Tensor搬运，包括一维搬运和左右常量填充场景等。 | Ascend 950PR/Ascend 950DT |
| [cast](./cast) | 使用实验性寄存器Tensor加载、类型转换和存储接口完成多种数据类型之间的逐元素转换。 | Ascend 950PR/Ascend 950DT |
| [trunc](./trunc) | 使用实验性寄存器Tensor接口对FP16、BF16和FP32数据执行逐元素向零取整。 | Ascend 950PR/Ascend 950DT |
