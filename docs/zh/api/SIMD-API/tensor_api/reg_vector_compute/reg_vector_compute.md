# Reg 矢量计算

Tensor API Reg 矢量计算接口以 `asc::te::reg_tensor<T>` 为输入和返回值，mask 随寄存器对象传递，
支持返回值风格和 C++ 运算符表达式。

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 接口分类

| 分类 | 说明 |
| --- | --- |
| [基础算术](basic_arithmetic/basic_arithmetic.md) | 基础算术、数学函数、激活函数、进位和宽乘。 |
| [逻辑与位运算](logical_compute/logical_compute.md) | 逻辑非、按位运算和移位。 |
| [复合计算](composite_compute/composite_compute.md) | 乘加、差值、指数和类型转换等融合计算。 |
| [直方图计算](histogram_compute/histogram_compute.md) | 频率直方图和累计直方图。 |

## 编程模型

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void reg_vector_compute_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    using asc::te::experimental::operator+;

    auto mask = asc::te::all_mask<float>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = asc::te::experimental::abs(src0_reg) + src1_reg;
    output.store(asc::te::make_coord(0, 0), result);
}
```

接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
`__simd_vf__` 函数直接或间接调用。`reg_tensor<T>`
中的 `reg` 保存矢量数据，`mask` 表示参与计算的有效 lane；调用计算接口前必须先初始化输入寄存器的
`mask`，通常使用 `load(...).with_mask(m)` 完成绑定。
