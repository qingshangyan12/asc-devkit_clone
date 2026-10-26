# broadcast_mode

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/reg_data_load.h"`。

`broadcast_mode`用于指定[load_broadcast](../reg_data_load/load_broadcast.md)从UB搬入并广播数据的方式。

## 类型定义

```cpp
enum class broadcast_mode : uint8_t {
    elem,
    datablock,
    elem2datablock,
};
```

| 参数名 | 描述 |
| :-- | :-- |
| elem | 读取一个元素并广播到目的寄存器的所有元素位置。 |
| datablock | 读取一个DataBlock（32B）并重复广播到整个目的寄存器。 |
| elem2datablock | 依次读取多个元素，每个元素分别广播到目的寄存器的一个DataBlock。 |
