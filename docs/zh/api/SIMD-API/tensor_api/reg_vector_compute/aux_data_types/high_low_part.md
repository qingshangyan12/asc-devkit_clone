# high_low_part

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/reg_compute_sel.h"`。

`high_low_part`用于指定寄存器选择接口处理源寄存器的低半部分或高半部分。

## 类型定义

```cpp
enum class high_low_part : uint8_t {
    low,
    high,
};
```

| 参数名 | 描述 |
| :-- | :-- |
| low | 选择低半部分。 |
| high | 选择高半部分。 |
