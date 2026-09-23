---
okftype: reference.type.cpp
title: asc_dual_dst_mode
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/enum/asc_dual_dst_mode.md
description: 表示L0C Buffer数据搬出时双目标拆分模式的枚举，作为矩阵搬出接口dual_dst_ctrl参数的类型。
tag:
  - api_surfaces: [c_api]
  - roles: [data_movement_configuration]
  - families: []
  - operations: []
  - objects: []
  - execution_models: []
  - source_locations: []
  - destination_locations: []
  - traits: []
dependencies: []
examples: []
headers: [/include/c_api/defs/enum.h]
declaration:
  symbol: asc_dual_dst_mode
  cpp: "enum class asc_dual_dst_mode : uint8_t {\n    DUAL_DST_DISABLE = 0,\n    DUAL_DST_SPLIT_M\
    \ = 1,\n    DUAL_DST_SPLIT_N = 2\n};"
  members:
    - name: DUAL_DST_DISABLE
      value: '0'
      description: 关闭双目标模式。
    - name: DUAL_DST_SPLIT_M
      value: '1'
      description: 开启双目标模式，沿M方向拆分数据。
    - name: DUAL_DST_SPLIT_N
      value: '2'
      description: 开启双目标模式，沿N方向拆分数据。
  underlying_type: uint8_t
timestamp: '2026-09-23'
---
# asc_dual_dst_mode

`asc_dual_dst_mode`用于表示L0C Buffer数据搬出时的双目标拆分模式，作为矩阵搬出接口中`dual_dst_ctrl`参数的类型。

头文件路径为：`"c_api/defs/enum.h"`。

## 枚举类具体定义

```cpp
enum class asc_dual_dst_mode : uint8_t {
    DUAL_DST_DISABLE = 0,
    DUAL_DST_SPLIT_M = 1,
    DUAL_DST_SPLIT_N = 2
};
```

## 枚举值详解

**表1** 枚举值说明

| 枚举值 | 取值 | 说明 |
| --- | --- | --- |
| DUAL_DST_DISABLE | 0 | 关闭双目标模式。 |
| DUAL_DST_SPLIT_M | 1 | 开启双目标模式，沿M方向拆分数据。 |
| DUAL_DST_SPLIT_N | 2 | 开启双目标模式，沿N方向拆分数据。 |
