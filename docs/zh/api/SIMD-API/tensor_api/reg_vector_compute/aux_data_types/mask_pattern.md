# mask_pattern

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/mask_reg_compute.h"`。

`mask_pattern`用于指定[make_mask](../mask_reg_compute/make_mask.md)创建Mask寄存器时的有效位模式。

## 类型定义

```cpp
enum class mask_pattern : uint8_t {
    all, vl1, vl2, vl3, vl4, vl8, vl16, vl32, vl64, vl128,
    every3, every4, half, quarter, none,
};
```

| 参数名 | 描述 |
| :-- | :-- |
| all | 所有元素设置为有效。 |
| vl1、vl2、vl3、vl4、vl8、vl16、vl32、vl64、vl128 | 最低指定数量的元素设置为有效。 |
| every3 | 索引为3的倍数的元素设置为有效。 |
| every4 | 索引为4的倍数的元素设置为有效。 |
| half | 最低一半元素设置为有效。 |
| quarter | 最低四分之一元素设置为有效。 |
| none | 所有元素设置为无效。 |
