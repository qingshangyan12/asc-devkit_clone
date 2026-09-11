---
okftype: reference.api.cpp
title: GetSortOffset
resource: asc-devkit/docs/zh/api/SIMD-API/adv_api/sort_operations/GetSortOffset.md
description: 根据元素位置，获取Sort数据中的对应偏移量（单位为字节）。
tag: [adv_api]
usage_dependencies:
  status: unconfirmed
signature:
- override_id: GetSortOffset__u32
  archs: []
  headers: [/include/basic_api/kernel_operator_proposal_intf.h]
examples: []
timestamp: '2026-09-11'
---

# GetSortOffset

## 功能说明

根据元素位置，获取Sort数据中的对应偏移量（单位为字节）。

## 函数原型

```yaml
api_signature:
  override_id: "GetSortOffset__u32"
  archs: []
  headers: ["/include/basic_api/kernel_operator_proposal_intf.h"]
```

```
template <typename T>
__aicore__ inline uint32_t GetSortOffset(const uint32_t elemOffset)
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数的数据类型。支持的数据类型为：half、float。 |

**表2**  接口参数列表

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| elemOffset | 输入 | 元素的位置。 |

## 返回值说明

Sort数据中的对应偏移量。

## 约束说明

无

## 调用示例

```
AscendC::GetSortOffset<half>(128);
```
