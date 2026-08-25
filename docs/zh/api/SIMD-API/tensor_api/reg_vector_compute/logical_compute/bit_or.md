# operator|

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

逐元素执行按位或运算，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种形式。

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator|(const T& a, const U& b)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a` | 输入 | 左操作数。非`bool`类型可以是寄存器或标量；为寄存器时优先使用`a.mask`。 |
| `b` | 输入 | 右操作数。非`bool`类型可以是同类型寄存器或可转换标量；仅`b`为寄存器时使用`b.mask`。 |

`bool`类型要求`a`、`b`均为`reg_tensor<bool>`：`reg`保存predicate数据，执行mask取`a.mask`。

## 支持的数据类型

| 参数 | 支持的数据类型 |
| --- | --- |
| 寄存器元素类型 | `bool`、`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float` |

## 返回值说明

返回与寄存器操作数元素类型相同的`reg_tensor`：

- `reg`：有效元素保存两个操作数逐bit或的结果。普通矢量类型的无效元素置零；
  `bool`使用predicate指令。
- `mask`：包括`reg_tensor<bool>`在内，均等于第一个寄存器操作数的`mask`。

## 约束说明

- mask初始化和传导规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
- 双寄存器元素类型不一致时，编译失败，并提示寄存器操作数必须具有相同的元素类型。
- 元素类型不在“支持的数据类型”列表中时，编译失败，并提示`operator| does not support this element type`。
- `bool`与标量组合时，编译失败，并提示bool按位运算需要两个`reg_tensor<bool>`操作数。
- 构造`reg_tensor<bool>{maskValue}`只初始化对象的`mask`，不会生成predicate数据；作为操作数前，
  `reg`必须已经由mask生成、加载或其他predicate计算接口写入。
- 对浮点类型执行的是元素的二进制编码的按位或，不是数值逻辑或；结果可能为`nan`或其他特殊浮点值。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

同类Tensor API样例的Kernel、Host调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/logical_compute/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void bit_or_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    using asc::te::experimental::operator|;

    auto mask = asc::te::all_mask<int32_t>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = src0_reg | src1_reg;
    output.store(asc::te::make_coord(0, 0), result);
}
```
