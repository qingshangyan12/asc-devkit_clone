# Vector逻辑架构与Unified Buffer介绍<a name="ZH-CN_TOPIC_0000002573902865"></a>

## NPU硬件架构

### 概述

如下图所示，基于Ascend C开发的算子运行在AI Core上。Ascend C编程模型基于AI Core硬件架构的抽象进行介绍，了解硬件架构能够帮助开发者更好地理解Memory矢量计算接口。

**图 1**  NPU硬件架构

![](../../../../../guide/figures/概述.png)

AI Core负责执行矩阵、矢量计算密集的任务，其包括以下组成部分：

- 计算单元：包括Cube（矩阵）计算单元、Vector（矢量）计算单元和Scalar（标量）计算单元。
- 存储单元：包括L1 Buffer、L0A Buffer、L0B Buffer、L0C Buffer、**Unified Buffer**、BiasTable Buffer、Fixpipe Buffer等专为高效计算设计的存储单元。
- 搬运单元：包括MTE1、MTE2、MTE3和Fixpipe，用于数据在不同存储单元之间的高效传输。

### 硬件架构说明（含Vector逻辑架构）

<cann-filter npu-type="A3,910b">

- 针对以下型号，NPU硬件架构（含Vector逻辑架构）可参考[NPU架构版本2201](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/%E7%BC%96%E7%A8%8B%E6%8C%87%E5%8D%97/%E9%AB%98%E7%BA%A7%E7%BC%96%E7%A8%8B/%E7%A1%AC%E4%BB%B6%E5%AE%9E%E7%8E%B0/%E6%9E%B6%E6%9E%84%E8%A7%84%E6%A0%BC/NPU%E6%9E%B6%E6%9E%84%E7%89%88%E6%9C%AC2201.md)。

  - Atlas A3 训练系列产品/Atlas A3 推理系列产品
  - Atlas A2 训练系列产品/Atlas A2 推理系列产品

</cann-filter>

<cann-filter npu-type="950">

- 针对Ascend 950PR/Ascend 950DT，NPU硬件架构（含Vector逻辑架构）可参考[NPU架构版本3510](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/%E7%BC%96%E7%A8%8B%E6%8C%87%E5%8D%97/%E9%AB%98%E7%BA%A7%E7%BC%96%E7%A8%8B/%E7%A1%AC%E4%BB%B6%E5%AE%9E%E7%8E%B0/%E6%9E%B6%E6%9E%84%E8%A7%84%E6%A0%BC/NPU%E6%9E%B6%E6%9E%84%E7%89%88%E6%9C%AC3510.md)。

</cann-filter>

## Unified Buffer介绍<a name="ZH-CN_TOPIC_0000002574022839"></a>

### 通用说明和约束

- Unified Buffer地址对齐约束，请参考[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)中的`Unified Buffer`内容。
- Unified Buffer地址重叠约束，请参考[通用地址重叠约束](../../../通用说明和约束.md#section668772811100)。

### Unified Buffer的内存结构介绍与bank冲突

为了提高数据访问的效率和吞吐量，Unified Buffer采用了大小相等的内存模块（bank）结构设计。当多条读写指令同时访问Unified Buffer时，由于硬件资源的限制，这些指令不能同时执行，从而引发bank冲突。在这种情况下，指令需要排队等待资源，无法在一个指令周期内完成。

<cann-filter npu-type="A3,910b">

- 针对以下型号，Unified Buffer的内存结构介绍和bank冲突优化，可参考[避免bank冲突（NPU架构版本2201）](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/%E7%AE%97%E5%AD%90%E5%AE%9E%E8%B7%B5%E5%8F%82%E8%80%83/SIMD%E7%AE%97%E5%AD%90%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/%E5%86%85%E5%AD%98%E8%AE%BF%E9%97%AE/%E9%81%BF%E5%85%8DUB%E7%9A%84bank%E5%86%B2%E7%AA%81/%E9%81%BF%E5%85%8Dbank%E5%86%B2%E7%AA%81%EF%BC%88NPU%E6%9E%B6%E6%9E%84%E7%89%88%E6%9C%AC2201%EF%BC%89.md)。
  - Atlas A3 训练系列产品/Atlas A3 推理系列产品
  - Atlas A2 训练系列产品/Atlas A2 推理系列产品

</cann-filter>

<cann-filter npu-type="950">

- 针对Ascend 950PR/Ascend 950DT，Unified Buffer的内存结构介绍和bank冲突优化，可参考[避免bank冲突（NPU架构版本3510）](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/%E7%AE%97%E5%AD%90%E5%AE%9E%E8%B7%B5%E5%8F%82%E8%80%83/SIMD%E7%AE%97%E5%AD%90%E6%80%A7%E8%83%BD%E4%BC%98%E5%8C%96/%E5%86%85%E5%AD%98%E8%AE%BF%E9%97%AE/%E9%81%BF%E5%85%8DUB%E7%9A%84bank%E5%86%B2%E7%AA%81/%E9%81%BF%E5%85%8Dbank%E5%86%B2%E7%AA%81%EF%BC%88NPU%E6%9E%B6%E6%9E%84%E7%89%88%E6%9C%AC3510%EF%BC%89.md)。

</cann-filter>
