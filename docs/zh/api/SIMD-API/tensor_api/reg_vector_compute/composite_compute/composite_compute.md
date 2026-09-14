# 复合计算

本节介绍将多个计算步骤融合为一次调用的寄存器计算接口。

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [axpy](axpy.md) | 标量乘与累加融合。 |
| [abs_diff](abs_diff.md) | 减法与绝对值融合。 |
| [exp_diff](exp_diff.md) | 减法与自然指数融合。 |
| [madd](madd.md) | 目的操作数乘与加法融合。 |
| [mula](mula.md) | 两个源操作数相乘并累加到目的操作数。 |
| [fma](fma.md) | 融合乘加。 |
| [muls_cast](muls_cast.md) | 标量乘与`float`到`half`转换融合。 |
