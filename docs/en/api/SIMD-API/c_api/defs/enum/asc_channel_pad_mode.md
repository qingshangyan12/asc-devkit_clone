# asc_channel_pad_mode

<!-- md-trans-meta sourceCommit=b80bcfa78036291b1d4471b8067ef0816732a63c translatedAt=2026-10-23T00:01:26.950Z pushedAt=2026-10-28T12:47:03.090Z nl=2 -->

The header file path is `"c_api/defs/enum.h"`.

asc_channel_pad_mode represents the padding or compaction mode for each 32-byte channel block when data is moved from GM to the L1 Buffer. It can be used with the [asc_copy_gm2l1](../../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_highdim_split_arch_3510.md) high-dimensional split data movement API. This mode is a typed definition of the hardware `pad_func_mode`, applies to each 32-byte channel data block, and takes a value in the range [0, 8].

`EXPAND_*` is the padding mode, which inserts padding values into the destination data. The channel padding value is set through [asc_set_gm2l1_padding](../../cube_datamove/asc_set_gm2l1_padding.md).

`COMPACT_*` is the removal mode, which removes the high-order portion of data from each 32-byte source data block and retains only the low-order valid region.

This enum controls padding or removal within a channel block, which differs from the left and right padding configured through [asc_set_gm2l1_pad](../../cube_datamove/asc_set_gm2l1_pad.md).

## Specific Definition of the Enum Class

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

## Enum Value Details

**Table 1** Enum value description

| Enum Value | Value | Description |
| --- | --- | --- |
| NONE | 0 | No channel padding or compaction is performed. |
| EXPAND_1B | 1 | Inserts 31 padding bytes after each 1-byte source data, where the bit width of each inserted padding is b8. |
| EXPAND_2B | 2 | Inserts 15 padding values after each 2-byte source data, where the bit width of each inserted padding is b16. |
| EXPAND_4B | 3 | Inserts 14 padding values after each 4-byte source data, where the bit width of each inserted padding is b16. |
| EXPAND_8B | 4 | Inserts 12 padding values after each 8-byte source data, where the bit width of each inserted padding is b16. |
| EXPAND_16B | 5 | Inserts 8 padding values after each 16-byte source data, where the bit width of each inserted padding is b16. |
| COMPACT_4B | 6 | Retains the low 4 bytes of each 32-byte source data and discards the high 28 bytes, retaining only the 4 least significant bytes. |
| COMPACT_8B | 7 | Retains the low 8 bytes of each 32-byte source data and discards the high 24 bytes, retaining only the 8 least significant bytes. |
| COMPACT_16B | 8 | Retains the low 16 bytes of each 32-byte source data and discards the high 16 bytes, retaining only the 16 least significant bytes. |
