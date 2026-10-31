# asc_l13d_fmatrix_config

<!-- md-trans-meta sourceCommit=462c9a712209ad24f2d57d4b5f5dd0d416050b06 translatedAt=2026-10-23T00:08:00.821Z pushedAt=2026-10-28T12:47:03.118Z nl=2 -->

asc_l13d_fmatrix_config is used to set the Feature map attribute parameters of the 3D format copy API of [asc_copy_l12l0a](../../cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md)/[asc_copy_l12l0b](../../cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b.md).

## Structure Definition

```cpp
constexpr uint64_t ASC_DEFAULT_L13D_FMATRIX_CONFIG_VALUE = 0;
union asc_l13d_fmatrix_config {
    uint64_t config = ASC_DEFAULT_L13D_FMATRIX_CONFIG_VALUE;
    struct {
        uint16_t l1_width;
        uint16_t l1_height;
        uint8_t padding_left_size;
        uint8_t padding_right_size;
        uint8_t padding_top_size;
        uint8_t padding_bottom_size;
    };
};
```

## Constraints

After a union variable of the `asc_l13d_fmatrix_config` type is defined, `l1_height` and `l1_width` must be assigned values that conform to the actual scenario and value range requirements.

## Field Description

**Table 1** Field description

|Field Name|Description|
|----------|----------|
| l1_width | Width of the feature map. Value range: [1, 32767]. |
| l1_height | Height of the feature map. Value range: [1, 32767]. |
| padding_left_size | Size of the left padding. Value range: [0, 255]. |
| padding_right_size | Size of the right padding. Value range: [0, 255]. |
| padding_top_size | Size of the top padding. Value range: [0, 255]. |
| padding_bottom_size | Size of the bottom padding. Value range: [0, 255]. |
