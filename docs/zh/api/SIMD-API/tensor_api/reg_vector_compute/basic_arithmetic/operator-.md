# operator-

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

头文件路径：`"tensor_api/tensor.h"`。

命名空间：`asc::te::experimental`。

二元`operator-`逐元素执行不带借位输出的减法，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种操作数组合。

$$
dst_i = a_i - b_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(
    const reg_tensor<T>& src, const T& scalar)

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(
    const T& scalar, const reg_tensor<T>& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。 |
| `src0` | 输入 | 寄存器/寄存器形式中的被减数，类型为`reg_tensor<T>`。`src0.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `src1` | 输入 | 寄存器/寄存器形式中的减数，类型为`reg_tensor<T>`。`src1.mask`不用于本次计算。 |
| `src` | 输入 | 寄存器/标量或标量/寄存器形式中的寄存器操作数，类型为`reg_tensor<T>`。`src.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `scalar` | 输入 | 标量操作数，类型为`T`。 |

## 返回值说明

返回`reg_tensor<T>`。`reg`保存逐元素减法结果，计算所用的输入`mask`对应位置为`0`时，`reg`的对应位置置零。寄存器/寄存器形式的返回值`mask`与`src0.mask`相同，包含标量的形式与`src.mask`相同。

## 约束说明

- 整型dtype减法溢出时按环绕（wrap-around）策略处理：有符号类型溢出回绕到对应dtype的最小值或最大值（MIN-1->MAX，MAX+1->MIN），无符号类型下溢回绕到对应dtype的最大值（0-1->UMAX）。
- 浮点dtype（half、bfloat16_t、float）减法按IEEE 754浮点减法语义执行，溢出与无效输入（如nan、inf）的处理遵循浮点运算规则。
- 标量类型必须与寄存器元素类型一致。
- 标量位于左侧时执行`scalar - reg`，操作数顺序不会交换。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void sub_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    using asc::te::experimental::operator-;
    using asc::te::experimental::all_mask;
    using asc::te::experimental::load;
    using asc::te::experimental::store;

    auto mask = all_mask<uint32_t>();
    auto src0 = load(input0, asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1 = load(input1, asc::te::make_coord(0, 0)).with_mask(mask);
    auto result = src0 - src1;
    store(output, asc::te::make_coord(0, 0), result);
}
```
