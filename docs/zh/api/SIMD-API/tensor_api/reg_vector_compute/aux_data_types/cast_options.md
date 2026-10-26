# cast_options

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/type_conversion.h"`。

`cast_options`用于集中配置[cast](../type_conversion/cast.md)的数据排布、舍入和饱和模式。

## 类型定义

```cpp
struct cast_options {
    cast_layout layout = cast_layout::zero;
    cast_round_mode round = cast_round_mode::rint;
    cast_sat_mode sat = cast_sat_mode::not_sat;
};
```

| 参数名 | 描述 |
| :-- | :-- |
| layout | 数据排布，详情参见[cast_layout](cast_layout.md)。 |
| round | 舍入模式，详情参见[cast_round_mode](cast_round_mode.md)。 |
| sat | 饱和模式，详情参见[cast_sat_mode](cast_sat_mode.md)。 |

`cast_options`需要定义为编译期常量，并作为`cast`的模板参数传入。各类型组合支持的配置不同，具体请参考[cast](../type_conversion/cast.md)。
