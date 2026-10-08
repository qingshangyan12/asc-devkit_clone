---
okftype: reference.constant
description: "定义MrgSort4操作支持的队列数量常量，值为4，作为asc_mrgsort4接口src数组与asc_get_vms4_sr接口sorted_num数组的长度。"
tag:
  - roles: [configuration]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/vector_compute/vector_sort/asc_get_vms4_sr.md
    kind: related
  - path: docs/zh/api/SIMD-API/c_api/vector_compute/vector_sort/asc_mrgsort4.md
    kind: related
version: 9.0.0-
---
# ASC_C_API_MRGSORT_ELEMENT_LEN

ASC_C_API_MRGSORT_ELEMENT_LEN表示MrgSort4操作支持的队列数量，值为4。该常量用于[asc_mrgsort4](../../vector_compute/vector_sort/asc_mrgsort4.md)接口的src数组长度，以及[asc_get_vms4_sr](../../vector_compute/vector_sort/asc_get_vms4_sr.md)接口的sorted_num数组长度。

头文件路径：`c_api/defs/constant.h`。

## 常量定义

```cpp
constexpr uint8_t ASC_C_API_MRGSORT_ELEMENT_LEN = 4;
```
