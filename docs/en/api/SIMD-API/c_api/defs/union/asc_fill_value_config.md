# asc_fill_value_config

<!-- md-trans-meta sourceCommit=462c9a712209ad24f2d57d4b5f5dd0d416050b06 translatedAt=2026-10-23T00:07:32.506Z pushedAt=2026-10-28T12:47:03.115Z nl=2 -->

asc_fill_value_config is the initialization parameter structure for fill_value, containing the various initialization parameters that the [asc_fill_l0a](../../cube_datamove/asc_fill_l0a.md)/[asc_fill_l0b](../../cube_datamove/asc_fill_l0b.md)/[asc_fill_l1](../../cube_datamove/asc_fill_l1.md) APIs require.

## Structure Definition

```cpp
constexpr uint64_t ASC_FILL_VALUE_DEFAULT_VALUE = 0x0000000000000000;
union asc_fill_value_config {
    uint64_t config = ASC_FILL_VALUE_DEFAULT_VALUE;
    struct {
        uint64_t repeat : 15;
        uint64_t reserved1 : 1;
        uint64_t blk_num : 15;
        uint64_t reserved2 : 1;
        uint64_t dst_gap : 15;
        uint64_t reserved3 : 17;
    };
};
```

## Field Details

**Table 1** Field description

|Field Name|Description|
|----------|----------|
| repeat | Number of iterations. Default value: 0. Value Range: [0, 32767]. |
| blk_num | Number of data blocks initialized per iteration. Default value: 0. Value Range: [0, 32767].<br>When the target operand is in the L1 Buffer, the data block size is 32B. When the target operand is in the L0A Buffer/L0B Buffer, the data block size is 512B. |
| dst_gap | Distance between the end address of the previous iteration and the start address of the next iteration of the target operand. Default value: 0. Value Range: [0, 32767].<br>When the target operand is in the L1 Buffer, the unit is 32B. When the target operand is in the L0A Buffer/L0B Buffer, the unit is 512B. |