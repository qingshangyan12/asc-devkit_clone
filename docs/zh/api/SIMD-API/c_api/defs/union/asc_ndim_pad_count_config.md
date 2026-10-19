---
okftype: reference.union
description: "用于asc_set_ndim_pad_count接口中，为asc_ndim_copy_gm2ub接口配置loop1至loop4各维左右两侧需要补齐的padding元素个数，各字段取值范围为0到255，默认值为0。"
tag:
  - roles: [data_movement_configuration]
  - traits: [high_dimensional, padding]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/vector_datamove/asc_ndim_copy_gm2ub.md
    kind: configures
    condition: "配置该接口各维度左右侧的padding元素个数"
  - path: docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ndim_pad_count.md
    kind: configures
    condition: "作为该接口配置的填充元素数结构体"
version: ">=9.0.0"
---
# asc_ndim_pad_count_config

头文件路径为：`"c_api/defs/union.h"`。

asc_ndim_pad_count_config用于[asc_set_ndim_pad_count](../../vector_datamove/asc_set_ndim_pad_count.md)接口中，设置[asc_ndim_copy_gm2ub](../../vector_datamove/asc_ndim_copy_gm2ub.md)接口的各个维度左右侧的padding元素个数。

## 结构体具体定义

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

## 字段详解

**表1** 字段说明

|字段名|字段含义|
|----------|----------|
| loop1_lp_count | 表示1维左侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]，默认值为0。 |
| loop1_rp_count | 表示1维右侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]，默认值为0。 |
| loop2_lp_count | 表示2维左侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]，默认值为0。 |
| loop2_rp_count | 表示2维右侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]，默认值为0。 |
| loop3_lp_count | 表示3维左侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]，默认值为0。 |
| loop3_rp_count | 表示3维右侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]，默认值为0。 |
| loop4_lp_count | 表示4维左侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]，默认值为0。 |
| loop4_rp_count | 表示4维右侧需要补齐的元素个数。<br> 单位为元素个数，取值范围为[0, 255]，默认值为0。 |
