---
okftype: reference.constant
description: "定义反量化缩放配置中scale、offset与sign模式三个数组的元素个数常量，值为16，用于asc_set_deq_scale接口的数组长度定义。"
tag:
  - roles: [configuration]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_set_deq_scale.md
    kind: related
version: 9.0.0-
---
# ASC_VDEQ_SIZE

ASC_VDEQ_SIZE表示[asc_set_deq_scale](../../vector_compute/vector_fused/asc_set_deq_scale.md)接口中scale_arr、offset_arr和sign_mode_arr数组的元素个数，值为16。

头文件路径：`c_api/defs/constant.h`。

## 常量定义

```cpp
constexpr uint8_t ASC_VDEQ_SIZE = 16;
```
