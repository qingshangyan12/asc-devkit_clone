# cast_layout

## 功能说明

头文件路径：`"tensor_api/experimental/utils/reg_enum.h"`。

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

## 枚举值说明

| 枚举值 | 说明 |
| :-- | :-- |
| unknown | 未指定数据排布，预留值，不用于当前`cast`接口。 |
| zero | 大转小时，转换结果位于目的操作数组内第0个小位宽位置；小转大时，选取源操作数组内第0个小位宽元素。 |
| one | 大转小时，转换结果位于目的操作数组内第1个小位宽位置；小转大时，选取源操作数组内第1个小位宽元素。 |
| two | 大转小时，转换结果位于目的操作数组内第2个小位宽位置；小转大时，选取源操作数组内第2个小位宽元素。 |
| three | 大转小时，转换结果位于目的操作数组内第3个小位宽位置；小转大时，选取源操作数组内第3个小位宽元素。 |

源操作数与目的操作数位宽比为1:2、2:1、1:4和4:1时的数据排布分别如下图所示。具体支持的`cast_layout`取值请参考[cast](../type_conversion/cast.md)。

![](../../../../figures/src_dst_bitwidth_1to2.png "源操作数与目的操作数位宽比为1:2")

![](../../../../figures/src_dst_bitwidth_2to1.png "源操作数与目的操作数位宽比为2:1")

![](../../../../figures/src_dst_bitwidth_1to4.png "源操作数与目的操作数位宽比为1:4")

![](../../../../figures/src_dst_bitwidth_4to1.png "源操作数与目的操作数位宽比为4:1")
