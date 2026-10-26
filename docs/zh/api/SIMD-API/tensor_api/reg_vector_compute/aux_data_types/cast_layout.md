# cast_layout

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/type_conversion.h"`。

`cast_layout`用于指定[cast](../type_conversion/cast.md)执行位宽变化时，较小位宽元素在每组数据中的位置。

## 类型定义

```cpp
enum class cast_layout {
    unknown = -1,
    zero,
    one,
    two,
    three,
};
```

| 枚举值 | 描述 |
| --- | --- |
| `unknown` | 未指定数据排布，预留值，不用于当前`cast`接口。 |
| `zero`、`one`、`two`、`three` | 较小位宽元素位于同组元素中的第0、1、2、3个位置。位宽比为2时仅支持`zero`、`one`；位宽比为4时支持全部四种取值；同位宽转换仅支持`zero`。 |

