---
okftype: reference.api.cpp
title: GetRankId
resource: asc-devkit/docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/GetRankId.md
description: 获取本卡RankId。该接口默认在所有核上工作，用户也可以在调用前通过[GetBlockIdx](../../../basic_api/tool_interface/system_resources_and_variables/GetBlockIdx.md)指定其在某一个核上运行。
tag: [adv_api]
usage_dependencies:
  status: unconfirmed
signature:
- override_id: GetRankId__z
  archs: ['3510', '2201']
  headers: [/include/adv_api/hccl/hccl.h]
examples: []
timestamp: '2026-09-11'
---

# GetRankId

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
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas 训练系列产品：不支持
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/GetRankId_res.md#id1 -->

## 功能说明

获取本卡RankId。该接口默认在所有核上工作，用户也可以在调用前通过[GetBlockIdx](../../../basic_api/tool_interface/system_resources_and_variables/GetBlockIdx.md)指定其在某一个核上运行。

## 函数原型

```yaml
api_signature:
  override_id: "GetRankId__z"
  archs: ["3510", "2201"]
  headers: ["/include/adv_api/hccl/hccl.h"]
```

```
__aicore__ inline uint32_t GetRankId()
```

## 参数说明

无

## 返回值说明

返回本卡RankId。

## 约束说明

无

## 调用示例

请参见[GetWindowsInAddr](GetWindowsInAddr.md)的调用示例。

