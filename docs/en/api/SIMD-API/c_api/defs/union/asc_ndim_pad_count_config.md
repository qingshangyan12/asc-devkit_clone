# asc_ndim_pad_count_config

<!-- md-trans-meta sourceCommit=462c9a712209ad24f2d57d4b5f5dd0d416050b06 translatedAt=2026-10-23T00:08:41.641Z pushedAt=2026-10-28T12:47:03.119Z nl=2 -->

asc_ndim_pad_count_config is used in the [asc_set_ndim_pad_count](../../vector_datamove/asc_set_ndim_pad_count.md) API to set the number of padding elements on the left and right sides of each dimension of the [asc_ndim_copy_gm2ub](../../vector_datamove/asc_ndim_copy_gm2ub.md) API.

## Struct Definition

```cpp
constexpr uint64_t ASC_DEFAULT_NDIM_PAD_COUNT_CONFIG_VALUE = 0;
union asc_ndim_pad_count_config {
    uint64_t config = ASC_DEFAULT_NDIM_PAD_COUNT_CONFIG_VALUE;
    struct {
        uint8_t loop1_lp_count;
        uint8_t loop1_rp_count;
        uint8_t loop2_lp_count;
        uint8_t loop2_rp_count;
        uint8_t loop3_lp_count;
        uint8_t loop3_rp_count;
        uint8_t loop4_lp_count;
        uint8_t loop4_rp_count;
    };
};
```

## Field Description

**Table 1** Field description

|Field Name|Description|
|----------|----------|
| loop1_lp_count | Indicates the number of elements to be padded on the left side of dimension 1.<br> Unit: number of elements. Value range: [0, 255]. Default value is 0. |
| loop1_rp_count | Indicates the number of elements to be padded on the right side of dimension 1.<br> Unit: number of elements. Value range: [0, 255]. Default value is 0. |
| loop2_lp_count | Indicates the number of elements to be padded on the left side of dimension 2.<br> Unit: number of elements. Value range: [0, 255]. Default value is 0. |
| loop2_rp_count | Indicates the number of elements to be padded on the right side of dimension 2.<br> Unit: number of elements. Value range: [0, 255]. Default value is 0. |
| loop3_lp_count | Indicates the number of elements to be padded on the left side of dimension 3.<br> Unit: number of elements. Value range: [0, 255]. Default value is 0. |
| loop3_rp_count | Indicates the number of elements to be padded on the right side of dimension 3.<br> Unit: number of elements. Value range: [0, 255]. Default value is 0. |
| loop4_lp_count | Indicates the number of elements to be padded on the left side of dimension 4.<br> Unit: number of elements. Value range: [0, 255]. Default value is 0. |
| loop4_rp_count | Indicates the number of elements to be padded on the right side of dimension 4.<br> Unit: number of elements. Value range: [0, 255]. Default value is 0. |
