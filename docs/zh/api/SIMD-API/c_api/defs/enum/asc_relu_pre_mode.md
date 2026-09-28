---
okftype: reference.type.cpp
title: asc_relu_pre_mode
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/enum/asc_relu_pre_mode.md
description: 该枚举表示L0C Buffer数据搬出时的随路ReLU模式，并作为矩阵搬出接口relu_pre_mode参数的类型。
tag:
  - api_surfaces: [c_api]
  - roles: [data_movement_configuration]
declaration:
  symbol: asc_relu_pre_mode
  underlying_type: uint8_t
header: [/include/c_api/defs/enum.h]
timestamp: '2026-09-28'
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
