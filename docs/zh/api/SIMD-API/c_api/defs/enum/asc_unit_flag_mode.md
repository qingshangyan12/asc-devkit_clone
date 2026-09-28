---
okftype: reference.type.cpp
title: asc_unit_flag_mode
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/enum/asc_unit_flag_mode.md
description: 该枚举用于配置矩阵计算或矩阵搬出接口执行后的UnitFlag行为，作为相关接口unit_flag_mode参数的类型。
tag:
  - api_surfaces: [c_api]
  - roles: [configuration]
declaration:
  symbol: asc_unit_flag_mode
  underlying_type: uint8_t
header: [/include/c_api/defs/enum.h]
timestamp: '2026-09-28'
---
# asc_unit_flag_mode

`asc_unit_flag_mode`用于配置矩阵计算或矩阵搬出接口执行后的UnitFlag行为，作为相关接口中`unit_flag_mode`参数的类型。

头文件路径为：`"c_api/defs/enum.h"`。

## 枚举类具体定义

```cpp
enum class asc_unit_flag_mode : uint8_t {
    DISABLE = 0,
    ENABLE_KEEP = 2,
    ENABLE_UPDATE = 3
};
```

## 枚举值详解

**表1** 枚举值说明

| 枚举值 | 取值 | 说明 |
| --- | --- | --- |
| DISABLE | 0 | 不使能UnitFlag。 |
| ENABLE_KEEP | 2 | 使能UnitFlag，硬件执行完指令后不改变单元标志位。 |
| ENABLE_UPDATE | 3 | 使能UnitFlag，硬件执行完指令后改变单元标志位。 |
