# asc_set_atomic_none（废弃）

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/atomic/datamove_atomic.h"`。

**该接口已废弃，请使用[asc_disable_dma_atomic](../atomic/datamove_atomic/asc_disable_dma_atomic.md)替代。**

与[asc_set_atomic_add](../atomic/datamove_atomic/asc_set_atomic_add.md)、[asc_set_atomic_max](../atomic/datamove_atomic/asc_set_atomic_max.md)、[asc_set_atomic_min](../atomic/datamove_atomic/asc_set_atomic_min.md)接口配合使用，用于在完成原子操作后关闭原子操作，避免影响后续搬运。

## 函数原型

```c
[[deprecated("NOTICE: asc_set_atomic_none is deprecated. Please use asc_disable_dma_atomic instead.")]]
__aicore__ inline void asc_set_atomic_none()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

请参见[asc_disable_dma_atomic的调用示例](../atomic/datamove_atomic/asc_disable_dma_atomic.md#调用示例)。

## 参见

| 接口 | 描述 |
| --- | --- |
| [asc_disable_dma_atomic](../atomic/datamove_atomic/asc_disable_dma_atomic.md) | 与asc_set_atomic_add、asc_set_atomic_max、asc_set_atomic_min接口配合使用，用于在完成原子操作后关闭原子操作，避免影响后续搬运。 |
| [asc_set_atomic_add](../atomic/datamove_atomic/asc_set_atomic_add.md) | 原子累加过程：将待搬运到GM的数据和GM上已有数据进行求和，然后将求和结果写入GM。 |
| [asc_set_atomic_max](../atomic/datamove_atomic/asc_set_atomic_max.md) | 原子取大过程：将待搬运到GM的数据和GM上已有数据进行比较，然后将两者中的最大值写入GM。 |
| [asc_set_atomic_min](../atomic/datamove_atomic/asc_set_atomic_min.md) | 将待搬运到GM的数据和GM上已有数据进行比较，然后将两者中的最小值写入GM。 |
