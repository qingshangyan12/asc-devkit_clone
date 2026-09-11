---
okftype: reference.api.cpp
title: End
resource: asc-devkit/docs/zh/api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/End.md
description: 多个Matmul对象之间切换计算时，必须调用一次End函数，用于释放Matmul计算资源，防止多个Matmul对象的计算资源冲突。
tag: [adv_api]
usage_dependencies:
  status: unconfirmed
signature:
- override_id: End__z
  archs: ['3510', '2201', '3002', '2002', x90, '9030']
  headers: [/include/adv_api/matmul/matmul.h]
examples: []
timestamp: '2026-09-11'
---

# End

## 产品支持情况

<!-- npu="950" id3 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id3 -->
<!-- npu="A3" id4 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id4 -->
<!-- npu="910b" id5 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id5 -->
<!-- npu="310b" id6 -->
- Atlas 200I/500 A2 推理产品：支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品AI Core：支持
- Atlas 推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持
<!-- end id8 -->
<!-- npu="x90" id1 -->
- Kirin X90：支持
<!-- end id1 -->
<!-- npu="9030" id2 -->
- Kirin 9030：支持
<!-- end id2 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/End_res.md#id1 -->

## 功能说明

多个Matmul对象之间切换计算时，必须调用一次End函数，用于释放Matmul计算资源，防止多个Matmul对象的计算资源冲突。

## 函数原型

```yaml
api_signature:
  override_id: "End__z"
  archs: ["3510", "2201", "3002", "2002", "x90", "9030"]
  headers: ["/include/adv_api/matmul/matmul.h"]
```

```
__aicore__ inline void End()
```

## 参数说明

无

## 返回值说明

无

## 约束说明

无

## 调用示例

```
mm1.IterateAll(gm_c1);
mm1.End();
mm2.IterateAll(gm_c2);
mm2.End();
```
