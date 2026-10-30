# cast_sat_mode

## 功能说明

头文件路径：`"tensor_api/experimental/utils/reg_enum.h"`。

`cast_sat_mode`用于指定[cast](../type_conversion/cast.md)转换时是否采用饱和模式。

## 类型定义

```cpp
enum class cast_sat_mode {
    unknown = -1,
    not_sat,
    sat,
};
```

## 枚举值说明

| 枚举值 | 说明 |
| :-- | :-- |
| unknown | 当转换类型不支持饱和模式配置时使用的预留值。 |
| not_sat | 不饱和模式。超出范围时按目的格式截断，或保留浮点格式支持的特殊值。 |
| sat | 饱和模式。超出范围时将结果钳位到目的类型边界。 |

不同类型转换的具体行为请参考[cast的饱和与非饱和模式](../type_conversion/cast.md#饱和与非饱和模式)。
