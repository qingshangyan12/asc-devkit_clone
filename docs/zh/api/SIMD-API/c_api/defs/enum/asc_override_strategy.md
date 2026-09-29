---
okftype: reference.type
title: asc_override_strategy
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/enum/asc_override_strategy.md
description: 该枚举表示饱和控制策略，用于设置或查询单指令/API路径与全局CAST饱和配置的生效方式。
tag:
  - api_surfaces: [c_api]
  - roles: [configuration]
  - traits: [saturation_mode]
declaration:
  symbol: asc_override_strategy
  underlying_type: uint8_t
header: [/include/c_api/defs/enum.h]
timestamp: '2026-09-28'
---
# asc_override_strategy

asc_override_strategy用于表示饱和控制策略，在调用[asc_set_saturation_strategy](../../spr/asc_set_saturation_strategy.md)和[asc_get_saturation_strategy](../../spr/asc_get_saturation_strategy.md)时使用。该策略影响CAST类全局饱和配置是否生效。

## 枚举类具体定义

```cpp
enum class asc_override_strategy : uint8_t {
    USE_API = 0,
    USE_GLOBAL = 1
};
```

## 枚举值详解

**表1** 枚举值说明

| 枚举值 | 说明 |
| ------ | ----------- |
| USE_API | 按单指令/API路径配置饱和。此时通过[asc_set_saturation_flag](../../spr/asc_set_saturation_flag.md)配置的全局`CAST`饱和不生效。 |
| USE_GLOBAL | 使用全局饱和配置。此时通过[asc_set_saturation_flag](../../spr/asc_set_saturation_flag.md)配置的`CAST`模式才会在精度转换中生效。 |

典型用法：`USE_GLOBAL`配合CAST饱和做float→int32动态量化，参见[asc_set_saturation_strategy](../../spr/asc_set_saturation_strategy.md)调用示例及[Cast样例](../../../../../../../examples/02_simd_c_api/03_c_api/02_reg_vector_compute/cast)；`USE_API`配合单指令饱和参见[Cast](../../../basic_api/reg_vector_compute/type_conversion/Cast.md)中`SatMode`与饱和模式全局或单指令生效配置表。
