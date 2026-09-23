---
okftype: reference.type.cpp
title: asc_ndim_pad_count_config
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/union/asc_ndim_pad_count_config.md
description: 用于asc_set_ndim_pad_count接口设置asc_ndim_copy_gm2ub各维度左右侧padding元素个数的联合体。
tag:
  - api_surfaces: [c_api]
  - roles: [data_movement_configuration]
  - families: []
  - operations: []
  - objects: []
  - execution_models: []
  - source_locations: []
  - destination_locations: []
  - traits: [padding]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/vector_datamove/asc_ndim_copy_gm2ub.md
    kind: related
  - path: docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ndim_pad_count.md
    kind: related
examples: []
headers: [/include/c_api/defs/union.h]
declaration:
  symbol: asc_ndim_pad_count_config
  cpp: "union asc_ndim_pad_count_config {\n    uint64_t config = ASC_DEFAULT_NDIM_PAD_COUNT_CONFIG_VALUE;\n\
    \    struct {\n        uint8_t loop1_lp_count;\n        uint8_t loop1_rp_count;\n\
    \        uint8_t loop2_lp_count;\n        uint8_t loop2_rp_count;\n        uint8_t\
    \ loop3_lp_count;\n        uint8_t loop3_rp_count;\n        uint8_t loop4_lp_count;\n\
    \        uint8_t loop4_rp_count;\n    };\n};"
  fields:
    - name: loop1_lp_count
      description: 表示1维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]，默认值为0。
      type: uint8_t
    - name: loop1_rp_count
      description: 表示1维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]，默认值为0。
      type: uint8_t
    - name: loop2_lp_count
      description: 表示2维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]，默认值为0。
      type: uint8_t
    - name: loop2_rp_count
      description: 表示2维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]，默认值为0。
      type: uint8_t
    - name: loop3_lp_count
      description: 表示3维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]，默认值为0。
      type: uint8_t
    - name: loop3_rp_count
      description: 表示3维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]，默认值为0。
      type: uint8_t
    - name: loop4_lp_count
      description: 表示4维左侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]，默认值为0。
      type: uint8_t
    - name: loop4_rp_count
      description: 表示4维右侧需要补齐的元素个数。 单位为元素个数，取值范围为[0, 255]，默认值为0。
      type: uint8_t
  record_kind: union
timestamp: '2026-09-23'
---
# asc_ndim_pad_count_config

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
