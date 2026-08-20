# 直方图计算

本目录介绍 `asc::te` 命名空间下的寄存器直方图统计接口。

## 头文件

```cpp
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/histogram_compute.h"
```

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [histograms](Histograms.md) | 统计 `uint8_t` 输入的频率分布或累计分布。 |

## 通用说明

- 接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用。
- `src.mask` 决定参与统计的输入 lane；返回对象继承 `dst.mask`，用于约束后续计算和存储。
