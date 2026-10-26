# cast_round_mode

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/type_conversion.h"`。

`cast_round_mode`用于指定[cast](../type_conversion/cast.md)转换时的舍入方式。

## 类型定义

```cpp
enum class cast_round_mode {
    unknown = -1,
    rint,
    round,
    floor,
    ceil,
    trunc,
    odd,
    hybrid,
};
```

| 枚举值 | 描述 |
| --- | --- |
| `unknown` | 未指定舍入方式，预留值。 |
| `rint` | 舍入到最近值，正好位于中间值时舍入到偶数。 |
| `round` | 舍入到最近值，正好位于中间值时远离0。 |
| `floor` | 向负无穷方向舍入。 |
| `ceil` | 向正无穷方向舍入。 |
| `trunc` | 向0方向舍入。 |
| `odd`、`hybrid` | 预留模式，当前`cast`接口暂不支持。 |

具体类型组合支持的舍入方式请参考[cast的数据类型说明](../type_conversion/cast.md#数据类型)。

