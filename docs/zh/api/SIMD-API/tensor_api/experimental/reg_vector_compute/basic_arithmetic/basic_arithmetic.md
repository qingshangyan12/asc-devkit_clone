# 基础算术

本节介绍基于`reg_tensor<T>`的基础算术、数学函数和激活函数接口。

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [abs](abs.md) | 逐元素计算绝对值。 |
| [exp](exp.md) | 逐元素计算自然指数。 |
| [sqrt](sqrt.md) | 逐元素计算平方根。 |
| [log](log.md) | 逐元素计算自然对数。 |
| [log2](log2.md) | 逐元素计算以2为底的对数。 |
| [log10](log10.md) | 逐元素计算以10为底的对数。 |
| [operator-（一元）](neg.md) | 逐元素取负。 |
| [relu](relu.md) | 逐元素执行ReLU。 |
| [prelu](prelu.md) | 逐元素执行PReLU。 |
| [leaky_relu](leaky_relu.md) | 逐元素执行Leaky ReLU。 |
| [operator+](operator+.md) | 逐元素执行不带进位输出的加法。 |
| [addc](addc.md) | 执行带进位输出加法，或带进位输入和输出加法。 |
| [operator-（二元）](operator-.md) | 逐元素执行不带借位输出的减法。 |
| [operator*](operator*.md) | 逐元素执行同位宽乘法。 |
| [mull](mull.md) | 计算32位整数宽乘的低位和高位结果。 |
| [operator/](div.md) | 逐元素执行除法。 |
| [max](max.md) | 逐元素取最大值。 |
| [min](min.md) | 逐元素取最小值。 |
