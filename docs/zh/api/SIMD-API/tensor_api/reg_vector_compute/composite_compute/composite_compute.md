# 复合计算

本节介绍将多个计算步骤融合为一次调用的寄存器计算接口。

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [axpy](axpy.md) | 标量乘与累加融合。 |
| [abs_diff](abs_diff.md) | 减法与绝对值融合。 |
| [exp_sub](exp_sub.md) | 减法与自然指数融合。 |
| [fma](fma.md) | 融合乘加。 |
| [muls_cast](muls_cast.md) | 标量乘与`float`到`half`转换融合。 |

各接口的mask规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
