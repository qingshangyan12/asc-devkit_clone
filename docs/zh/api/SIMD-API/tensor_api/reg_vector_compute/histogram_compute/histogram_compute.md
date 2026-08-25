# 直方图计算

本节介绍寄存器直方图统计接口。

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [histograms](histograms.md) | 统计`uint8_t`输入的频率分布或累计分布。 |

输入数据的有效范围由`src.mask`决定，返回对象继承`dst.mask`。通用mask规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
