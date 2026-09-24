---
okftype: reference.api.cpp
title: asc_rsqrt
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/vector_compute/vector_arith/asc_rsqrt.md
description: 按元素对源矢量先开方再取倒数，将平方根倒数写入目的矢量，支持half与float数据类型。
tag:
  - api_surfaces: [c_api]
  - roles: [compute]
  - operations: [reciprocal_sqrt]
  - execution_models: [memory_vector]
  - traits: [elementwise]
signature:
  - override_id: asc_rsqrt__PUf16_PUf16_u32
    archs: ['2201']
    headers: [/include/c_api/vector_compute/compute/vector_arith.h]
  - override_id: asc_rsqrt__PUf32_PUf32_u32
    archs: ['2201']
    headers: [/include/c_api/vector_compute/compute/vector_arith.h]
  - override_id: asc_rsqrt__PUf16_PUf16_u8_u16_u16_u16_u16
    archs: ['2201']
    headers: [/include/c_api/vector_compute/compute/vector_arith.h]
  - override_id: asc_rsqrt__PUf32_PUf32_u8_u16_u16_u16_u16
    archs: ['2201']
    headers: [/include/c_api/vector_compute/compute/vector_arith.h]
dependencies: []
examples: []
timestamp: '2026-09-23'
---
# asc_rsqrt

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：不支持
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

头文件路径为：`"c_api/vector_compute/compute/vector_arith.h"`。

按元素进行开方后取倒数的计算，计算公式如下：

$$
dst_i = 1/(sqrt(src_i))
$$

## 函数原型

- 前n个数据计算

  ```cpp
  __aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
  __aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
  ```

- 高维切分计算

  ```cpp
  __aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
  __aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
  ```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述                          |
| :----| :-----|:----------------------------|
| dst | 输出 | 目的操作数（矢量）的起始地址。             |
| src  | 输入 | 源操作数（矢量）的起始地址。              |
| count | 输入 | 参与计算的元素个数。                  |
| repeat | 输入 | 迭代次数。                       |
| dst_block_stride | 输入 | 目的操作数单次迭代内不同DataBlock间地址步长。 |
| src_block_stride | 输入 | 源操作数单次迭代内不同DataBlock间地址步长。  |
| dst_repeat_stride | 输入 | 目的操作数相邻迭代间相同DataBlock的地址步长。 |
| src_repeat_stride | 输入 | 源操作数相邻迭代间相同DataBlock的地址步长。  |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- dst、src的起始地址需要32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

## 调用示例

```cpp
// total_length指参与计算的数据总长度
constexpr uint64_t total_length = 128;
__ubuf__ half src[total_length];
__ubuf__ half dst[total_length];
asc_rsqrt(dst, src, total_length);
```
