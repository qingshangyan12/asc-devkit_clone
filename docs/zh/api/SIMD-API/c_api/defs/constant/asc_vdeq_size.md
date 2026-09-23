---
okftype: reference.type.cpp
title: ASC_VDEQ_SIZE
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/constant/asc_vdeq_size.md
description: 定义asc_set_deq_scale接口中scale_arr、offset_arr和sign_mode_arr数组元素个数的常量，值为16。
tag:
  - api_surfaces: [c_api]
  - roles: [configuration]
  - families: []
  - operations: []
  - objects: []
  - execution_models: []
  - source_locations: []
  - destination_locations: []
  - traits: []
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_set_deq_scale.md
    kind: related
examples: []
headers: [/include/c_api/defs/constant.h]
declaration:
  symbol: ASC_VDEQ_SIZE
  cpp: constexpr uint8_t ASC_VDEQ_SIZE = 16;
  value: '16'
  type: uint8_t
timestamp: '2026-09-23'
---
# ASC_VDEQ_SIZE

ASC_VDEQ_SIZE表示[asc_set_deq_scale](../../vector_compute/vector_fused/asc_set_deq_scale.md)接口中scale_arr、offset_arr和sign_mode_arr数组的元素个数，值为16。

头文件路径：`c_api/defs/constant.h`。

## 常量定义

```cpp
constexpr uint8_t ASC_VDEQ_SIZE = 16;
```
