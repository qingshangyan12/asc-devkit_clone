---
okftype: reference.api.cpp
title: SetMMColumnMajor/SetMMRowMajor
resource: asc-devkit/docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute_aux_config/SetMMColumnMajor-SetMMRowMajor.md
description: 控制Mmad/MmadWithSparse优先通过M/N的哪个方向。
tag: [basic_api]
usage_dependencies:
  status: unconfirmed
signature:
- override_id: SetMMColumnMajor__z
  archs: ['3510', '2201', '3002']
  headers: [/include/basic_api/kernel_operator_mm_intf.h]
- override_id: SetMMRowMajor__z
  archs: ['3510', '2201', '3002']
  headers: [/include/basic_api/kernel_operator_mm_intf.h]
examples: []
timestamp: '2026-09-11'
---

# SetMMColumnMajor/SetMMRowMajor

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：支持
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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute_aux_config/SetMMColumnMajor-SetMMRowMajor_res.md#id1 -->

## 功能说明

控制Mmad/MmadWithSparse优先通过M/N的哪个方向。

## 函数原型

```yaml
api_signature:
  override_id: "SetMMColumnMajor__z"
  archs: ["3510", "2201", "3002"]
  headers: ["/include/basic_api/kernel_operator_mm_intf.h"]
```

```cpp
__aicore__ inline void SetMMColumnMajor() // 代表CUBE将首先通过M方向，然后通过N方向生成结果
```

```yaml
api_signature:
  override_id: "SetMMRowMajor__z"
  archs: ["3510", "2201", "3002"]
  headers: ["/include/basic_api/kernel_operator_mm_intf.h"]
```

```cpp
__aicore__ inline void SetMMRowMajor()   // 代表CUBE将首先通过N方向，然后通过M方向产生结果
```

## 参数说明

无

## 返回值说明

无

## 约束说明

无

## 调用示例

```cpp
AscendC::SetMMRowMajor();
```
