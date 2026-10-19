---
okftype: reference.enum
description: "定义饱和控制策略枚举，供asc_set_saturation_strategy和asc_get_saturation_strategy使用，决定CAST类全局饱和配置是否生效，取值USE_API或USE_GLOBAL。"
tag:
  - roles: [configuration]
  - traits: [saturation_mode]
dependencies:
  - path: docs/zh/api/SIMD-API/basic_api/reg_vector_compute/type_conversion/Cast.md
    kind: related
    condition: "USE_API配合单指令饱和时参见其SatMode与饱和生效配置表"
  - path: docs/zh/api/SIMD-API/c_api/spr/asc_get_saturation_strategy.md
    kind: related
    condition: "作为调用该接口时的策略枚举类型"
  - path: docs/zh/api/SIMD-API/c_api/spr/asc_set_saturation_flag.md
    kind: related
    condition: "该策略影响其配置的CAST全局饱和是否生效"
  - path: docs/zh/api/SIMD-API/c_api/spr/asc_set_saturation_strategy.md
    kind: configures
    condition: "作为调用该接口时传入的策略枚举"
version: ">=9.0.0"
---
# asc_override_strategy

asc_override_strategy用于表示饱和控制策略，在调用[asc_set_saturation_strategy](../../spr/asc_set_saturation_strategy.md)和[asc_get_saturation_strategy](../../spr/asc_get_saturation_strategy.md)时使用。该策略影响CAST类全局饱和配置是否生效。

头文件路径为：`"c_api/defs/enum.h"`。

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
