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

| 枚举值 | 描述 |
| --- | --- |
| `all`、`none` | 分别将所有元素设置为有效、无效。 |
| `vl1`、`vl2`、`vl3`、`vl4`、`vl8`、`vl16`、`vl32`、`vl64`、`vl128` | 将最低指定数量的元素设置为有效。 |
| `every3`、`every4` | 分别将索引为3或4的倍数的元素设置为有效。 |
| `half`、`quarter` | 分别将最低二分之一或四分之一的元素设置为有效。 |

