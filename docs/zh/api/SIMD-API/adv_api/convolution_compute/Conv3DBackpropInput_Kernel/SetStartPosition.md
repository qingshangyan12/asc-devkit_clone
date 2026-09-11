---
okftype: reference.api.cpp
title: SetStartPosition
resource: asc-devkit/docs/zh/api/SIMD-API/adv_api/convolution_compute/Conv3DBackpropInput_Kernel/SetStartPosition.md
description: 设置单核上GradOutput载入数据的起始位置。
tag: [adv_api]
usage_dependencies:
  status: unconfirmed
signature:
- override_id: SetStartPosition__u32_i32
  archs: ['2201']
  headers: [/include/adv_api/conv_backprop/conv3d_bp_input_intf.h]
examples: []
timestamp: '2026-09-11'
---

# SetStartPosition

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
- Atlas 推理系列产品Vector Core：不支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas 训练系列产品：不支持
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/adv_api/convolution_compute/Conv3DBackpropInput_Kernel/SetStartPosition_res.md#id1 -->

## 功能说明

设置单核上GradOutput载入数据的起始位置。

## 函数原型

```yaml
api_signature:
  override_id: "SetStartPosition__u32_i32"
  archs: ["2201"]
  headers: ["/include/adv_api/conv_backprop/conv3d_bp_input_intf.h"]
```

```
__aicore__ inline void SetStartPosition(uint32_t curDinStartIdx, int32_t curHoStartIdx)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| curDinStartIdx | 输入 | 当前核D方向起始位置。 |
| curHoStartIdx | 输入 | 当前核H方向起始位置。 |

## 返回值说明

无

## 约束说明

无

## 调用示例

```
gradInput_.SetStartPosition(dinStartIdx_, curHoStartIdx_); // 设置单核上GradOutput载入的起始位置
```

