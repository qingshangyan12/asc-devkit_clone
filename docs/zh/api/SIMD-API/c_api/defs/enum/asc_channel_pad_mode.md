# asc_channel_pad_mode

asc_channel_pad_mode用于表示从GM搬运到L1 Buffer时的通道padding模式，可用于[asc_copy_gm2l1](../../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_highdim_split_arch_3510.md)高维切分数据搬运接口。

## 枚举类具体定义

```cpp
enum class asc_channel_pad_mode : uint8_t {
    NONE = 0,
    EXPAND_1B = 1,
    EXPAND_2B = 2,
    EXPAND_4B = 3,
    EXPAND_8B = 4,
    EXPAND_16B = 5,
    COMPACT_4B = 6,
    COMPACT_8B = 7,
    COMPACT_16B = 8
};
```

## 枚举值详解

**表1** 枚举值说明

|枚举值|通道padding模式|
|------|-----------|
| NONE | 不进行padding，每个32字节通道块保持不变。 |
| EXPAND_1B | 每读取1字节源数据，插入31个padding值。 |
| EXPAND_2B | 每读取2字节源数据，插入15个padding值。 |
| EXPAND_4B | 每读取4字节源数据，插入14个padding值。 |
| EXPAND_8B | 每读取8字节源数据，插入12个padding值。 |
| EXPAND_16B | 每读取16字节源数据，插入8个padding值。 |
| COMPACT_4B | 每个32字节源数据块丢弃高28字节，保留低4字节。 |
| COMPACT_8B | 每个32字节源数据块丢弃高24字节，保留低8字节。 |
| COMPACT_16B | 每个32字节源数据块丢弃高16字节，保留低16字节。 |
