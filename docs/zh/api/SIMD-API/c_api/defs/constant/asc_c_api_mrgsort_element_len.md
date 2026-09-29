---
okftype: reference.type
title: ASC_C_API_MRGSORT_ELEMENT_LEN
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/constant/asc_c_api_mrgsort_element_len.md
description: 定义MrgSort4操作支持的队列数量常量，值为4，并用于相关排序接口的数组长度约束。
tag:
  - api_surfaces: [c_api]
  - roles: [configuration]
declaration:
  symbol: ASC_C_API_MRGSORT_ELEMENT_LEN
  value: '4'
  type: uint8_t
header: [/include/c_api/defs/constant.h]
timestamp: '2026-09-28'
---
# ASC_C_API_MRGSORT_ELEMENT_LEN

ASC_C_API_MRGSORT_ELEMENT_LEN表示MrgSort4操作支持的队列数量，值为4。该常量用于[asc_mrgsort4](../../vector_compute/vector_sort/asc_mrgsort4.md)接口的src数组长度，以及[asc_get_vms4_sr](../../vector_compute/vector_sort/asc_get_vms4_sr.md)接口的sorted_num数组长度。

头文件路径：`c_api/defs/constant.h`。

## 常量定义

```cpp
constexpr uint8_t ASC_C_API_MRGSORT_ELEMENT_LEN = 4;
```
