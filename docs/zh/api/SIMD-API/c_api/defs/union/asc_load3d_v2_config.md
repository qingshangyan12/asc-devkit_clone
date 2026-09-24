---
okftype: reference.type.cpp
title: asc_load3d_v2_config
resource: asc-devkit/docs/zh/api/SIMD-API/c_api/defs/union/asc_load3d_v2_config.md
description: 设置Load3Dv2接口repeat参数的联合体，包含迭代步长、迭代次数与迭代方向控制字段。
tag:
  - api_surfaces: [c_api]
  - roles: [configuration]
dependencies: []
examples: []
headers: [/include/c_api/defs/union.h]
declaration:
  symbol: asc_load3d_v2_config
  cpp: "union asc_load3d_v2_config {\n    uint64_t config = ASC_DEFAULT_LOAD3D_V2_CONFIG_VALUE;\n\
    \    struct {\n        uint64_t rpt_stride : 16;\n        uint64_t rpt_time :\
    \ 8;\n        uint64_t rpt_mode : 1;\n        uint64_t reserved2 : 39;\n    };\n\
    };"
  fields:
    - name: rpt_stride
      description: height/width方向上的前一个迭代与后一个迭代起始地址的距离，取值范围：[0, 65535]，默认值为0。
      type: uint64_t
      bits: '16'
    - name: rpt_time
      description: height/width方向上的迭代次数，取值范围：[0, 255]，默认值为1。
      type: uint64_t
      bits: '8'
    - name: rpt_mode
      description: 控制repeat迭代的方向，取值范围：[0, 1]，默认值为0。0：迭代沿height方向；1：迭代沿width方向。
      type: uint64_t
      bits: '1'
  record_kind: union
timestamp: '2026-09-23'
---
# asc_load3d_v2_config

asc_load3d_v2_config用于设置Load3Dv2接口的repeat参数。

## 结构体具体定义

```cpp
constexpr uint64_t ASC_DEFAULT_LOAD3D_V2_CONFIG_VALUE = 0x0000000000010000;
union asc_load3d_v2_config {
    uint64_t config = ASC_DEFAULT_LOAD3D_V2_CONFIG_VALUE;
    struct {
        uint64_t rpt_stride : 16;
        uint64_t rpt_time : 8;
        uint64_t rpt_mode : 1;
        uint64_t reserved2 : 39;
    };
};
```

## 字段详解

**表1** 字段说明

|字段名|字段含义|
|----------|----------|
| rpt_stride | height/width方向上的前一个迭代与后一个迭代起始地址的距离，取值范围：[0, 65535]，默认值为0。 |
| rpt_time | height/width方向上的迭代次数，取值范围：[0, 255]，默认值为1。 |
| rpt_mode | 控制repeat迭代的方向，取值范围：[0, 1]，默认值为0。<br>0：迭代沿height方向；<br>1：迭代沿width方向。 |
