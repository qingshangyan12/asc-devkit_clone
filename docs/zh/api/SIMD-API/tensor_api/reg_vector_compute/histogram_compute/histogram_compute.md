# 直方图计算

本目录介绍 `asc::te` 命名空间下的寄存器直方图统计接口。

## 头文件

```cpp
#include "tensor_api/experimental/reg/histogram_compute.h"
```

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [histograms](Histograms.md) | 统计 `uint8_t` 输入的频率分布或累计分布。 |
