---
okftype: reference.api.cpp
title: GetOffsetC
resource: asc-devkit/docs/zh/api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/GetOffsetC.md
description: 预留接口，为后续功能做预留。
tag: [adv_api]
usage_dependencies:
  status: unconfirmed
signature:
- override_id: GetOffsetC__z
  archs: []
  headers: [/include/adv_api/matmul/matmul.h]
examples: []
timestamp: '2026-09-11'
---

# GetOffsetC

## 功能说明

预留接口，为后续功能做预留。

获取本次计算时当前分片在整个C矩阵中的位置。

## 函数原型

```yaml
api_signature:
  override_id: "GetOffsetC__z"
  archs: []
  headers: ["/include/adv_api/matmul/matmul.h"]
```

```
__aicore__ inline MatrixOffset GetOffsetC()
```

## 参数说明

无

## 返回值说明

MatrixOffset结构体如下：

```
struct MatrixOffset {
    int32_t offset;
    int32_t row, col;
    int32_t height, width;
};
```

## 约束说明

无

