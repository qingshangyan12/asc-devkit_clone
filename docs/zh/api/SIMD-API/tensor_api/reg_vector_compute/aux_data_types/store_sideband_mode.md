# store_sideband_mode

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/reg_data_store.h"`。

`store_sideband_mode`用于指定[store](../reg_data_store/store.md)从寄存器搬出到UB时的数据排列方式。

## 类型定义

```cpp
enum class store_sideband_mode : uint8_t {
    direct,
    store_1st,
    intlv,
    pack,
    pack_quarter,
};
```

| 参数名 | 描述 |
| :-- | :-- |
| direct | 正常模式，根据源寄存器Mask搬出有效元素。 |
| store_1st | 仅搬出源寄存器的第一个元素。 |
| intlv | 交织模式，将两个源寄存器的元素交替搬出。仅用于双源重载。 |
| pack | 2倍压缩模式，搬出每个有效元素的低半部分bit。 |
| pack_quarter | 4倍压缩模式，搬出每个有效32bit元素的低8bit。 |
