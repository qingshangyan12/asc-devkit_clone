# operator-（二元）

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

二元`operator-`执行逐元素减法，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种组合。

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator-(const T& a, const U& b)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a` | 输入 | 被减数，可以是`reg_tensor`或标量；`a`为寄存器时优先使用`a.mask`。 |
| `b` | 输入 | 减数，可以是同元素类型寄存器或可转换标量；仅`b`为寄存器时使用`b.mask`。 |

## 支持的数据类型

| 参数 | 支持的数据类型 |
| --- | --- |
| 寄存器元素类型 | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` |

## 返回值说明

返回元素类型与寄存器操作数相同的`reg_tensor`：

- `reg`：有效元素保存左操作数减右操作数的结果。整型结果超出`T`的位宽时，
  仅保留目标类型位宽内的结果。
- `mask`：等于第一个`reg_tensor`操作数的`mask`。

## 约束说明

- mask初始化和传导规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
- 双寄存器元素类型不一致时，编译失败，并提示寄存器操作数必须具有相同的元素类型。
- 整型`operator-`的结果超出元素类型表示范围时采用非饱和截断。
- 标量位于左侧时执行`scalar - reg`，操作数顺序不会交换。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

同类Tensor API样例的Kernel、Host调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/basic_arithmetic/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void sub_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    using asc::te::experimental::operator-;

    auto mask = asc::te::all_mask<uint32_t>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto difference = src0_reg - src1_reg;
    output.store(asc::te::make_coord(0, 0), difference);
}
```
