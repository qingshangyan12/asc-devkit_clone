---
okftype: reference.api.cpp
title: "asc_pack_to_high"
resource: "asc-devkit/docs/zh/api/SIMD-API/c_api/reg_compute/reg_permute_sel/asc_pack_to_high.md"
description: "将源操作数中的元素选取低8位（b16）、低16位（b32）、低32位（b64）写入目的操作数的高半部分。 当源操作数为vector_bool时，其按bit位进行压缩，每2bit将其中的低位bit存储至目的操作数的高半部分。"
tag: [c_api, compute, 'operations:compression', vector, register_vector]
usage_dependencies:
  status: unconfirmed
signature:
  - override_id: "asc_pack_to_high__Rvu8_vu16"
    archs: ["3510"]
    headers: [/include/c_api/reg_compute/compute/reg_permute_sel.h]
  - override_id: "asc_pack_to_high__Rvu16_vu32"
    archs: ["3510"]
    headers: [/include/c_api/reg_compute/compute/reg_permute_sel.h]
  - override_id: "asc_pack_to_high__Rvb_vb"
    archs: ["3510"]
    headers: [/include/c_api/reg_compute/compute/reg_permute_sel.h]
examples: []
timestamp: "2026-09-10"
---

# asc_pack_to_high

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
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

头文件路径为：`"c_api/reg_compute/compute/reg_permute_sel.h"`。

将源操作数中的元素选取低8位（b16）、低16位（b32）、低32位（b64）写入目的操作数的高半部分。
当源操作数为vector_bool时，其按bit位进行压缩，每2bit将其中的低位bit存储至目的操作数的高半部分。

![](../../../../figures/pack_high_draft.png)

## 函数原型

```yaml
api_signature:
  override_id: "asc_pack_to_high__Rvu8_vu16"
  archs: ["3510"]
  headers: [/include/c_api/reg_compute/compute/reg_permute_sel.h]
```

```cpp
__simd_callee__ inline void asc_pack_to_high(vector_uint8_t& dst, vector_uint16_t src)
```

```yaml
api_signature:
  override_id: "asc_pack_to_high__Rvu16_vu32"
  archs: ["3510"]
  headers: [/include/c_api/reg_compute/compute/reg_permute_sel.h]
```

```cpp
__simd_callee__ inline void asc_pack_to_high(vector_uint16_t& dst, vector_uint32_t src)
```

```yaml
api_signature:
  override_id: "asc_pack_to_high__Rvb_vb"
  archs: ["3510"]
  headers: [/include/c_api/reg_compute/compute/reg_permute_sel.h]
```

```cpp
__simd_callee__ inline void asc_pack_to_high(vector_bool& dst, vector_bool src)
```

## 参数说明

**表1** 参数说明

| 参数名       | 输入/输出 | 描述               |
| --------- | ----- | ---------------- |
| dst       | 输出    | 目的操作数（矢量数据寄存器/掩码寄存器）。            |
| src | 输入    | 源操作数（矢量数据寄存器/掩码寄存器）。            |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

无

## 调用示例

```cpp
vector_uint8_t dst;
vector_uint16_t src;
asc_pack_to_high(dst, src);    // 将src的低8位写入dst的高半部分
```
