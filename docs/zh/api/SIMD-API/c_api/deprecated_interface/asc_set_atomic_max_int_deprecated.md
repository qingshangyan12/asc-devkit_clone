---
okftype: reference.api.cpp
title: asc_set_atomic_max_int（废弃）
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/deprecated_interface/asc_set_atomic_max_int_deprecated.md
description: 已废弃接口，设置计算结果以原子比较方式传输到GM，拷贝前将int32_t数据与GM已有数据逐元素比较并将最大值写入GM，请使用asc_set_atomic_max_int32替代。
tag:
  - api_surfaces: [c_api]
  - roles: [data_movement_configuration]
  - operations: [maximum]
  - execution_models: [scalar]
  - traits: [atomic, deprecated]
signature:
  - override_id: asc_set_atomic_max_int__z
    archs: ['2201', '3510']
    headers: [/include/c_api/atomic/datamove_atomic.h]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/atomic/datamove_atomic/asc_disable_dma_atomic.md
    kind: related
    condition: 使用结束后，建议通过asc_disable_dma_atomic关闭原子最大操作
  - path: docs/zh/api/SIMD-API/c_api/atomic/datamove_atomic/asc_set_atomic_max.md
    kind: related
examples: []
timestamp: '2026-09-24'
---
# asc_set_atomic_max_int（废弃）

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

**该接口已废弃，请使用[asc_set_atomic_max_int32](../atomic/datamove_atomic/asc_set_atomic_max.md)替代。**

设置计算结果以原子比较的方式传输到GM。在拷贝前，将待传输的int32_t数据与GM中已有数据进行逐元素比较，并将最大值写入GM。

## 函数原型

```c
[[deprecated("NOTICE: asc_set_atomic_max_int is deprecated. Please use asc_set_atomic_max_int32 instead.")]]
__aicore__ inline void asc_set_atomic_max_int()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

使用结束后，建议通过[asc_disable_dma_atomic](../atomic/datamove_atomic/asc_disable_dma_atomic.md)关闭原子最大操作，以免影响后续相关指令功能。

## 调用示例

请参考[asc_set_atomic_max](../atomic/datamove_atomic/asc_set_atomic_max.md)的调用示例。
