# min

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

逐元素返回两个操作数中的较小值，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种组合。

$$
dst_i = \min(a_i, b_i)
$$

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) min(const T& a, const U& b)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T`、`U` | 模板参数 | 至少一个模板参数为`reg_tensor`类型。寄存器元素类型支持`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`，标量须可转换为寄存器元素类型。 |
| `a` | 输入 | 左操作数，可以是`reg_tensor`或标量。`a`为`reg_tensor`时，`a.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `b` | 输入 | 右操作数，可以是与`a`元素类型相同的`reg_tensor`或可转换为该元素类型的标量。`a`为标量时，使用`b.mask`标记参与计算的元素。`a`和`b`均为`reg_tensor`时，`b.mask`不用于本次计算。 |

## 返回值说明

返回与寄存器操作数元素类型相同的`reg_tensor`。`reg`保存两个操作数逐元素比较后的较小值，计算所用的输入`mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与本次计算所用的输入`mask`相同。

## 约束说明

浮点输入分别为`-0`和`+0`时，返回`-0`。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void min_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::all_mask<float>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = asc::te::experimental::min(src0_reg, src1_reg);
    output.store(asc::te::make_coord(0, 0), result);
}
```
