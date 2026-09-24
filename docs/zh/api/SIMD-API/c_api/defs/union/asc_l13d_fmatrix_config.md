---
okftype: reference.type.cpp
title: asc_l13d_fmatrix_config
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/union/asc_l13d_fmatrix_config.md
description: 设置asc_copy_l12l0a、asc_copy_l12l0b的3D格式搬运接口Feature map属性参数的联合体。
tag:
  - api_surfaces: [c_api]
  - roles: [data_movement_configuration]
  - traits: [padding]
dependencies:
  - path: docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md
    kind: related
  - path: docs/zh/api/SIMD-API/c_api/cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b.md
    kind: related
examples: []
headers: [/include/c_api/defs/union.h]
declaration:
  symbol: asc_l13d_fmatrix_config
  cpp: "union asc_l13d_fmatrix_config {\n    uint64_t config = ASC_DEFAULT_L13D_FMATRIX_CONFIG_VALUE;\n\
    \    struct {\n        uint16_t l1_width;\n        uint16_t l1_height;\n     \
    \   uint8_t padding_left_size;\n        uint8_t padding_right_size;\n        uint8_t\
    \ padding_top_size;\n        uint8_t padding_bottom_size;\n    };\n};"
  fields:
    - name: l1_width
      description: Feature map的width，取值范围：[1, 32767]。
      type: uint16_t
    - name: l1_height
      description: Feature map的height，取值范围：[1, 32767]。
      type: uint16_t
    - name: padding_left_size
      description: 左侧填充的大小，取值范围：[0, 255]。
      type: uint8_t
    - name: padding_right_size
      description: 右侧填充的大小，取值范围：[0, 255]。
      type: uint8_t
    - name: padding_top_size
      description: 上侧填充的大小，取值范围：[0, 255]。
      type: uint8_t
    - name: padding_bottom_size
      description: 下侧填充的大小，取值范围：[0, 255]。
      type: uint8_t
  record_kind: union
timestamp: '2026-09-23'
---
# asc_l13d_fmatrix_config

asc_l13d_fmatrix_config用于设置[asc_copy_l12l0a](../../cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md)/[asc_copy_l12l0b](../../cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b.md)的3D格式搬运接口的Feature map属性参数。

## 结构体具体定义

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

## 约束说明

`asc_l13d_fmatrix_config`类型的联合体变量被定义后必须对`l1_height`和`l1_width`进行赋值以符合实际场景及取值范围要求。

## 字段详解

**表1** 字段说明

|字段名|字段含义|
|----------|----------|
| l1_width | Feature map的width，取值范围：[1, 32767]。 |
| l1_height | Feature map的height，取值范围：[1, 32767]。 |
| padding_left_size | 左侧填充的大小，取值范围：[0, 255]。 |
| padding_right_size | 右侧填充的大小，取值范围：[0, 255]。 |
| padding_top_size | 上侧填充的大小，取值范围：[0, 255]。 |
| padding_bottom_size | 下侧填充的大小，取值范围：[0, 255]。 |
