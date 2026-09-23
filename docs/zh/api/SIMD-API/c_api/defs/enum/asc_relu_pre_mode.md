---
okftype: reference.type.cpp
title: asc_relu_pre_mode
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/enum/asc_relu_pre_mode.md
description: 表示L0C Buffer数据搬出时随路ReLU模式的枚举，作为矩阵搬出接口relu_pre_mode参数的类型。
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
  symbol: asc_relu_pre_mode
  cpp: "enum class asc_relu_pre_mode : uint8_t {\n    NONE = 0,\n    NORMAL = 1,\n\
    \    SCALAR = 2,\n    VECTOR = 3,\n};"
  members:
    - name: NONE
      value: '0'
      description: 不执行随路ReLU。
    - name: NORMAL
      value: '1'
      description: 执行普通ReLU。
    - name: SCALAR
      value: '2'
      description: 使用Scalar参数执行随路ReLU。
    - name: VECTOR
      value: '3'
      description: 使用Vector参数执行随路ReLU。
  underlying_type: uint8_t
timestamp: '2026-09-23'
---
# asc_relu_pre_mode

`asc_relu_pre_mode`用于表示L0C Buffer数据搬出时的随路ReLU模式，作为矩阵搬出接口中`relu_pre_mode`参数的类型。

头文件路径为：`"c_api/defs/enum.h"`。

## 枚举类具体定义

```cpp
enum class asc_relu_pre_mode : uint8_t {
    NONE = 0,
    NORMAL = 1,
    SCALAR = 2,
    VECTOR = 3,
};
```

## 枚举值详解

**表1** 枚举值说明

| 枚举值 | 取值 | 说明 |
| --- | --- | --- |
| NONE | 0 | 不执行随路ReLU。 |
| NORMAL | 1 | 执行普通ReLU。 |
| SCALAR | 2 | 使用Scalar参数执行随路ReLU。 |
| VECTOR | 3 | 使用Vector参数执行随路ReLU。 |
