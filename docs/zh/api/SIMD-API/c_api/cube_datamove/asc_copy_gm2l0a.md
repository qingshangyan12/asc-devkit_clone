---
okftype: reference.api.cpp
title: "asc_copy_gm2l0a"
resource: "asc-devkit/docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_gm2l0a.md"
description: "将数据从GM搬运到L0A Buffer。"
tag: [c_api, data_movement, copy]
usage_dependencies:
  status: unconfirmed
signature:
  - override_id: "asc_copy_gm2l0a__PCAbf16_PGbf16_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
  - override_id: "asc_copy_gm2l0a__PCAf16_PGf16_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
  - override_id: "asc_copy_gm2l0a__PCAf32_PGf32_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
  - override_id: "asc_copy_gm2l0a__PCAi32_PGi32_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
  - override_id: "asc_copy_gm2l0a__PCAu8_PGu8_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
  - override_id: "asc_copy_gm2l0a__PCAi8_PGi8_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
  - override_id: "asc_copy_gm2l0a__PCAu32_PGu32_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
  - override_id: "asc_copy_gm2l0a__PCAi4x2_PGi4x2_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
  - override_id: "asc_copy_gm2l0a_sync__PCAbf16_PGbf16_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/composite/cube_datamove_composite.h"]
  - override_id: "asc_copy_gm2l0a_sync__PCAf16_PGf16_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/composite/cube_datamove_composite.h"]
  - override_id: "asc_copy_gm2l0a_sync__PCAf32_PGf32_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/composite/cube_datamove_composite.h"]
  - override_id: "asc_copy_gm2l0a_sync__PCAi32_PGi32_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/composite/cube_datamove_composite.h"]
  - override_id: "asc_copy_gm2l0a_sync__PCAu8_PGu8_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/composite/cube_datamove_composite.h"]
  - override_id: "asc_copy_gm2l0a_sync__PCAi8_PGi8_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/composite/cube_datamove_composite.h"]
  - override_id: "asc_copy_gm2l0a_sync__PCAu32_PGu32_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/composite/cube_datamove_composite.h"]
  - override_id: "asc_copy_gm2l0a_sync__PCAi4x2_PGi4x2_u16_u8_u16_u16"
    archs: ["2201"]
    headers: ["/include/c_api/composite/cube_datamove_composite.h"]
examples: []
timestamp: "2026-09-11"
---

# asc_copy_gm2l0a

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

头文件路径为：`"c_api/cube_datamove/cube_datamove.h"`。

将数据从GM搬运到L0A Buffer。

## 函数原型

- 常规搬运

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a__PCAbf16_PGbf16_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a(__ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a__PCAf16_PGf16_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a(__ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a__PCAf32_PGf32_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a(__ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a__PCAi32_PGi32_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a(__ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a__PCAu8_PGu8_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a(__ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a__PCAi8_PGi8_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a(__ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a__PCAu32_PGu32_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a(__ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a__PCAi4x2_PGi4x2_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/cube_datamove/cube_datamove.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a(__ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

- 同步搬运
    
```yaml
api_signature:
  override_id: "asc_copy_gm2l0a_sync__PCAbf16_PGbf16_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/composite/cube_datamove_composite.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a_sync__PCAf16_PGf16_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/composite/cube_datamove_composite.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a_sync__PCAf32_PGf32_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/composite/cube_datamove_composite.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a_sync__PCAi32_PGi32_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/composite/cube_datamove_composite.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a_sync__PCAu8_PGu8_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/composite/cube_datamove_composite.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a_sync__PCAi8_PGi8_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/composite/cube_datamove_composite.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a_sync__PCAu32_PGu32_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/composite/cube_datamove_composite.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

```yaml
api_signature:
  override_id: "asc_copy_gm2l0a_sync__PCAi4x2_PGi4x2_u16_u8_u16_u16"
  archs: ["2201"]
  headers: ["/include/c_api/composite/cube_datamove_composite.h"]
```

    ```cpp
    __aicore__ inline void asc_copy_gm2l0a_sync(__ca__ int4b_t* dst, __gm__ int4b_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

## 参数说明

**表1** 参数说明

| 参数名 |输入/输出| 描述                                                        |
|-----| ------------ |-----------------------------------------------------------|
| dst | 输出 | 目的操作数起始地址。                                                |
| src | 输入 | 源操作数起始地址。                                                 | 
| base_idx | 输入 | 分形矩阵ID，搬运起始位置为源操作数的第几个分形（0为源操作数中第1个分形矩阵）。取值范围：[0, 65535]。 |
| repeat | 输入 | 迭代次数，每个迭代可以处理512B数据。取值范围：[1, 255]。                        |
| src_stride | 输入 | 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B。取值范围：[0, 65535]。    |
| dst_gap | 输入 | 相邻迭代间，目的操作数前一个迭代第一个分形的结束地址到下一个迭代第一个分形起始地址的间隔。单位：512B。取值范围：[0, 65535]。   |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_description_and_constraints.md#通用地址重叠约束)。

## 调用示例
```cpp
__ca__ half dst[256];
// src表示源操作数的起始地址
asc_copy_gm2l0a(dst, src, 0, 1, 64, 64);
```
