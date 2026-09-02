# and

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"tensor_api/experimental/vector_compute.h"`。

逐元素执行按位与运算，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种形式。

$$
dst_i = a_i \,\mathbin{\&}\, b_i
$$

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator&(const T& a, const U& b)
```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T、U | 操作数类型。两个参数中必须至少有一个为reg_tensor，且两个操作数的元素类型需保持一致。支持的数据类型请参考[数据类型](#数据类型)。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a` | 输入 | 左操作数，可以是`reg_tensor`或标量。`a`为`reg_tensor`时，`a.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `b` | 输入 | 右操作数，可以是与`a`元素类型相同的`reg_tensor`或可转换为该元素类型的标量。`a`为标量时，使用`b.mask`标记参与计算的元素。`a`和`b`均为`reg_tensor`时，`b.mask`不用于本次计算。 |

`bool`类型要求`a`和`b`均为`reg_tensor<bool>`，接口使用`a.mask`标记参与计算的元素。

## 数据类型

| 操作数组合 | 寄存器元素类型 |
| --- | --- |
| 寄存器/寄存器 | `bool`、`int8_t`、`uint8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` |
| 寄存器/标量、标量/寄存器 | `int8_t`、`uint8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` |

## 返回值说明

返回与寄存器操作数元素类型相同的`reg_tensor`。`reg`保存逐元素按位与结果，计算所用的输入`mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与本次计算所用的输入`mask`相同。

## 约束说明

- 用于控制计算的mask需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- 构造`reg_tensor<bool>{maskValue}`只初始化对象的mask，不会生成布尔寄存器数据。作为操作数前，reg必须已经由mask生成、加载或其他布尔计算接口写入。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void bit_and_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    using asc::te::experimental::operator&;

    auto mask = asc::te::experimental::all_mask<int32_t>();
    auto src0_reg = asc::te::experimental::load(input0, asc::te::make_coord(0)).with_mask(mask);
    auto src1_reg = asc::te::experimental::load(input1, asc::te::make_coord(0)).with_mask(mask);

    auto result = src0_reg & src1_reg;
    asc::te::experimental::store(output, asc::te::make_coord(0), result);
}
```
