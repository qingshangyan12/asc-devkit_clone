# merge_mode

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/data_padding.h"`。

`merge_mode`用于指定带Mask的寄存器写入操作中，目的寄存器未被Mask选中的元素如何处理。

## 类型定义

```cpp
enum class merge_mode {
    zeroing,
    merging,
};
```

## 枚举值说明

| 枚举值 | 描述 |
| --- | --- |
| `merge_mode::zeroing` | Mask未选中的元素在目的寄存器中置0。 |
| `merge_mode::merging` | Mask未选中的元素保留目的寄存器中的原值。 |

## 使用说明

`merge_mode`作为模板参数使用。以[fill](../data_padding/fill.md)接口为例，默认采用`merge_mode::zeroing`；需要保留目的寄存器中Mask未选中的元素时，按如下方式指定`merge_mode::merging`：

```cpp
dst = asc::te::experimental::fill<asc::te::experimental::merge_mode::merging>(dst, value, mask);
```

