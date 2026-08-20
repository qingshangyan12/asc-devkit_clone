# 基础算术

本目录介绍 `asc::te` 命名空间下基于 `reg_tensor<T>` 的基础算术、数学函数和激活函数接口。

## 头文件

```cpp
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/basic_arithmetic.h"
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
| [operator+](Add.md) | 逐元素执行普通加法。 |
| [add / add_c](AddC.md) | 执行带进位输出的加法，并支持传递进位。 |
| [operator-（二元）](Sub.md) | 逐元素执行普通减法。 |
| [sub / sub_c](SubC.md) | 执行带进位输出的补码减法，并支持传递进位。 |
| [operator*（mul）](Mul.md) | 逐元素执行寄存器与寄存器乘法。 |
| [operator*（muls）](Muls.md) | 逐元素执行寄存器与标量乘法。 |
| [mull](Mull.md) | 计算 32 位整数宽乘的高、低部分。 |
| [operator/](Div.md) | 逐元素除法。 |
| [max](Max.md) | 逐元素取最大值。 |
| [min](Min.md) | 逐元素取最小值。 |

## 通用说明

- 接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用。
- 返回对象的 `reg` 保存计算结果，`mask` 跟随接口选定的执行 mask。二元接口通常使用第一个
  `reg_tensor` 操作数的 `mask`，具体规则见对应接口页。
- 输入寄存器的执行 mask 必须在调用前初始化，通常使用 `load(...).with_mask(m)` 完成绑定。
