---
okftype: reference.api.cpp
title: SetGroupName
resource: asc-devkit/docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_Tiling/SetGroupName.md
description: 设置通信任务所在的通信域。
tag: [adv_api]
usage_dependencies:
  status: unconfirmed
signature: []
examples: []
timestamp: '2026-09-11'
---

# SetGroupName

## 功能说明

设置通信任务所在的通信域。

## 函数原型

```
uint32_t SetGroupName(const std::string& groupName)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| groupName | 输入 | 当前通信任务所在的通信域。string类型，支持的最大长度为128字节。 |

## 返回值说明

-   0表示设置成功。
-   非0表示设置失败。

## 约束说明

无

## 调用示例

本接口的调用示例请见[调用示例](SetOpType.md#调用示例)。
