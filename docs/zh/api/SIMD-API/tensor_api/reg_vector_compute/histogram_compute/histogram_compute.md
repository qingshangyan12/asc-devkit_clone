# 直方图计算

本目录介绍 `asc::te::experimental` 命名空间下的寄存器直方图统计接口。

## 头文件

```cpp
#include "tensor_api/tensor.h"
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

- 接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
`__simd_vf__` 函数直接或间接调用。
- `src.mask` 决定参与统计的输入 lane；返回对象继承 `dst.mask`，用于约束后续计算和存储。
