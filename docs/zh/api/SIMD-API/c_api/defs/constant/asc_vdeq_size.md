---
okftype: reference.type
title: ASC_VDEQ_SIZE
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/constant/asc_vdeq_size.md
description: 定义反量化缩放接口数组的元素个数常量，规定scale、offset和sign模式数组长度为16。
tag:
  - api_surfaces: [c_api]
  - roles: [configuration]
declaration:
  symbol: ASC_VDEQ_SIZE
  value: '16'
  type: uint8_t
header: [/include/c_api/defs/constant.h]
timestamp: '2026-09-28'
---
# ASC_VDEQ_SIZE

ASC_VDEQ_SIZE表示[asc_set_deq_scale](../../vector_compute/vector_fused/asc_set_deq_scale.md)接口中scale_arr、offset_arr和sign_mode_arr数组的元素个数，值为16。

头文件路径：`c_api/defs/constant.h`。

## 常量定义

```cpp
constexpr uint8_t ASC_VDEQ_SIZE = 16;
```
