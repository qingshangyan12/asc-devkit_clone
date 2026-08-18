# 基础算术

本目录介绍 `asc::te` 命名空间下基于 `reg_tensor<T>` 的基础算术、数学函数和激活函数接口。

## 头文件

```cpp
#include "tensor_api/experimental/reg/basic_arithmetic.h"
```

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 接口列表

| 接口 | 功能 |
| --- | --- |
| [abs](Abs.md) | 逐元素计算绝对值。 |
| [exp](Exp.md) | 逐元素计算自然指数。 |
| [sqrt](Sqrt.md) | 逐元素计算平方根。 |
| [log](Log.md) | 逐元素计算自然对数。 |
| [log2](Log2.md) | 逐元素计算以 2 为底的对数。 |
| [log10](Log10.md) | 逐元素计算以 10 为底的对数。 |
| [operator-（一元）](Neg.md) | 逐元素取负。 |
| [relu](Relu.md) | 逐元素执行 ReLU。 |
| [prelu](Prelu.md) | 逐元素执行 PReLU。 |
| [leaky_relu](LeakyRelu.md) | 逐元素执行 Leaky ReLU。 |
| [operator+ / add](Add.md) | 加法以及带进位输出的加法。 |
| [add_c](AddC.md) | 执行带进位输入和输出的加法。 |
| [operator- / sub](Sub.md) | 减法以及带进位输出的减法。 |
| [sub_c](SubC.md) | 执行带进位输入和输出的减法。 |
| [operator*](Mul.md) | 逐元素乘法。 |
| [mull](Mull.md) | 计算 32 位整数宽乘的高、低部分。 |
| [operator/](Div.md) | 逐元素除法。 |
| [max](Max.md) | 逐元素取最大值。 |
| [min](Min.md) | 逐元素取最小值。 |

## 通用说明

- 接口只能在支持 `__simd_callee__` 调用的设备侧函数中使用。
- 返回值为 `reg_tensor<T>`，结果 mask 跟随参与计算的输入寄存器。
