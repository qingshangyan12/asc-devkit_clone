---
okftype: reference.type.cpp
title: asc_l13d_repeat_direction
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/enum/asc_l13d_repeat_direction.md
description: 表示3D img2col搬运repeat迭代方向的枚举，作为asc_set_l13d_rpt和asc_set_l13d_rpt_b接口repeat_direction参数的类型。
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
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/cube_datamove/asc_set_l13d_rpt.md
    kind: related
  - path: docs/zh/api/SIMD-API/c_api/cube_datamove/asc_set_l13d_rpt_b.md
    kind: related
examples: []
headers: [/include/c_api/defs/enum.h]
declaration:
  symbol: asc_l13d_repeat_direction
  cpp: "enum class asc_l13d_repeat_direction : uint8_t {\n    M_DIRECTION = 0,\n \
    \   K_DIRECTION = 1\n};"
  members:
    - name: M_DIRECTION
      value: '0'
      description: 16个元素。
    - name: K_DIRECTION
      value: '1'
      description: 32 / sizeof(data_type)`个元素。其中，`data_type`为3D搬运接口源操作数的数据类型。
  underlying_type: uint8_t
timestamp: '2026-09-23'
---
# asc_l13d_repeat_direction

`asc_l13d_repeat_direction`用于表示3D img2col搬运的repeat迭代方向，作为[asc_set_l13d_rpt](../../cube_datamove/asc_set_l13d_rpt.md)和[asc_set_l13d_rpt_b](../../cube_datamove/asc_set_l13d_rpt_b.md)接口的`repeat_direction`参数类型。

头文件路径为：`"c_api/defs/enum.h"`。

## 枚举类具体定义

```cpp
enum class asc_l13d_repeat_direction : uint8_t {
    M_DIRECTION = 0,
    K_DIRECTION = 1
};
```

## 枚举值详解

**表1** 枚举值说明

| 枚举值 | 取值 | repeat迭代方向 | `repeat_stride`单位 |
| --- | --- | --- | --- |
| M_DIRECTION | 0 | 沿height（M）方向迭代。 | 16个元素。 |
| K_DIRECTION | 1 | 沿width（K）方向迭代。 | `32 / sizeof(data_type)`个元素。其中，`data_type`为3D搬运接口源操作数的数据类型。 |
