---
okftype: reference.api.cpp
title: "asc_select"
resource: "asc-devkit/docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_select.md"
description: "执行矢量选择操作，给定两个源操作数src0和src1，根据条件选择元素，得到目的操作数dst。此接口配合[asc_set_cmp_mask(sel_mask)](../vector_compare/asc_set_cmp_mask.md)设置，规则为：当sel_mask的比特位是1时，从src0选取；当比特位是0时，从src1选择。"
tag: [c_api, compute, 'operations:selection', mask, vector, memory_vector, masked]
usage_dependencies:
  status: unconfirmed
signature:
  - override_id: "asc_select__PUf16_PUf16_PUf16_u32"
    archs: ["2201"]
    headers: [/include/c_api/vector_compute/compute/vector_permute_sel.h]
  - override_id: "asc_select__PUf32_PUf32_PUf32_u32"
    archs: ["2201"]
    headers: [/include/c_api/vector_compute/compute/vector_permute_sel.h]
  - override_id: "asc_select__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8"
    archs: ["2201"]
    headers: [/include/c_api/vector_compute/compute/vector_permute_sel.h]
  - override_id: "asc_select__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8"
    archs: ["2201"]
    headers: [/include/c_api/vector_compute/compute/vector_permute_sel.h]
  - override_id: "asc_select_sync__PUf16_PUf16_PUf16_u32"
    archs: ["2201"]
    headers: [/include/c_api/composite/vector_compute_composite.h]
  - override_id: "asc_select_sync__PUf32_PUf32_PUf32_u32"
    archs: ["2201"]
    headers: [/include/c_api/composite/vector_compute_composite.h]
examples: []
timestamp: "2026-09-10"
---

# asc_select

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/vector_compute/compute/vector_permute_sel.h"`。

执行矢量选择操作，给定两个源操作数src0和src1，根据条件选择元素，得到目的操作数dst。此接口配合[asc_set_cmp_mask(sel_mask)](../vector_compare/asc_set_cmp_mask.md)设置，规则为：当sel_mask的比特位是1时，从src0选取；当比特位是0时，从src1选择。

## 函数原型

- 前n个数据计算

```yaml
api_signature:
  override_id: "asc_select__PUf16_PUf16_PUf16_u32"
  archs: ["2201"]
  headers: [/include/c_api/vector_compute/compute/vector_permute_sel.h]
```

    ```cpp
    __aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
    ```

```yaml
api_signature:
  override_id: "asc_select__PUf32_PUf32_PUf32_u32"
  archs: ["2201"]
  headers: [/include/c_api/vector_compute/compute/vector_permute_sel.h]
```

    ```cpp
    __aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
    ```

- 高维切分计算

```yaml
api_signature:
  override_id: "asc_select__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8"
  archs: ["2201"]
  headers: [/include/c_api/vector_compute/compute/vector_permute_sel.h]
```

    ```cpp
    __aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
    ```

```yaml
api_signature:
  override_id: "asc_select__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8"
  archs: ["2201"]
  headers: [/include/c_api/vector_compute/compute/vector_permute_sel.h]
```

    ```cpp
    __aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
    ```

- 同步计算

```yaml
api_signature:
  override_id: "asc_select_sync__PUf16_PUf16_PUf16_u32"
  archs: ["2201"]
  headers: [/include/c_api/composite/vector_compute_composite.h]
```

    ```cpp
    __aicore__ inline void asc_select_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
    ```

```yaml
api_signature:
  override_id: "asc_select_sync__PUf32_PUf32_PUf32_u32"
  archs: ["2201"]
  headers: [/include/c_api/composite/vector_compute_composite.h]
```

    ```cpp
    __aicore__ inline void asc_select_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
    ```

## 参数说明

**表1** 参数说明

|参数名|输入/输出|描述|
| :--- | :--- | :--- |
| dst  | 输出  | 目的操作数（矢量）的起始地址。 |
| src0 | 输入  | 源操作数0（矢量）的起始地址。 |
| src1 | 输入  | 源操作数1（矢量）的起始地址。 |
| count | 输入    | 参与计算的元素个数。 |
| repeat | 输入    | 迭代次数。 |
| dst_block_stride | 输入    | 目的操作数单次迭代内不同DataBlock间地址步长。 |
| src0_block_stride | 输入    | 源操作数0单次迭代内不同DataBlock间地址步长。 |
| src1_block_stride | 输入    | 源操作数1单次迭代内不同DataBlock间地址步长。 |
| dst_repeat_stride | 输入    | 目的操作数相邻迭代间相同DataBlock的地址步长。 |
| src0_repeat_stride | 输入    | 源操作数0相邻迭代间相同DataBlock的地址步长。 |
| src1_repeat_stride | 输入    | 源操作数1相邻迭代间相同DataBlock的地址步长。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 需要配合[asc_set_cmp_mask](../vector_compare/asc_set_cmp_mask.md)接口使用。
- dst、src0、src1的起始地址需要32字节对齐
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

## 调用示例

```cpp
constexpr uint32_t total_length = 256;
constexpr uint32_t sel_length = 8;
__ubuf__ uint8_t sel_mask[sel_length];
__ubuf__ float dst[total_length];
__ubuf__ float src0[total_length];
__ubuf__ float src1[total_length];
asc_set_cmp_mask(sel_mask);
asc_sync_pipe(PIPE_V);
asc_select(dst, src0, src1, total_length);
```
